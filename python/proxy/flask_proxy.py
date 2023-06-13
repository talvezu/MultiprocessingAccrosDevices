import os
import sys
current_dir = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(current_dir))

from flask import Flask, request
import requests
from server_pool import ServerPool

app = Flask(__name__)
server_pool = ServerPool()

backend_servers = server_pool.get_ips()

current_server = 0

@app.route('/enable_stream', methods=['POST'])
def enable_stream():
    global current_server
    requests.post(backend_servers[current_server] + '/enable_stream')
    current_server = (current_server + 1) % len(backend_servers)
    return 'Stream enabled'

@app.route('/disable_stream', methods=['POST'])
def disable_stream():
    requests.post(backend_servers[current_server] + '/disable_stream')
    return 'Stream disabled'

if __name__ == '__main__':
    app.run()
