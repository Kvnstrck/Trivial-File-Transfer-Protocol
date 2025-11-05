//
// Created by kecs on 24.10.25.
//

#include "TFTP_Connection_State.h"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "Packet_Builder.h"
#include "utils/TFTP_Utils.h"
#include "utils/UDP_Utils.h"

bool TFTP_Connection_State::establish_connection_client(const utils::TFTP_TRANSMISSION_TYPE transmission,
                                                        const int port) const {
    const int sock_fd = utils::UDP_Utils::create_udp_socket(port);

    const char *ip = "127.0.0.1";

    //build the read/write packet
    const utils::TFTP_MESSAGE_TYPE message_type = transmission == utils::READ_TRANSMISSION
                                                      ? utils::TFTP_MESSAGE_TYPE::READ_REQUEST
                                                      : utils::TFTP_MESSAGE_TYPE::WRITE_REQUEST;

    std::string packet = Packet_Builder::build_packet(message_type, *this);

    // send the read/write request
    //TODO: build support for argument target port

    retransmission:
    utils::UDP_Utils::send_udp_message(sock_fd, packet.c_str(), 10069, ip);

    // wait and receive the ACK packet
    char buffer[utils::UDP_PROTOCOL_PARAMETERS::RECEIVE_BUFFER_SIZE];


    try {
        utils::UDP_Utils::receive_udp_message(sock_fd, buffer);
    } catch (std::runtime_error &test) {
        std::cout << test.what();
        goto retransmission;
    }

    printf("Server ACK: %s\n", buffer);

    return true;
}

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
