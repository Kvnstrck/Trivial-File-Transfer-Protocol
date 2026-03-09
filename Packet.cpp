//
// Created by kecs on 24.10.25.
//

#include "Packet.h"

#include <stdexcept>
#include <iostream>


std::string Packet::toString() {
    return std::to_string(this->opcode);
}

std::vector<uint8_t> Packet::toByteArray() {
    std::vector<uint8_t> bytes;

    bytes.push_back(this->opcode>>8);
    bytes.push_back(this->opcode&0xFF);

    return bytes;
}

Packet::~Packet() = default;

std::vector<uint8_t> RREQ_Packet::toByteArray()
{   
    std::vector<uint8_t> bytes;
    //find length of file name and mode, +2 for the string terminators, +2 for the opcode
    int cumulative_string_length = this->file_name.length()+this->mode.length()+4;

    // copy opcode in buffer
    bytes.push_back(this->opcode>>8);
    bytes.push_back(this->opcode&0xFF);

    // copy file_name and mode in the buffer
    bytes.insert(bytes.end(), this->file_name.begin(), this->file_name.end() + 1);
    bytes.insert(bytes.end(), this->mode.begin(), this->mode.end() + 1);

    return bytes;
}

std::string RREQ_Packet::toString() {
    return "0" + std::to_string(this->opcode) + this->file_name + "0" + this->mode + "0";
}

RREQ_Packet::~RREQ_Packet() = default;

std::string WREQ_Packet::toString() {
    return "0" + std::to_string(this->opcode) + this->file_name + "0" + this->mode + "0";
}

WREQ_Packet::~WREQ_Packet() = default;

std::vector<uint8_t> WREQ_Packet::toByteArray()
{   
    std::vector<uint8_t> bytes;
    //find length of file name and mode, +2 for the string terminators, +2 for the opcode
    int cumulative_string_length = this->file_name.length()+this->mode.length()+4;

    // copy opcode in buffer
    bytes.push_back(this->opcode>>8);
    bytes.push_back(this->opcode&0xFF);

    // copy file_name and mode in the buffer
    bytes.insert(bytes.end(), this->file_name.begin(), this->file_name.end() + 1);
    bytes.insert(bytes.end(), this->mode.begin(), this->mode.end() + 1);

    return bytes;
}

std::string DATA_Packet::toString() {
    const std::string padded_block_number = (block_number < 256)
                                                ? "0" + std::to_string(block_number)
                                                : std::to_string(block_number);
    return std::to_string(this->opcode) + padded_block_number; // + this->block_data;
}

std::vector<uint8_t> DATA_Packet::toByteArray() {
    std::vector<uint8_t> bytes;

    bytes.push_back(this->opcode>>8);
    bytes.push_back(this->opcode&0xFF);
    bytes.push_back(this->block_number>>8);
    bytes.push_back(this->block_number&0xFF);

    bytes.insert(bytes.end(),this->block_data.begin(),this->block_data.end()+1);

    return bytes;
}

DATA_Packet::~DATA_Packet() = default;

std::string ACK_Packet::toString() {
    const std::string padded_block_number = (block_number < 256)
                                                ? "0" + std::to_string(block_number)
                                                : std::to_string(block_number);
    return "0" + std::to_string(this->opcode) + padded_block_number;
}

std::vector<uint8_t> ACK_Packet::toByteArray() {
    std::vector<uint8_t> bytes;
    bytes.push_back(this->opcode>>8);
    bytes.push_back(this->opcode&0xFF);
    bytes.push_back(this->block_number>>8);
    bytes.push_back(this->block_number&0xFF);

    return bytes;
}

ACK_Packet::~ACK_Packet() = default;

std::string ERROR_Packet::toString() {
    return "0" + std::to_string(this->opcode) + std::to_string(this->error_code) + this->error_message + "0";
}

ERROR_Packet::~ERROR_Packet() = default;

std::vector<uint8_t> ERROR_Packet::toByteArray() {
    int cumulative_string_length = this->error_message.length()+1; //+1 for string terminator

    std::vector<uint8_t> bytes;
    bytes.push_back(this->opcode>>8);
    bytes.push_back(this->opcode&0xFF);
    bytes.push_back(this->error_code>>8); 
    bytes.push_back(this->error_code&0xFF); 

    bytes.insert(bytes.end(), this->error_message.begin(), this->error_message.end() + 1);

    return bytes;
}
