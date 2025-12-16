//
// Created by kecs on 24.10.25.
//

#include "TFTP_Connection_State.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "Packet_Helper.h"
#include "utils/TFTP_Utils.h"
#include "utils/UDP_Utils.h"

int TFTP_Connection_State::establish_connection_client(const utils::TFTP_TRANSMISSION_TYPE transmission,
                                                       const int client_port) const {
    //create client socket on specified port
    const int sock_fd = utils::UDP_Utils::create_udp_socket(client_port);

    //send the initial connection message
    std::string response = send_connection_message(transmission, sock_fd, "127.0.0.1", 10069);

    //TODO: refactor to send first message(read/write)

    //TODO: build method for data packet sending

    return sock_fd;
}

std::string TFTP_Connection_State::send_connection_message(const utils::TFTP_TRANSMISSION_TYPE transmission_type,
                                                const int socket_fd,
                                                const std::string &receiver_ip, const uint16_t receiver_port) const {
    //build the read/write packet
    const utils::TFTP_MESSAGE_TYPE message_type = transmission_type == utils::READ_TRANSMISSION
                                                      ? utils::TFTP_MESSAGE_TYPE::READ_REQUEST
                                                      : utils::TFTP_MESSAGE_TYPE::WRITE_REQUEST;


    const std::string packet = Packet_Helper::build_packet(message_type, *this);

    char buffer[utils::UDP_PROTOCOL_PARAMETERS::RECEIVE_BUFFER_SIZE];

    for (int i = 0; i < utils::MAXIMUM_RETRANSMISSION_COUNTER; i++) {
        utils::UDP_Utils::send_udp_message(socket_fd, packet.c_str(), receiver_port, receiver_ip.c_str());
        //TODO: clean logging
        try {
            utils::UDP_Utils::receive_udp_message(socket_fd, buffer);
            auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            std::cout << "package received at: " << std::ctime(&end_time);
        } catch (std::runtime_error &timeout_error) {
            std::cout << timeout_error.what();
            auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            std::cout << "timeout occurred at: " << std::ctime(&end_time);
            continue;
        }
        break;
    }

    TFTP_Connection_State response_state = Packet_Parser::parse_packet(response);

    if (transmission_type == utils::READ_TRANSMISSION) {

        //get opcode of parsed packet

        //read transmission, response should be a data packet
        //TODO: parse the packet for
    }else {
        //write transmission, response should be an ACK packet
    }


    printf("Server ACK: %s\n", buffer);

    return std::string(buffer);
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
