//
// Created by kecs on 15.10.25.
//

#ifndef UDP_UTILS_H
#define UDP_UTILS_H
#include <netinet/in.h>

namespace utils {
    /**
     * This enum holds all parameters for UDP sockets and their utils.
     */
    enum UDP_PROTOCOL_PARAMETERS {
        TIMEOUT_SECONDS = 1000,
        TIMEOUT_MICROSECONDS = 0,
        RECEIVE_BUFFER_SIZE = 1024
    };

    class UDP_Utils {
    public:
        static int create_udp_socket(uint32_t);

        static sockaddr_in receive_udp_message(int, char *);

        static void send_udp_message(int, const char *, int, const char *);
    };
} // utils

#endif //UDP_UTILS_H
