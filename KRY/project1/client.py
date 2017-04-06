import logging
from hashlib import sha256
from base64 import b64decode

from Crypto.Cipher import AES

from conn import Connection, msg_encrypt, msg_decrypt
from dh import DiffieHellman
from ffs import FFSProver

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)


def get_response(link, phase, params, fail_msg):
    """Get response and check if the state is correct."""
    response = next(link.receive())
    response = response.split(',')

    if len(response) != params or response[0] != phase:
        raise RuntimeError(fail_msg)
    return response


def client():
    """Client mode work flow."""
    downlink = Connection('downlink', 'r')
    uplink = Connection('uplink', 'w+')

    # Establish connection
    dh = DiffieHellman()
    p, g = dh.get_pg()
    uplink.send('INIT,{0},{1}'.format(p, g))

    response = get_response(downlink, 'MY_PUBLIC', 2,
                            'Failed to authenticate via DH')

    public = dh.get_my_public()
    uplink.send('MY_PUBLIC,{0}'.format(public))
    key = dh.compute_shared(response[1])

    logging.debug('Key established: {0}'.format(key.hexdigest()))

    # Start encrypted communication
    response = get_response(downlink, 'AES', 2, 'Failed to start AES')
    aes = AES.new(key.digest(), AES.MODE_CBC, b64decode(response[1]))

    # Verify identity using FFS
    while True:
        ffs = FFSProver()
        # Send X
        msg = msg_encrypt(aes, str(ffs.get_x()))
        uplink.send('FFS_X,{0}'.format(msg))
        # Get A vector
        response = get_response(downlink, 'FFS_A', 2,
                                'Failed to verify identity via FFS')
        a_vector = [int(a) for a in msg_decrypt(aes, response[1])]
        # Solve Y
        msg = msg_encrypt(aes, str(ffs.solve(a_vector)))
        uplink.send('FFS_Y,{0}'.format(msg))
        # Get result
        response = next(downlink.receive())
        if response not in ('FFS_OK', 'FFS_DONE'):
            raise RuntimeError('Failed to verify identity via FFS')
        if response == 'FFS_DONE':
            break

    # Send messages
    print('Connection established, communication channel open.')
    print('If you want to end this program press CTRL(C')
    while True:
        # Snd message
        raw_msg = raw_input('Enter message: ')
        msg = msg_encrypt(aes, raw_msg)
        uplink.send('MSG,{0}'.format(msg))

        # Get response
        response = next(downlink.receive())
        response = response.split(',')

        if len(response) != 2 or response[0] != 'SHA256':
            raise RuntimeError('Channel compromised')

        # Compare Hashes
        our_hash = sha256(raw_msg).hexdigest()
        their_hash = msg_decrypt(aes, response[1])
        print('Recieved: ' + their_hash)

        if their_hash != our_hash:
            print('Hash mismatch!')
            raise RuntimeError('Channel compromised')
        else:
            print('Hash matches')
