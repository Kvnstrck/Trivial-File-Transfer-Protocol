#include "Connection_Endpoint_Client.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include "../Packet.h"
#include "../utils/TFTP_Utils.h"
#include "../utils/UDP_Utils.h"

int Connection_Endpoint_Client::start_transmission(const utils::TFTP_TRANSMISSION_TYPE transmission,
                                                   const int client_port)
{
    std::string server_ip = "127.0.0.1";
    u_int16_t server_port = 10069;

    // create client socket on specified port
    const int client_socket_fd = utils::UDP_Utils::create_udp_socket(client_port);

    // send the initial connection message
    perform_connection_establishment(transmission, client_socket_fd, server_ip, server_port);

    // check the type of transmission and perform the corresponding handling
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

void Connection_Endpoint_Client::perform_connection_establishment(
    const utils::TFTP_TRANSMISSION_TYPE transmission_type, 
    const int socket_fd, 
    const std::string &receiver_ip, 
    const __uint16_t receiver_port) const
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

    // send the read/write request to the server
    utils::UDP_Utils::send_udp_message(socket_fd, packet, receiver_port, receiver_ip.c_str());
    std::vector<uint8_t> receive_buffer(utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE);

    try
    {
        // TODO: validate that the received message is actually an acknowledgement for the sent packet(ACK for block 0 in case of WREQ), otherwise throw error and terminate the connection
        receive_buffer = utils::UDP_Utils::receive_udp_message(socket_fd, utils::TIMEOUT_MICROSECONDS);
    }
    catch (std::runtime_error &timeout_error)
    {
        // send error packet to server after timeout and terminate the connection
        std::cout << timeout_error.what();
        std::vector<uint8_t> error_message = ERROR_Packet(0, "Connection establishment timed out!").toByteArray();
        utils::UDP_Utils::send_udp_message(socket_fd, error_message, receiver_port, receiver_ip.c_str());
        exit(1);
    }

    return;

}

void Connection_Endpoint_Client::perform_write_transmission(const int socket_fd,
                                                            const std::string &receiver_ip, const uint16_t receiver_port)
{

    std::vector<uint8_t> data = utils::TFTP_Utils::read_file_to_byte_array(this->file_name);

    char receive_buffer[utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_BUFFER_SIZE];

    // while data is being available to send, send the next chunk of data and wait for the acknowledgement of the sent chunk
    bool data_available = true;
    auto chunk_iterator = data.begin();
    while (data_available)
    {

        // get next data chunk
        auto chunk_end = std::min(chunk_iterator + utils::UDP_PROTOCOL_PARAMETERS::MESSAGE_CHUNK_SIZE - 1, data.end());
        std::vector<uint8_t> chunk(chunk_iterator, chunk_end);
        chunk_iterator = chunk_end;

        // build the packet for transmission
        u_int16_t block_number_value = this->block_number + 1;
        Packet *data_packet = new DATA_Packet(block_number_value, chunk);
        this->block_number = block_number_value;

        std::vector<uint8_t> packet = data_packet->toByteArray();

        // send the packet and wait for the acknowledgement of the sent packet, if no acknowledgement is received within the timeout period, otherwise throw an error and terminate the connection
        utils::UDP_Utils::send_udp_message(socket_fd, packet, receiver_port, receiver_ip.c_str());
        try
        {
            // TODO: validate that the received message is actually an acknowledgement for the sent packet, otherwise throw error and terminate the connection
            utils::UDP_Utils::receive_udp_message(socket_fd, utils::UDP_PROTOCOL_PARAMETERS::TIMEOUT_MICROSECONDS);
        }
        catch (std::runtime_error &timeout_error)
        {
            // send error packet to server after timeout and terminate the connection
            std::cout << timeout_error.what();
            std::vector<uint8_t> error_message = ERROR_Packet(0, "Connection establishment timed out!").toByteArray();
            utils::UDP_Utils::send_udp_message(socket_fd, error_message, receiver_port, receiver_ip.c_str());
            exit(1);
        }

        // check if theres more data to be sent, if not end the transmission
        if (chunk_iterator == data.end())
        {
            data_available = false;
        }
    }
}