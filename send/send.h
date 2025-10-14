#include <iostream>
//
// Created by kecs on 14.10.25.
//

#ifndef SEND_H
#define SEND_H

namespace send {
    class Send {
        std::string file;
    public:
        Send(std::string);
        bool check_for_file_validity();
    };
} // send

#endif //SEND_H
