//
// Created by kecs on 24.10.25.
//

#ifndef TFTP_CONNECTION_STATE_H
#define TFTP_CONNECTION_STATE_H
#include <string>
#include <utility>

#include "utils/TFTP_Utils.h"


class TFTP_Connection_State {
private:
    std::string file_name;
    std::string transmission_mode;
    int block_number;
    std::string block_data;

public:
    TFTP_Connection_State(std::string file_name, std::string transmission_mode, int block_number,
                          std::string block_data): file_name(std::move(file_name)),
                                             transmission_mode(std::move(transmission_mode)), block_number(block_number),
                                             block_data(std::move(block_data)) {
    }

    [[nodiscard]] bool establish_connection_client(utils::TFTP_TRANSMISSION_TYPE transmission, int port) const;

    [[nodiscard]] std::string get_file_name() const;

    [[nodiscard]] std::string get_transmission_mode() const;

    [[nodiscard]] int get_block_number() const;

    [[nodiscard]] std::string get_block_data() const;

    void set_file_name(std::string file_name);

    void set_transmission_mode(std::string transmission_mode);

    void set_block_number(int block_number);

    void set_block_data(std::string block_data);
};


#endif //TFTP_CONNECTION_STATE_H
