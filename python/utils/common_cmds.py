def get_app_container_map():
    cmd = "docker ps | grep server_app | awk \'{print $1 \" \" $NF}\'"
    return cmd

def get_container_exopted_ip(container_id):
    cmd = "docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}'" + f" {container_id}"
    return cmd

def get_container_exopted_port(container_id):
    cmd = "docker inspect -f '{{.NetworkSettings.Ports}}' " + f" {container_id}" + "| awk -F'[ }]' '{print $2}'"
    return cmd
