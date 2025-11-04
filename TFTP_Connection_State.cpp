//
// Created by kecs on 24.10.25.
//

#include "TFTP_Connection_State.h"

#include <utility>


std::string TFTP_Connection_State::get_file_name() const {
    return this->file_name;
}

std::string TFTP_Connection_State::get_transmission_mode() const {
    return this->transmission_mode;
}

int TFTP_Connection_State::get_block_number() const {
    return this->block_number;
}

std::string TFTP_Connection_State::get_block_data() const {
    return this->block_data;
}

void TFTP_Connection_State::set_file_name(std::string file_name) {
    this->file_name = std::move(file_name);
}

void TFTP_Connection_State::set_transmission_mode(std::string transmission_mode) {
    this->transmission_mode = std::move(transmission_mode);
}

void TFTP_Connection_State::set_block_number(int block_number) {
    this->block_number = block_number;
}

void TFTP_Connection_State::set_block_data(std::string block_data) {
    this->block_data = std::move(block_data);
}
