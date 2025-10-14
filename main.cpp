#include <iostream>

#include "send/send.h"

int send_wrapper(char* file_path) {
    /*
     * Manages the creation and handling of sending a file to a tftp receiver.
     * @param file_path Path to the file that is to be sent.
     */


    send::Send* test = new send::Send(file_path);

    test->check_for_file_validity();


    //TODO: make check that file is actual file/actually exists
    //TODO: request udp socket

    return 0;
}

int receive_wrapper() {
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


