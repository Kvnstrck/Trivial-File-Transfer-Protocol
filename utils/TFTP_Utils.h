//
// Created by kecs on 19.10.25.
//

#ifndef TFTP_UTILS_H
#define TFTP_UTILS_H

namespace utils {
    enum TFTP_MESSAGE_TYPE {
        READ_REQUEST,
        WRITE_REQUEST,
        ACKNOWLEDGMENT,
        DATA,
        ERROR
    };

    //TODO: create method that converts binary to hex for payload building
}

#endif //TFTP_UTILS_H
