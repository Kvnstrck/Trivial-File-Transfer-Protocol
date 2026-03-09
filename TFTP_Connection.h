//
// Created by kecs on 24.10.25.
//

#ifndef TFTP_CONNECTION_STATE_H
#define TFTP_CONNECTION_STATE_H
#include <string>
#include <utility>

#include "utils/TFTP_Utils.h"


class TFTP_Connection {
private:
    std::string file_name;
    std::string transmission_mode;
    u_int16_t block_number;

public:
    TFTP_Connection(std::string file_name, std::string transmission_mode,
                    const u_int16_t block_number): file_name(std::move(file_name)),
                                                   transmission_mode(std::move(transmission_mode)),
                                                   block_number(block_number) {
    }


    /**
     * Handles the connection of a new TFTP Client connecting to another TFTP server.
     * @param transmission Transmission type that the TFTP connection should use.
     * @param client_port The UDP port that the TFTP application should operate on.
     * @return The file descriptor of the UDP socket used for the connection.
     */
    [[nodiscard]]

    int start_transmission_client(utils::TFTP_TRANSMISSION_TYPE transmission, int client_port);

    /**
     * Handles the connection of a new TFTP Server waiting for client connections.
     * @return The file descriptor of the UDP socket used for the connection.
     */
    [[nodiscard]] int start_transmission_server() ;

    /**
     * Performs a connection establishment as per RFC1350 for either a read-request or a write-request.
     * @param transmission_type Transmission type that the TFTP connection should use.
     * @param socket_fd The socket file descriptor of the UDP socket to use.
     * @param receiver_ip The IPv4 address of the server to be connected to.
     * @param receiver_port The UDP port number of the server to be connected to.
     * @return The message the server responded with during the connection establishment.
     */
    [[nodiscard]] std::string perform_connection_establishment_client(utils::TFTP_TRANSMISSION_TYPE transmission_type,
                                                                      int socket_fd, const std::string &receiver_ip,
                                                                      __uint16_t receiver_port) const;

    /**
     * Waits for clients to connect and handles a connection establishment as per RFC 1350.
     * @param socket_fd The socket file descriptor of the UDP socket to use.
     * @return The message the client responded with during the connection establishment.
     */
    [[nodiscard]] std::string perform_connection_establishment_server(int socket_fd) const;

    /**
     * Sends the next data packet and waits for the acknowledgement of the packet.
     * @return Whether the packet was acknowledged correctly.
     */
    void perform_write_transmission(int sender_socket_fd,
                                                  const std::string &receiver_ip, uint16_t receiver_port);

    void ack_write_transmission(int socket_fd) ;

    [[nodiscard]] std::string get_file_name() const;

    [[nodiscard]] std::string get_transmission_mode() const;

    [[nodiscard]] int get_block_number() const;

    void set_file_name(std::string file_name);

    void set_transmission_mode(std::string transmission_mode);

    void set_block_number(int block_number);
};


#endif //TFTP_CONNECTION_STATE_H
