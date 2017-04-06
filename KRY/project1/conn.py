"""Connect processes using named pipes."""
import os
import errno
from base64 import b64decode, b64encode

from Crypto.Cipher import AES


class Connection:
    """Named pipe object class."""

    def __init__(self, name, mode):
        """Create and open pipe."""
        self.fd = None
        self.name = name

        try:
            os.mkfifo(name)
        except OSError as e:
            if e.errno != errno.EEXIST:
                raise

        self.fd = open(name, mode)

    def close(self):
        """Close and remove pipe from file system."""
        self.fd.close()
        os.remove(self.name)

    def receive(self):
        r"""Method for receiving data from pipe.

        The method is reading a line and strips the '\n' character at the end.
        """
        while True:
            data = self.fd.readline()
            if not data:
                continue
            yield data[:-1]

    def send(self, data):
        """Push date to the pipe as a line of text."""
        self.fd.write(data + '\n')
        self.fd.flush()


def msg_encrypt(aes, msg):
    """Message encryption handler."""
    # Align to block size
    padded = bytes(msg) + '\0' * (AES.block_size - len(msg) % AES.block_size)
    # Encrypt
    msg = aes.encrypt(padded)
    # Encode to base64
    return b64encode(msg)


def msg_decrypt(aes, msg):
    """Message decryption handler."""
    # Decode Base64
    decoded_msg = b64decode(msg)
    return aes.decrypt(decoded_msg).strip('\0')
