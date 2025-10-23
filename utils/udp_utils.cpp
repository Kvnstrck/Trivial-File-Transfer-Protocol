//
// Created by kecs on 15.10.25.
//

#include "udp_utils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>

namespace utils {
    /**
     * Creates a new UDP socket.
     * @param port the port that the UDP socket should operate on.
     * @return a file descriptor that points to the created UDP socket.
     */
    int udp_utils::create_udp_socket(const u_int32_t port) {
        int sock_fd;

        // create udp socket, exit if syscall fails
        if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            std::perror("socket creation failed");
            std::exit(EXIT_FAILURE);
        }

        // create address data structure and fill in address information
        sockaddr_in socket_information = {};

        //fill in server information
        socket_information.sin_addr.s_addr = INADDR_ANY; //set socket to all available interfaces
        socket_information.sin_family = AF_INET; //internet domain socket family for IPv4
        socket_information.sin_port = htons(port); //port in network byte order

        // Bind the socket with the server address, exit if syscall fails
        if (bind(sock_fd, reinterpret_cast<const struct sockaddr *>(&socket_information),
                 sizeof(socket_information)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        //add string terminator to buffer
        //TODO: when file is put together, this is unnecessary

        return sock_fd;
    }

    /**
     * Listens to the specified socket and upon datagram receival, writes the received message into the buffer.
     * @param socket_fd The file descriptor of the socket to listen to.
     * @param buffer The buffer to write the received message into.
     * @return The socket information of the datagram sender.
     */
    sockaddr_in udp_utils::receive_udp_message(const int socket_fd, char *buffer) {
        //create an object for client information
        sockaddr_in client_information = {};
        socklen_t client_information_length = sizeof(client_information);

        //prepare socket options for timeout handling
        struct timeval timeout_parameters{};
        timeout_parameters.tv_sec = static_cast<int>(UDP_PROTOCOL_PARAMETERS::TIMEOUT_SECONDS);
        timeout_parameters.tv_usec = static_cast<int>(UDP_PROTOCOL_PARAMETERS::TIMEOUT_MICROSECONDS);

        //enable the timeout option for socket, throw error if syscall fails
        if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout_parameters, sizeof(timeout_parameters)) < 0) {
            throw std::runtime_error("Failed to set socket timeout");
        }

        //wait for the UDP packet to arrive and write it in the buffer
        const ssize_t n = recvfrom(socket_fd, buffer, UDP_PROTOCOL_PARAMETERS::RECEIVE_BUFFER_SIZE,
                                   MSG_WAITALL, reinterpret_cast<struct sockaddr *>(&client_information),
                                   &client_information_length);

        //check if the receiving of UDP datagram failed
        if (n < 0) {
            //check for blocking or unavailable resource
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                //TODO: when protocol is usable enable timeouts and implement sending the packets again
                std::cout << "TIMEOUT HAPPEND!!!";
            }
        }

        //add string terminator to buffer
        buffer[n] = '\0';

        return client_information;
    }

    /**
     * Sends the message stored in the buffer from the given socket to the given ip/port combination.
     * @param sock_fd The socket to send the datagram from.
     * @param buffer The buffer holding the data to send.
     * @param port The port of the target socket.
     * @param ip The IP of the target socket.
     */
    void udp_utils::send_udp_message(const int sock_fd, const char *buffer, const int port, const char *ip) {
        sockaddr_in server_information = {};

        server_information.sin_family = AF_INET;
        server_information.sin_port = htons(port);

        if (inet_pton(AF_INET, ip, &server_information.sin_addr) <= 0) {
            printf("Invalid address/ Address not supported \n");
            return;
        }

        sendto(sock_fd, buffer, strlen(buffer),
               MSG_CONFIRM, reinterpret_cast<sockaddr *>(&server_information),
               sizeof(server_information));
    }
} // utils
