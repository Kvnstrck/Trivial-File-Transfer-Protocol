//
// Created by kecs on 24.10.25.
//

#include "Packet.h"

#include <stdexcept>

std::string Packet::toString() {
    return std::to_string(this->opcode);
}

Packet::~Packet() = default;

std::string RREQ_Packet::toString() {
    return "0" + std::to_string(this->opcode) + this->file_name + "0" + this->mode + "0";
}

RREQ_Packet::~RREQ_Packet() = default;

std::string WREQ_Packet::toString() {
    return "0" + std::to_string(this->opcode) + this->file_name + "0" + this->mode + "0";
}

WREQ_Packet::~WREQ_Packet() = default;

std::string DATA_Packet::toString() {
    const std::string padded_block_number = (block_number <= 256)
                                                ? "0" + std::to_string(block_number)
                                                : std::to_string(block_number);
    return "0" + std::to_string(this->opcode) + std::to_string(block_number) + padded_block_number;
}

DATA_Packet::~DATA_Packet() = default;

std::string ACK_Packet::toString() {
    const std::string padded_block_number = (block_number < 256)
                                                ? "0" + std::to_string(block_number)
                                                : std::to_string(block_number);
    return "0" + std::to_string(this->opcode) + padded_block_number;
}

ACK_Packet::~ACK_Packet() = default;

std::string ERROR_Packet::toString() {
    return "0" + std::to_string(this->opcode) + std::to_string(this->error_code) + this->error_message + "0";
}

ERROR_Packet::~ERROR_Packet() = default;
