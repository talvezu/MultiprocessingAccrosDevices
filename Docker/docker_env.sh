#!/bin/bash


python3 generate_conf_files.py
docker-compose -f docker_compose.yaml up -d
