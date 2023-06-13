
'''
    test getting ip's of all server instances
'''
import sys
import os
import getpass

current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(current_dir))

from proxy.server_pool import ServerPool
from proxy.proxy import LoadBalancer



def test_server_pool():
    """ testing getting ip's from all server instances

    Optional more detailed explanation or additional information about the function.

    No Args

    echo to string the ip's
        type: Description of the return value(s).

    Raises:
        ExceptionType: Description of when and why this exception may be raised.

    """
    p = ServerPool()

    for ip, port in p.get_ips():
        print(ip+":"+port)

def test_proxy():

    LoadBalancer('localhost', 5555, 'round robin').start_thread()
    print("Press any key to exit...")
    getpass.getpass()

test_server_pool()
test_proxy()
