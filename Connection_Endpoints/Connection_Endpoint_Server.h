#include "Connection_Endpoint.h"

class Connection_Endpoint_Server : public Connection_Endpoint
{
    using Connection_Endpoint::Connection_Endpoint;

public:
    /**
     * Handles the connection of a new TFTP Server waiting for client connections.
     * @return The file descriptor of the UDP socket used for the connection.
     */
    [[nodiscard]] int start_transmission();

    /**
     * Waits for clients to connect and handles a connection establishment as per RFC 1350.
     * @param socket_fd The socket file descriptor of the UDP socket to use.
     * @return The message the client responded with during the connection establishment.
     */
    [[nodiscard]] std::string perform_connection_establishment_server(int socket_fd) const;

    void ack_write_transmission(int socket_fd);
};