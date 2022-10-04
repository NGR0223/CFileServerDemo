cmake_minimum_required(VERSION 3.20)
project(Demo C)

set(CMAKE_C_STANDARD 17)


add_executable(Demo server/server.c client client/client.c main.c)

set(SERVER YES) # if use client, set YES to NO
if (SERVER)
    add_subdirectory(server)
    target_link_libraries(Demo Server)
else ()
    add_subdirectory(client)
    target_link_libraries(Demo Client)
endif ()
