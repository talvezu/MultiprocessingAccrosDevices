from jinja2 import Environment, FileSystemLoader
import yaml

template_files=["nginx.conf.j2", "docker_compose.yaml.j2"]
for template_file in template_files:
    out_file = template_file.replace(".j2", "")
    # Remove the ".j2" postfix
    with open('servers.yaml', 'r') as f:
        server_config = yaml.safe_load(f)

    # Create a Jinja2 environment
    env = Environment(loader=FileSystemLoader('.'))
    # Load the template file
    template = env.get_template(template_file)
    # Render the template with the server configurations
    rendered_output = template.render(servers=server_config['servers'])

    with open(out_file, 'w') as f:
        f.write(rendered_output)
