//
// Created by kecs on 15.10.25.
//

#include "udp_utils.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace utils {
    /**
     * Creates a new udp server socket.
     * @param port the port to use for the server socket
     */
    int udp_utils::create_udp_server_socket(const u_int32_t port) {
        int sock_fd;

        // create udp server socket, exit if syscall fails
        if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            std::perror("socket creation failed");
            std::exit(EXIT_FAILURE);
        }

        // create address data structure and fill in address information
        sockaddr_in server_information = {};

        //fill in server information
        server_information.sin_addr.s_addr = INADDR_ANY; //set socket to all available interfaces
        server_information.sin_family = AF_INET; //internet domain socket family for IPv4
        server_information.sin_port = htons(port); //port in network byte order

        // Bind the socket with the server address, exit if syscall fails
        if (bind(sock_fd, reinterpret_cast<const struct sockaddr *>(&server_information),
                 sizeof(server_information)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        //add string terminator to buffer
        //TODO: when file is put together, this is unnecessary

        return sock_fd;
    }

    sockaddr_in udp_utils::receive_udp_message(int socket_fd, char *buffer) {
        sockaddr_in client_information = {};
        socklen_t client_information_length = sizeof(client_information);

        //wait for the UDP packet to arrive and write it in buffer
        const int n = recvfrom(socket_fd, (char *)buffer, 1024,
                MSG_WAITALL, reinterpret_cast<struct sockaddr *>(&client_information),
                &client_information_length);

        buffer[n] = '\0';


        return client_information;
    }
} // utils
