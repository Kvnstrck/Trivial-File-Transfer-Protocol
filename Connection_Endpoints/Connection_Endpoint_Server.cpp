#include "Connection_Endpoint_Server.h"
#include <iostream>
#include "../utils/UDP_Utils.h"
#include "../utils/TFTP_Utils.h"
#include "../Packet.h"

int Connection_Endpoint_Server::start_transmission()
{
    // TODO: readjust buffer size for running state, NS3 experiment

    const int server_socket_fd = utils::UDP_Utils::create_udp_socket(10069);

    std::string response = perform_connection_establishment_server(server_socket_fd);

    // TODO: check that additional connections come from the same IP/Port to ensure a single connection

    ack_write_transmission(server_socket_fd);

    return server_socket_fd;
}

std::string Connection_Endpoint_Server::perform_connection_establishment_server(int socket_fd) const
{
    std::vector<uint8_t> receive_buffer = utils::UDP_Utils::receive_udp_message(socket_fd, utils::UDP_PROTOCOL_PARAMETERS::TIMEOUT_PRE_CONNECTION_ESTABLISHMENT_MICROSECONDS);

    // TODO: check that the received message is a valid read/write request, otherwise throw error

    Packet *connection_response = new ACK_Packet(this->block_number);

    std::vector<uint8_t> response_packet = connection_response->toByteArray();

    utils::UDP_Utils::send_udp_message(socket_fd, response_packet, 10070, "127.0.0.1");

    return std::string("test");
}

void Connection_Endpoint_Server::ack_write_transmission(int socket_fd)
{
    // create buffer for UDP data to be put into

    std::vector<uint8_t> buffer(utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE);

    bool data_available = true;
    while (data_available)
    {
        std::vector<uint8_t> buffer = utils::UDP_Utils::receive_udp_message(socket_fd, utils::UDP_PROTOCOL_PARAMETERS::TIMEOUT_MICROSECONDS);

        u_int16_t opcode = static_cast<u_int16_t>(buffer.at(0)) << 8 | buffer.at(1);
        uint16_t block_number = static_cast<uint16_t>(buffer.at(2)) << 8 | buffer.at(3);

        printf("Received Opcode: %d\n", opcode);
        printf("Received Block Number: %d\n", block_number);

        if (opcode == 3)
        {
            // opcode is 3 -> received packet is data packet

            // set the number of the received block
            this->block_number = block_number;

            // build and send ACK packet for the received data packet
            Packet *connection_response = new ACK_Packet(this->block_number);
            std::vector<uint8_t> response_packet = connection_response->toByteArray();
            utils::UDP_Utils::send_udp_message(socket_fd, response_packet, 10070, "127.0.0.1");

            //check if the received data packet is the last one, if so, end the transmission
            if (buffer.size() - 4 < 512)
            {
                data_available = false;
            }
        }
        else if (opcode == 5)
        {
            // opcode is 5 -> received packet is error packet

            std::string error_message("Received error packet during write transmission! \n Received error message: " + std::string(buffer.begin() + 4, buffer.end()) + "\n");
            std::cout << error_message << std::endl;
            exit(1);
        }
        else
        {
            // received packet is invalid, throw error
            std::string error_message = "Received invalid opcode during write transmission! \n Expected block number: " + std::to_string(this->block_number + 1) + " Received opcode: " + std::to_string(opcode) + "\n";
            std::cout << error_message << std::endl;
            exit(1);
        }
    }
}
