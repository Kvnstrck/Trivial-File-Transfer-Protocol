cmake_minimum_required(VERSION 3.31)
project(TFTP_Implementation)

set(CMAKE_CXX_STANDARD 20)

add_executable(tftp main.cpp
        utils/udp_utils.cpp
        utils/udp_utils.h
        utils/tftp_utils.h
)
