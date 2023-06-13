PROJECT_ROOT := $(shell pwd)

all: prepare

prereq:
	sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
	docker pull selenium/standalone-chrome

install:
	sudo apt-get install gcc g++ cmake make doxygen git llvm pkg-config curl zip unzip tar python3-dev clang-format clang-tidy

prepare:
	rm -rf build
	mkdir build

conan_d:
	rm -rf build
	mkdir build
	cd build && conan install .. -s build_type=Debug --output-folder=. --build missing -s compiler.cppstd=17

conan_r:
	rm -rf build
	mkdir build
	cd build && conan install .. -s build_type=Release --output-folder=. --build missing -s compiler.cppstd=17

prep_container_env:
	mkdir -p docker_env
	cp requirements.txt docker_env/
	cp Docker/Dockerfile docker_env/
	cp Docker/docker_env.sh docker_env/
	cp Docker/generate_conf_files.py docker_env/
	cp Docker/*.j2 docker_env/
	cp Docker/servers.yaml docker_env/
	cp -r python docker_env/

build_server_container: prep_container_env
	docker build -t server_app_image --progress=plain --build-arg PROJECT_ROOT=$(PROJECT_ROOT) docker_env/

run_pod: build_server_container
	cd docker_env && ./docker_env.sh 

run_single_server:
	docker run -p 8000:8000 server_app_image

stop_pod:
	@pwd
	cd docker_env && docker-compose -f docker_compose.yaml down


test_selenium:
	docker run -d -p 4444:4444 --shm-size=2g selenium/standalone-chrome

pep_inplace:
	autopep8 --in-place --aggressive ${file}
