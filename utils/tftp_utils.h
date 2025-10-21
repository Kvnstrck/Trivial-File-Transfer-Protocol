//
// Created by kecs on 19.10.25.
//

#ifndef TFTP_UTILS_H
#define TFTP_UTILS_H

enum TFTP_MESSAGE_TYPE {
    READ_REQUEST,
    WRITE_REQUEST,
    ACKNOWLEDGMENT,
    DATA,
    ERROR
};

#endif //TFTP_UTILS_H
