//
// Created by kecs on 04.11.25.
//
#include "TFTP_Utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

namespace utils
{
    std::vector<uint8_t> TFTP_Utils::read_file_to_byte_array(const std::string &file_path)
    {
        std::ifstream input(file_path, std::ios::binary);

        std::vector<uint8_t> file_data(
            (std::istreambuf_iterator<char>(input)),
            (std::istreambuf_iterator<char>()));

        input.close();
        return file_data;
    }
}
