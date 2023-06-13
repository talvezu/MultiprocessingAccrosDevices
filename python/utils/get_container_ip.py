import subprocess
#from subprocess_wrapper import *
from utils.common_cmds import *


def get_container_ip(name):
    res = subprocess_run(None)


def _get_docker_containers():
    docker_command = get_app_container_map()
    output = subprocess.check_output(docker_command, shell=True, universal_newlines=True)

    # Process the output and create a map
    container_info = {}
    containers_info = {}
    lines = output.strip().split('\n')
    for line in lines:
        container_id, container_name = line.split()
        #container_info[container_id] = container_name
        container_info["name"] = container_name
        containers_info[container_id] = container_info

    return containers_info


def get_server_app_docker_containers():
    # Example usage
    containers_info = _get_docker_containers()
    for _id, _info in containers_info.items():
        docker_command = get_container_exopted_ip(_id)
        output = subprocess.check_output(docker_command, shell=True, universal_newlines=True)
        _info["ip"] = output.strip().split('\n')
            #containers_info[name,container_id] = container_name

        # port
        docker_command = get_container_exopted_port(_id)
        output = subprocess.check_output(docker_command, shell=True, universal_newlines=True)
        _info["port"] = output.strip().split('\n')

    return containers_info
