cmake_minimum_required(VERSION 3.31)
project(TFTP_Implementation)

set(CMAKE_CXX_STANDARD 20)

add_executable(tftp main.cpp
        utils/UDP_Utils.cpp
        utils/UDP_Utils.h
        utils/TFTP_Utils.h
        utils/TFTP_Utils.cpp
        Packet.cpp
        Packet.h
        Connection_Endpoints/Connection_Endpoint_Client.cpp
        Connection_Endpoints/Connection_Endpoint_Client.h
        Connection_Endpoints/Connection_Endpoint_Server.cpp
        Connection_Endpoints/Connection_Endpoint_Server.h
        Connection_Endpoints/Connection_Endpoint.cpp
        Connection_Endpoints/Connection_Endpoint.h
        main.cpp
)
