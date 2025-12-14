#include <iostream>
#include <unistd.h>


#include "TFTP_Connection_State.h"
#include "utils/TFTP_Utils.h"
#include "utils/UDP_Utils.h"

int send_wrapper(char *argv[]) {
    /*
     * Manages the creation and handling of sending a file to a tftp receiver.
     * @param file_path Path to the file that is to be sent.
     */

    //TODO: build wrapper for connection establishment

    const auto message = "TEST MESSAGE";

    const auto connection_state = new TFTP_Connection_State("file", "netascii", 0, message);

    connection_state->establish_connection_client(utils::READ_TRANSMISSION, 10070);

    free(connection_state);
    return 0;
}

int receive_wrapper() {
    //TODO: readjust buffer size for running state

    //create buffer for UDP data to be put into
    char buffer[utils::UDP_PROTOCOL_PARAMETERS::RECEIVE_BUFFER_SIZE];

    int server_socket_fd = utils::UDP_Utils::create_udp_socket(10069);

    sockaddr_in client_information = utils::UDP_Utils::receive_udp_message(server_socket_fd, buffer);

    sleep(10);

    utils::UDP_Utils::send_udp_message(server_socket_fd, "ACK", 10070, "127.0.0.1");

    printf("Client : %s\n", buffer);

    return 0;
}

int main(int argc, char *argv[]) {
    if (std::string mode = argv[1]; mode == "send") {
        send_wrapper(argv);
    } else if (mode == "receive") {
        receive_wrapper();
    } else {
        std::cout << "Invalid mode given, please use either \'send\' or \'receive\' mode.\n";
        return 1;
    }
    return 0;
}
