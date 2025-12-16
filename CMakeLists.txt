cmake_minimum_required(VERSION 3.31)
project(TFTP_Implementation)

set(CMAKE_CXX_STANDARD 20)

add_executable(tftp main.cpp
        utils/UDP_Utils.cpp
        utils/UDP_Utils.h
        utils/TFTP_Utils.h
        Packet_Helper.cpp
        Packet_Helper.h
        TFTP_Connection_State.cpp
        TFTP_Connection_State.h
)
