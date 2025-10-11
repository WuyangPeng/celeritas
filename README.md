# celeritas

本设计旨在为一款高性能游戏服务器提供一个基础框架。该框架将使用C++20标准和boost库，实现一个异步、多线程、高吞吐量的系统。

## Supported Platforms（支持平台）:

- windows
- linux

## Supported Compiler（支持编译器）:

- MinGW 15.2 (windows)
- Visual Studio 2022  (windows)
- gcc 14.2  (linux)

## Dependency Library（依赖库）:

- boost 1.88.0
- protobuf 31.1
- openssl 3.5.1
- mongo v4.1
- hiredis v1.3.0

## Server（服务器）:

- service registry 服务注册中心
