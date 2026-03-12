//
// Created by kecs on 19.10.25.
//

#ifndef TFTP_UTILS_H
#define TFTP_UTILS_H
#include <string>
#include <vector>

namespace utils
{
    enum TFTP_MESSAGE_TYPE
    {
        READ_REQUEST,
        WRITE_REQUEST,
        ACKNOWLEDGMENT,
        DATA,
        ERROR
    };

    enum TFTP_TRANSMISSION_TYPE
    {
        READ_TRANSMISSION,
        WRITE_TRANSMISSION
    };

    class TFTP_Utils
    {
    public:
        static std::vector<uint8_t> read_file_to_byte_array(const std::string &file_path);
    };
    // TODO: create method that converts binary to the various transition modes for data payload building

}
#endif // TFTP_UTILS_H
