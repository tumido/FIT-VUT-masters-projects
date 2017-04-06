"""KRY project #1 runner."""
import sys

from server import server
from client import client


if __name__ == '__main__':
    # Works when called with -s or -c parameter
    try:
        if len(sys.argv) is 2 or sys.argv[1] in ('-c', '-s'):
            mode = {'-c': client, '-s': server}
            mode[sys.argv[1]]()
        else:
            sys.exit(1)
    except KeyboardInterrupt:
        print("Shutdown requested...exiting")
