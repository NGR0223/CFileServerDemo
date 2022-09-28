cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 17)

aux_source_directory(. DIR_LIB_SRCS)

set(STATIC YES) # if want a dynamic library, set YES to NO
if (STATIC)
    # Static library
    add_library(Server ${DIR_LIB_SRCS})
else ()
    # Dynamic library
    SET(CMAKE_C_FLAGS "-fPIC")
    ADD_LIBRARY(Server SHARED ${DIR_LIB_SRCS})
endif ()