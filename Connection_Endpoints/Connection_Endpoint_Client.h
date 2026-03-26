#include "Connection_Endpoint.h"
#include "../utils/TFTP_Utils.h"

class Connection_Endpoint_Client : public Connection_Endpoint
{
public:
    using Connection_Endpoint::Connection_Endpoint;

    /**
     * Handles the connection of a new TFTP Client connecting to another TFTP server.
     * @param transmission Transmission type that the TFTP connection should use.
     * @param client_port The UDP port that the TFTP application should operate on.
     * @return The file descriptor of the UDP socket used for the connection.
     */
    [[nodiscard]] int start_transmission(utils::TFTP_TRANSMISSION_TYPE transmission, int client_port);

    /**
     * Performs a connection establishment as per RFC1350 for either a read-request or a write-request.
     * @param transmission_type Transmission type that the TFTP connection should use.
     * @param socket_fd The socket file descriptor of the UDP socket to use.
     * @param receiver_ip The IPv4 address of the server to be connected to.
     * @param receiver_port The UDP port number of the server to be connected to.
     * @return The message the server responded with during the connection establishment.
     */
    void perform_connection_establishment(const utils::TFTP_TRANSMISSION_TYPE transmission_type,
                                                               const int socket_fd, const std::string &receiver_ip,
                                                               const __uint16_t receiver_port) const;

    /**
     * Sends the next data packet and waits for the acknowledgement of the packet.
     * @param socket_fd The socket file descriptor of the UDP socket to use for sending the data.
     * @param receiver_ip The IPv4 address of the server to be connected to.
     * @param receiver_port The UDP port number of the server to be connected to.
     * @return Whether the packet was acknowledged correctly.
     */
    void perform_write_transmission(int socket_fd, const std::string &receiver_ip, uint16_t receiver_port);
};