cmake_minimum_required(VERSION 3.20)
project(Demo C)

set(CMAKE_C_STANDARD 17)

add_executable(Demo client.c server.c main.c)
