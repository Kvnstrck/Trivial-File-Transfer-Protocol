//
// Created by kecs on 24.10.25.
//

#include "TFTP_Connection.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "Packet.h"
#include "utils/TFTP_Utils.h"
#include "utils/UDP_Utils.h"

int TFTP_Connection::start_transmission_client(const utils::TFTP_TRANSMISSION_TYPE transmission,
                                                       const int client_port) const {
    //create client socket on specified port
    const int client_socket_fd = utils::UDP_Utils::create_udp_socket(client_port);

    //send the initial connection message
    std::string response = perform_connection_establishment_client(transmission, client_socket_fd, "127.0.0.1", 10069);

    return client_socket_fd;
}

int TFTP_Connection::start_transmission_server() const {
    //TODO: readjust buffer size for running state, NS3 experiment

    const int server_socket_fd = utils::UDP_Utils::create_udp_socket(10069);

    std::string response = perform_connection_establishment_server(server_socket_fd);

    //TODO: check that additional connections come from the same IP/Port to ensure a single connection

    return server_socket_fd;
}

std::string TFTP_Connection::perform_connection_establishment_client(const utils::TFTP_TRANSMISSION_TYPE transmission_type,
                                                           const int socket_fd,
                                                           const std::string &receiver_ip, const uint16_t receiver_port) const {
    //build the read/write packet
    const utils::TFTP_MESSAGE_TYPE message_type = (transmission_type == utils::READ_TRANSMISSION)
                                                      ? utils::TFTP_MESSAGE_TYPE::READ_REQUEST
                                                      : utils::TFTP_MESSAGE_TYPE::WRITE_REQUEST;

    Packet* request_packet;
    if (message_type==utils::READ_REQUEST) {
        request_packet = new RREQ_Packet(this->file_name,this->transmission_mode);
    }else {
        request_packet = new WREQ_Packet(this->file_name,this->transmission_mode);
    }

    const std::string packet = request_packet->toString();

    char buffer[utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE];

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

    printf("Message from Server : %s\n", buffer);

    return std::string(buffer);
}

std::string TFTP_Connection::perform_connection_establishment_server(const int socket_fd) const {

    //create buffer for UDP data to be put into
    char buffer[utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE];

    sockaddr_in client_information = utils::UDP_Utils::receive_udp_message(socket_fd, buffer);

    //force timeout
    //sleep(10);

    Packet* connection_response = new ACK_Packet(get_block_number());
    const std::string connection_response_string = connection_response->toString();

    utils::UDP_Utils::send_udp_message(socket_fd, connection_response_string.c_str(), 10070, "127.0.0.1");

    printf("Message from Client: %s\n", buffer);
    return std::string(buffer);
}

std::string TFTP_Connection::get_file_name() const {
    return this->file_name;
}

std::string TFTP_Connection::get_transmission_mode() const {
    return this->transmission_mode;
}

int TFTP_Connection::get_block_number() const {
    return this->block_number;
}

void TFTP_Connection::set_file_name(std::string file_name) {
    this->file_name = std::move(file_name);
}

void TFTP_Connection::set_transmission_mode(std::string transmission_mode) {
    this->transmission_mode = std::move(transmission_mode);
}

void TFTP_Connection::set_block_number(int block_number) {
    this->block_number = block_number;
}
