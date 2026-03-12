//
// Created by kecs on 15.10.25.
//

#ifndef UDP_UTILS_H
#define UDP_UTILS_H
#include <netinet/in.h>
#include <vector>

namespace utils {
    /**
     * This enum holds all parameters for UDP sockets and their utils.
     */
    enum UDP_PROTOCOL_PARAMETERS {
        TIMEOUT_SECONDS = 5,
        TIMEOUT_MICROSECONDS = 0,
        MESSAGE_BUFFER_SIZE = 1024,
        MAXIMUM_RETRANSMISSION_COUNTER = 10,
        MESSAGE_CHUNK_SIZE = 512
    };

    class UDP_Utils {
    public:
        /**
         * Creates a UDP socket at the given port.
         * @return The socket file descriptor of the created socket.
         */
        static int create_udp_socket(uint32_t port);

        /**
         * Waits for an incoming UDP message and returns the client information
         * @return The number of bytes received.
         */
        static std::vector<uint8_t> receive_udp_message(int socket_fd);

        /**
         * Send a message via UDP to the specified IPv4/port combination.
         * @param sock_fd The socket file descriptor of the UDP socket used to send the message.
         * @param buffer Buffer holding the message.
         * @param port The destination UDP port.
         * @param ip The destination IPv4 address.
         */
        static void send_udp_message(int sock_fd, const std::vector<uint8_t> &buffer, int port, const char *ip);
    };
} // utils

#endif //UDP_UTILS_H
