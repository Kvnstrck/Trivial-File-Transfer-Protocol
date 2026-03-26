#ifndef CONNECTION_ENDPOINT_H
#define CONNECTION_ENDPOINT_H

#include <string>

class Connection_Endpoint
{
protected:
    std::string file_name;
    std::string transmission_mode;
    u_int16_t block_number;

public:
    /**
     * Builds a new connection endpoint.
     * @param file_name The name of the file to be transferred.
     * @param transmission_mode The mode of transmission to be used.
     * @param block_number The block number for the connection.
     */
    Connection_Endpoint(std::string file_name, std::string transmission_mode,
                        const u_int16_t block_number) : file_name(std::move(file_name)),
                                                        transmission_mode(std::move(transmission_mode)),
                                                        block_number(block_number)
    {
    }
};

#endif //CONNECTION_ENDPOINT_H