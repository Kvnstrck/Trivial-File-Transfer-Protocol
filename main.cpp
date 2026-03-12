#include <iostream>
#include <unistd.h>


#include "TFTP_Connection.h"
#include "utils/TFTP_Utils.h"
#include "utils/UDP_Utils.h"
#include <filesystem>

/**
* Manages the creation and handling of sending a file to a tftp client.
* @param file_path Path to the file that is to be sent.
*/
int client_wrapper(char *argv[]) {

    auto path = std::filesystem::canonical(argv[2]);

    auto connection_state = new TFTP_Connection(path, "netascii", 0);

    int client_fd = connection_state->start_transmission_client(utils::WRITE_TRANSMISSION, 10070);

    free(connection_state);
    return 0;
}

int server_wrapper() {
    const auto connection_state = new TFTP_Connection("FILE_NOT_SET", "TRANSMISSION_MODE_NOT_SET", 0);

    int server_fd = connection_state->start_transmission_server();

    return 0;
}

int main(int argc, char *argv[]) {
    //TODO: improve parsing of cmd line arguments
    const std::string mode = argv[1];
    if (mode == "send") {
        client_wrapper(argv);
    } else if (mode == "receive") {
        server_wrapper();
    } else {
        std::cout << "Invalid mode given, please use either \'send\' or \'receive\' mode.\n";
        return 1;
    }
    return 0;
}
