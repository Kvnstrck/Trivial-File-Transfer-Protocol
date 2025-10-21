//
// Created by kecs on 15.10.25.
//

#ifndef UDP_UTILS_H
#define UDP_UTILS_H
#include <netinet/in.h>

namespace utils {
    class udp_utils {
    public:

        static int create_udp_socket(uint32_t);

        static sockaddr_in receive_udp_message(int, char *);

        static void send_udp_message(int, const char *, int, const char *);
    };
} // utils

#endif //UDP_UTILS_H
