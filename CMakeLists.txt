cmake_minimum_required(VERSION 3.18)
project(Demo C)

set(CMAKE_C_STANDARD 11)

option(USE_SERVER "Use server module " ON)   # If use client, set ON to OFF(Re-compile after refreshing cmake cache)

if (USE_SERVER)
    message("Use server module")

    add_definitions(-DUSE_SERVER)

    add_subdirectory(server)

    add_executable(ServerDemo main.c)

    target_link_libraries(ServerDemo Server)
else ()
    message("Use client module")

    add_subdirectory(client)

    add_executable(ClientDemo main.c)

    target_link_libraries(ClientDemo Client)
endif ()
