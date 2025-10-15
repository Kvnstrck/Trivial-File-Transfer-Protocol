//
// Created by kecs on 15.10.25.
//

#ifndef UDP_UTILS_H
#define UDP_UTILS_H
#include <cstdint>

namespace utils {
    class udp_utils {
    public:
        static void create_udp_server_socket(uint32_t);

        static void create_udp_client_socket();
    };
} // utils

#endif //UDP_UTILS_H
