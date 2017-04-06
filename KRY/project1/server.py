import logging
from hashlib import sha256
from base64 import b64encode

from Crypto.Cipher import AES
# from Crypto import Random

from conn import Connection, msg_encrypt, msg_decrypt
from dh import DiffieHellman
from ffs import FFSVerifier

logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)
logger = logging.getLogger(__name__)

# server listening phases

STORAGE = dict()
FFS_COUNTER = 4
SKIP_FFS = False


def request_start_dh(data):
    """Client is requesting a communication.

    Receive P and G for Diffie-Hellman
    """
    STORAGE['dh'] = DiffieHellman(p_g=data)
    logger.info('Client connected. Recieved DH request, sending public key')

    return 'MY_PUBLIC,{0}'.format(STORAGE['dh'].get_my_public())


def request_complete_dh(data):
    """Client sends his public key, generate shared secret."""
    key = STORAGE['dh'].compute_shared(data)
    STORAGE['key'] = key
    logging.info('Key established: {0}'.format(key.hexdigest()))

    # Start AES
    # On MERLIN there's no Crypto.Random
    with open("/dev/urandom", "rb") as rnd:
        iv = rnd.read(AES.block_size)
    # iv = Random.new().read(AES.block_size)
    STORAGE['aes'] = AES.new(key.digest(), AES.MODE_CBC, iv)
    logging.info('Starting AES channel, sending init vector: {0} '
                 '(Base64)'.format(b64encode(iv)))

    return 'AES,{0}'.format(b64encode(iv))


def request_start_ffs(data):
    """Received FFS X, proceed to send A vector."""
    STORAGE['ffs'] = FFSVerifier()
    x = long(msg_decrypt(STORAGE['aes'], data))
    STORAGE['ffs'].save_X(x)
    a_vect = ''.join(str(a) for a in STORAGE['ffs'].get_A())
    encrypted = msg_encrypt(STORAGE['aes'], a_vect)

    return 'FFS_A,{0}'.format(encrypted)


def request_verify_ffs(data):
    """Received FFS Y, verify sender."""
    y = long(msg_decrypt(STORAGE['aes'], data))
    return 'FFS_OK' if STORAGE['ffs'].verify(y) else 'FFS_FAIL'


def request_msg(data):
    """Client sends a message."""
    msg = msg_decrypt(STORAGE['aes'], data)
    logging.info('Secret message received: {0}'.format(msg))
    our_hash = sha256(msg).hexdigest()
    encrypted = msg_encrypt(STORAGE['aes'], our_hash)
    return 'SHA256,{0}'.format(encrypted)


def serve_request(data, next_phase, ffs_counter):
    """Serving requests sent to the server.

    :param data: request content
    :param secret: server's internal memory
    """
    # Parse request
    request = data.split(',')
    phase = request[0]

    try:
        if phase not in ('INIT', next_phase):
            logger.warning('Command not recognised: {0}'.format(request))
            return None, 'INIT'
        # call handler
        if phase == 'INIT':
            return request_start_dh(request[1:]), 'MY_PUBLIC'
        elif phase == 'MY_PUBLIC':
            do_ffs = 'MSG' if SKIP_FFS else 'FFS_X'
            return request_complete_dh(request[1]), do_ffs
        elif phase == 'FFS_X':
            return request_start_ffs(request[1]), 'FFS_Y'
        elif phase == 'FFS_Y':
            response = request_verify_ffs(request[1])
            if response == 'FFS_OK':
                logging.info('Client verified via FFS ({0}x times)'
                             ''.format(ffs_counter[0] + 1))
                ffs_counter[0] += 1
                if ffs_counter[0] == FFS_COUNTER:
                    return 'FFS_DONE', 'MSG'
                return 'FFS_OK', 'FFS_X'
            else:
                ffs_counter[0] = 0
                return 'FFS_FAIL', 'INIT'
        elif phase == 'MSG':
            return request_msg(request[1]), 'MSG'

    except (IndexError, KeyError):
        logger.warning('Unsupported message format: {0}'.format(request))
    except Exception as e:
        logger.error('Something failed: {0}'.format(e))

    return None, 'INIT'


def server():
    """Server listening on named pipes and serving forever."""
    downlink = Connection('downlink', 'w+')
    uplink = Connection('uplink', 'r')

    cur_phase = 'INIT'
    ffs_counter = [0]
    # Listen for messages
    for request in uplink.receive():
        logger.debug('IN\t ' + request)

        response, cur_phase = serve_request(request, cur_phase, ffs_counter)
        if not response:
            continue

        downlink.send(response)
        logger.debug('OUT\t ' + response)

    downlink.close()
    uplink.close()
