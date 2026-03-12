//
// Created by kecs on 24.10.25.
//

#include "TFTP_Connection.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "Packet.h"
#include "utils/TFTP_Utils.h"
#include "utils/UDP_Utils.h"


int TFTP_Connection::start_transmission_client(const utils::TFTP_TRANSMISSION_TYPE transmission,
                                               const int client_port)
{
    std::string server_ip = "127.0.0.1";
    u_int16_t server_port = 10069;

    // create client socket on specified port
    const int client_socket_fd = utils::UDP_Utils::create_udp_socket(client_port);

    // send the initial connection message
    std::string response = perform_connection_establishment_client(transmission, client_socket_fd, server_ip,
                                                                   server_port);

    if (transmission == utils::READ_TRANSMISSION)
    {
        // Read Transmission handling
    }
    else
    {
        // Write Transmission handling
        this->perform_write_transmission(client_socket_fd, server_ip, server_port);
    }

    return client_socket_fd;
}

int TFTP_Connection::start_transmission_server()
{
    // TODO: readjust buffer size for running state, NS3 experiment

    const int server_socket_fd = utils::UDP_Utils::create_udp_socket(10069);

    std::string response = perform_connection_establishment_server(server_socket_fd);

    // TODO: check that additional connections come from the same IP/Port to ensure a single connection

    ack_write_transmission(server_socket_fd);

    return server_socket_fd;
}

std::string TFTP_Connection::perform_connection_establishment_client(
    const utils::TFTP_TRANSMISSION_TYPE transmission_type,
    const int socket_fd,
    const std::string &receiver_ip, const uint16_t receiver_port) const
{
    // build the read/write packet
    const utils::TFTP_MESSAGE_TYPE message_type = (transmission_type == utils::READ_TRANSMISSION)
                                                      ? utils::TFTP_MESSAGE_TYPE::READ_REQUEST
                                                      : utils::TFTP_MESSAGE_TYPE::WRITE_REQUEST;

    Packet *request_packet;
    if (message_type == utils::READ_REQUEST)
    {
        request_packet = new RREQ_Packet(this->file_name, this->transmission_mode);
    }
    else
    {
        request_packet = new WREQ_Packet(this->file_name, this->transmission_mode);
    }

    const std::vector<uint8_t> packet = request_packet->toByteArray();

    

    char receive_buffer[utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE];

    for (int i = 0; i < utils::MAXIMUM_RETRANSMISSION_COUNTER; i++)
    {
        utils::UDP_Utils::send_udp_message(socket_fd, packet, receiver_port, receiver_ip.c_str());

        // TODO: clean logging
        std::vector<uint8_t> receive_buffer;
        try
        {
            receive_buffer = utils::UDP_Utils::receive_udp_message(socket_fd);
            auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            std::cout << "package received at: " << std::ctime(&end_time);
        }
        catch (std::runtime_error &timeout_error)
        {
            std::cout << timeout_error.what();
            auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            std::cout << "timeout occurred at: " << std::ctime(&end_time);
            continue;
        }
        break;
    }

    return std::string(receive_buffer);
}

std::string TFTP_Connection::perform_connection_establishment_server(const int socket_fd) const
{
    std::vector<uint8_t> receive_buffer = utils::UDP_Utils::receive_udp_message(socket_fd);

    //TODO: check that the received message is a valid read/write request, otherwise throw error

    // force timeout
    // sleep(10);

    Packet *connection_response = new ACK_Packet(get_block_number());

    std::vector<uint8_t> response_packet = connection_response->toByteArray();

    utils::UDP_Utils::send_udp_message(socket_fd, response_packet, 10070, "127.0.0.1");

    return std::string("test");
}

void TFTP_Connection::perform_write_transmission(const int sender_socket_fd,
                                                 const std::string &receiver_ip, const uint16_t receiver_port)
{
    
    std::vector<uint8_t> data = utils::TFTP_Utils::read_file_to_byte_array(this->file_name);

    char receive_buffer[utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE];
    
    bool data_available = true;
    auto chunk_iterator = data.begin();
    while (data_available)
    {

        //get next data chunk
        auto chunk_end = std::min(chunk_iterator + utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_CHUNK_SIZE-1, data.end());
        std::vector<uint8_t> chunk(chunk_iterator, chunk_end); 
        chunk_iterator = chunk_end;
        
        // build the packet for transmission
        u_int16_t block_number = get_block_number() + 1;
        Packet *data_packet = new DATA_Packet(block_number, chunk);
        this->set_block_number(block_number);

        //TODO: remove retransmission, the standard says to send an error packet after timeout
        for (int i = 0; i < utils::MAXIMUM_RETRANSMISSION_COUNTER; i++)
        {
            std::vector<uint8_t> packet = data_packet->toByteArray();

            utils::UDP_Utils::send_udp_message(sender_socket_fd, packet, receiver_port,
                                               receiver_ip.c_str());
            // TODO: clean logging
            try
            {
                utils::UDP_Utils::receive_udp_message(sender_socket_fd);
                auto end = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(end);
                std::cout << "ACK package received at: " << std::ctime(&end_time);
            }
            catch (std::runtime_error &timeout_error)
            {
                std::cout << timeout_error.what();
                auto end = std::chrono::system_clock::now();
                std::time_t end_time = std::chrono::system_clock::to_time_t(end);
                std::cout << "timeout occurred at: " << std::ctime(&end_time);
                continue;
            }
            break;
        }

        printf("ACK Message from Server : %s\n", receive_buffer);

        // check if theres more data to be sent, if not end the transmission
        if(chunk_iterator== data.end())
        {
            data_available = false;
        }
    }
}

void TFTP_Connection::ack_write_transmission(int socket_fd)
{
    // create buffer for UDP data to be put into

    std::vector<uint8_t> buffer(utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE);

    bool data_available = true;
    while (data_available)
    {
        std::vector<uint8_t> buffer = utils::UDP_Utils::receive_udp_message(socket_fd);

        u_int16_t opcode = static_cast<u_int16_t>(buffer.at(0))<<8 | buffer.at(1);
        uint16_t block_number = static_cast<uint16_t>(buffer.at(2))<<8 | buffer.at(3);

        printf("Received Opcode: %d\n", opcode);
        printf("Received Block Number: %d\n", block_number);

        if (opcode == 3)
        {
            // opcode is 3 -> received packet is data packet

            // set the number of the received block
            set_block_number(block_number);

            Packet *connection_response = new ACK_Packet(get_block_number());

            std::vector<uint8_t> response_packet = connection_response->toByteArray();

            utils::UDP_Utils::send_udp_message(socket_fd, response_packet, 10070, "127.0.0.1");

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
            exit(block_number);
        }
        else
        {    
            // received packet is invalid, throw error
            std::string error_message = "Received invalid opcode during write transmission! \n Expected block number: " + std::to_string(get_block_number()+1) + " Received opcode: " + std::to_string(opcode)+"\n";
            throw std::runtime_error(error_message);
        }
    }
}

std::string TFTP_Connection::get_file_name() const
{
    return this->file_name;
}

std::string TFTP_Connection::get_transmission_mode() const
{
    return this->transmission_mode;
}

int TFTP_Connection::get_block_number() const
{
    return this->block_number;
}

void TFTP_Connection::set_file_name(std::string file_name)
{
    this->file_name = std::move(file_name);
}

void TFTP_Connection::set_transmission_mode(std::string transmission_mode)
{
    this->transmission_mode = std::move(transmission_mode);
}

void TFTP_Connection::set_block_number(const int block_number)
{
    this->block_number = block_number;
}
