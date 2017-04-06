"""Diffie-Hellman key exchange module."""
from random import randint
from hashlib import sha256

BIG_PI = 3141592653589793
DECIMAL = 15


class DiffieHellman:
    """Diffie-Hellman key exchange protocol."""

    def __init__(self, p_g=None):
        """Create instance using predefined P and G, generate these otherwise.

        :param p_g: P prime and G base numbers
        :type p_g:  tuple
        """
        if p_g is None:
            self.p, self.g = gen_shared_pg()
        else:
            self.p, self.g = p_g
        self.p = long(self.p)
        self.g = long(self.g)

        self.private = randint(1, self.p-1)
        self.shared = None

    def get_pg(self):
        """Return P and G pair."""
        return self.p, self.g

    def get_my_public(self):
        """Compute publicly shared version of the private key."""
        return pow(self.g, self.private, self.p)

    def compute_shared(self, others_public):
        """Decode shared secret from received data."""
        if not isinstance(others_public, long):
            others_public = long(others_public)

        self.shared = pow(others_public, self.private, self.p)
        key = str(self.shared)
        # Process the generated key via SHA256 to get 256 bits
        return sha256(key)


def gen_shared_pg():
    """Generator for P prime and G base.

    Function used to generate the proper prime taken from RFC 3526, Section 2
    https://www.ietf.org/rfc/rfc3526.txt
    """
    g = 2

    # pi multiplication approximation - Python cant handle the precission
    p = 2**1536 - 2**1472 - 1 + 2**64 * ((2**1406 * BIG_PI / DECIMAL) + 741804)
    return p, g
