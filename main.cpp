#include <iostream>

#include "send/send.h"
#include "utils/udp_utils.h"

int send_wrapper(char* file_path) {
    /*
     * Manages the creation and handling of sending a file to a tftp receiver.
     * @param file_path Path to the file that is to be sent.
     */

    return 0;
}

int receive_wrapper() {

    //TODO: make buffer parameter
    utils::udp_utils::create_udp_server_socket(8000);
    return 0;
}

int main(int argc, char* argv[]) {

    std::string mode = argv[1];

    if (mode == "send") {
        char* file_path = argv[2];
        send_wrapper(file_path);
    }else if (mode == "receive") {
        receive_wrapper();
    }else {
        std::cout << "Invalid mode given, please use either \'send\' or \'receive\' mode.\n";
        return 1;
    }
    return 0;
}


