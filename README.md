# Expandable flask server which serves streams
    python
      includes
        - utils
          getting ips of server containers.
        - proxy, WIP for manual load balance, currently on hold
          due to using nginx instance as load balancer
        - tests/test_server_pool check the proxy and the utils
        - server.py main file
          a flask server that accepts client and generate vector
          periodically

    Docker
      includes all files needed for rendering and running server instance

      server.yaml list the servers required.
      script for rendering - generate_conf_files.py
        - nginx.conf.j2
        - docker_compose.j2
      docker compose then spawns
        - N server container
        - 1 nginx which is a load balancer between the servers

      Docker folder is being used by
        - make build_server_container
            generate server_app_image which will be used for
            server instances
        - run_pod, which runs build_server_container
            and executes docker_env (originally copied from Docker to docker_env)

    Client
      c++ executable (matrix_client) that address the load balancer
       ./client/matrix_client --client_id 123 --frequency 10
      and received a periodic vector according to frequency
      50 values vector
      [0.3458259242494935, -0.7134870108521053, -0.18688024808856107, ...]

![C++](https://img.shields.io/badge/C%2B%2B-11%2F14%2F17%2F20%2F23-blue)
![License](https://camo.githubusercontent.com/890acbdcb87868b382af9a4b1fac507b9659d9bf/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f6c6963656e73652d4d49542d626c75652e737667)
![Linux Build](https://github.com/franneck94/CppProjectTemplate/workflows/Ubuntu%20CI%20Test/badge.svg)
[![codecov](https://codecov.io/gh/franneck94/CppProjectTemplate/branch/master/graph/badge.svg)](https://codecov.io/gh/franneck94/CppProjectTemplate)

This is a template for C++ projects. What you get:

- Library, executable and test code separated in distinct folders
- Use of modern CMake for building and compiling
- External libraries installed and managed by
  - [CPM](https://github.com/cpm-cmake/CPM.cmake) Package Manager OR
  - [Conan](https://conan.io/) Package Manager OR
  - [VCPKG](https://github.com/microsoft/vcpkg) Package Manager
- Unit testing using [Catch2](https://github.com/catchorg/Catch2) v2
- General purpose libraries: [JSON](https://github.com/nlohmann/json), [spdlog](https://github.com/gabime/spdlog), [cxxopts](https://github.com/jarro2783/cxxopts) and [fmt](https://github.com/fmtlib/fmt)
- Continuous integration testing with Github Actions and [pre-commit](https://pre-commit.com/)
- Code coverage reports, including automatic upload to [Codecov](https://codecov.io)
- Code documentation with [Doxygen](https://doxygen.nl/) and [Github Pages](https://franneck94.github.io/CppProjectTemplate/)
- Tooling: Clang-Format, Cmake-Format, Clang-tidy, Sanitizers

## Structure

``` text
├── CMakeLists.txt
├── app
│   ├── CMakesLists.txt
│   └── main.cc
├── client (executable - matrix_client)
│   ├── CMakesLists.txt
│   └── client.cpp
├── cmake
│   └── cmake modules
├── docs
│   ├── Doxyfile
│   └── html/
├── external
│   ├── CMakesLists.txt
│   ├── ...
├── python
│   ├── server.py
│   ├── proxy
|   │   └── ....
│   ├── utils
|   │   └── ....
│   └── tests
├── src
│   ├── CMakesLists.txt
│   ├── my_lib.h
│   └── my_lib.cc
├── tests
│   ├── CMakeLists.txt
│   └── main.cc
└── Docker
    ├── docker_compose.yaml.j2
    ├── docker_env.sh
    ├── Dockerfile
    ├── Dockerfile_flask
    ├── generate_conf_files.py
    ├── nginx.conf.j2
    └── servers.yaml
```

Library code goes into [src/](src/), main program code in [app/](app) and tests go in [tests/](tests/).

## Software Requirements

- CMake 3.21+
- GNU Makefile
- Doxygen
- Conan or VCPKG
- MSVC 2017 (or higher), G++9 (or higher), Clang++9 (or higher)
- Optional: Code Coverage (only on GNU|Clang): lcov, gcovr
- Optional: Makefile, Doxygen, Conan, VCPKG

## Building

First, clone this repo and do the preliminary work:

```shell
git clone --recursive https://github.com/franneck94/CppProjectTemplate
make prepare
```

- App Executable

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release --target main
cd app
./main
```

- Unit testing

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target unit_tests
cd tests
./unit_tests
```

- Documentation

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target docs
```

- Code Coverage (Unix only)

```shell
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON ..
cmake --build . --config Debug --target coverage
```

For more info about CMake see [here](./README_cmake.md).
