'''
basically just holds get_ips
has a method to refresh to reflect current servers ip's
'''
from utils.get_container_ip import get_server_app_docker_containers


class ServerPool:
    def __init__(self):
        self.refresh()

    def get_ips(self):
        return self.ips

    def refresh(self):
        self.containers_info = get_server_app_docker_containers()
        self.ips = set()
        for _, v in self.containers_info.items():
            pair = (v["ip"][0], v["port"][0])
            self.ips.add(pair)
