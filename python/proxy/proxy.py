'''
based on:
https://gist.github.com/zhouchangxun/5750b4636cc070ac01385d89946e0a7b
save in project as ref.py for easier developement

simple loadbalancer
supports roundrobin and random
needs to add
 least connections, continous IP hash
'''

import sys
import socket
import select
import random
import threading
from itertools import cycle
from proxy.server_pool import ServerPool
'''
# dumb netcat server, short tcp connection
# $ ~  while true ; do nc -l 8888 < server1.html; done
# $ ~  while true ; do nc -l 9999 < server2.html; done
SERVER_POOL = [('10.157.0.238', 8888)]

# dumb python socket echo server, long tcp connection
# $ ~  while  python server.py
# SERVER_POOL = [('localhost', 6666)]

ITER = cycle(SERVER_POOL)
def round_robin(iter):
    # round_robin([A, B, C, D]) --> A B C D A B C D A B C D ...
    return next(iter)
'''


class LoadBalancer(object):
    """ Socket implementation of a load balancer.

    Flow Diagram:
    +---------------+      +-----------------------------------------+      +---------------+
    | client socket | <==> | client-side socket | server-side socket | <==> | server socket |
    |   <client>    |      |          < load balancer >              |      |    <server>   |
    +---------------+      +-----------------------------------------+      +---------------+

    Attributes:
        ip (str): virtual server's ip; client-side socket's ip
        port (int): virtual server's port; client-side socket's port
        algorithm (str): algorithm used to select a server
        flow_table (dict): mapping of client socket obj <==> server-side socket obj
        sockets (list): current connected and open socket obj
    """

    flow_table = dict()
    sockets = list()

    def __init__(self, ip, port, algorithm='random'):
        self.ip = ip
        self.port = port
        self.algorithm = algorithm

        # init a client-side socket
        self.cs_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # the SO_REUSEADDR flag tells the kernel to reuse a local socket in TIME_WAIT state,
        # without waiting for its natural timeout to expire.
        self.cs_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.cs_socket.bind((self.ip, self.port))
        print(f"init client-side socket: {self.cs_socket.getsockname()}")
        self.cs_socket.listen(10)  # max connections
        self.sockets.append(self.cs_socket)
        self.server_pool = ServerPool()
        self.iter = cycle(self.server_pool.get_ips())
        self.thread = None

    def start(self):
        while True:
            read_list, write_list, exception_list = select.select(
                self.sockets, [], [])
            for sock in read_list:
                # new connection
                if sock == self.cs_socket:
                    print(f"{'='*40}flow start{'='*39}")
                    self.on_accept()
                    break
                # incoming message from a client socket
                else:
                    try:
                        # In Windows, sometimes when a TCP program closes abruptly,
                        # a "Connection reset by peer" exception will be thrown
                        data = sock.recv(4096)  # buffer size: 2^n
                        if data:
                            self.on_recv(sock, data)
                        else:
                            self.on_close(sock)
                            break
                    except:
                        sock.on_close(sock)
                        break

    def start_thread(self):
        # Create and start the internal thread
        thread = threading.Thread(target=self.start)
        thread.start()

    def on_accept(self):
        client_socket, client_addr = self.cs_socket.accept()
        print(
            f"client connected: {client_addr} <==> {self.cs_socket.getsockname()}")

        # select a server that forwards packets to
        print("1")
        self.server_pool.refresh()
        print("2")
        server_ip, server_port = self.select_server(
            self.server_pool.get_ips, self.algorithm)
        print(f"{server_ip}:{server_port}")
        # init a server-side socket
        ss_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            ss_socket.connect((server_ip, server_port))
            print(f"init server-side socket: {ss_socket.getsockname()}")
            print(
                f"server connected: {ss_socket.getsockname()} <==> {socket.gethostbyname(server_ip)}:{server_port}")
        except BaseException:
            print(
                f"Can't establish connection with remote server, err: {sys.exc_info()[0]}")
            print(f"Closing connection with client socket {client_addr}")
            client_socket.close()
            return

        self.sockets.append(client_socket)
        self.sockets.append(ss_socket)

        self.flow_table[client_socket] = ss_socket
        self.flow_table[ss_socket] = client_socket

    def on_recv(self, sock, data):
        print(
            f"recving packets: {sock.getpeername()} ==> {sock.getsockname()} data: {[data]}")
        # data can be modified before forwarding to server
        # lots of add-on features can be added here
        remote_socket = self.flow_table[sock]
        remote_socket.send(data)
        print(
            f"sending packets: {remote_socket.getsockname()} ==> {remote_socket.getpeername()}, data: {[data]}")

    def on_close(self, sock):
        print("client {sock.getpeername()} has disconnected")
        print(f"{'='*41}flow end{'='*40}")

        ss_socket = self.flow_table[sock]

        self.sockets.remove(sock)
        self.sockets.remove(ss_socket)

        sock.close()  # close connection with client
        ss_socket.close()  # close connection with server

        del self.flow_table[sock]
        del self.flow_table[ss_socket]

    def select_server(self, server_list, algorithm):
        if algorithm == 'random':
            print(f"algorithm random call {server_list}")
            return random.choice(server_list)
        elif algorithm == 'round robin':
            print(f"algorithm roundrobin call {server_list}")
            return self.round_robin()
        else:
            raise Exception('unknown algorithm: %s' % algorithm)

    def round_robin(self):
        # round_robin([A, B, C, D]) --> A B C D A B C D A B C D ...
        return next(self.iter)

    def refresh(self):
        self.iter = cycle(self.server_pool.get_ips)


if __name__ == '__main__':
    try:
        LoadBalancer('localhost', 5555, 'round robin').start()
    except KeyboardInterrupt:
        print("Ctrl C - Stopping load_balancer")
        sys.exit(1)
