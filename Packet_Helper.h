//
// Created by kecs on 24.10.25.
//

#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H
#include "TFTP_Connection_State.h"
#include "utils/TFTP_Utils.h"

class Packet{
private:
    uint8_t opcode;
};

class RREQ_Packet extends Packet{
    
};

class Packet_Helper {
public:
    static std::string build_packet(utils::TFTP_MESSAGE_TYPE, const TFTP_Connection_State &);

private:
    static std::string build_rreq_packet(const TFTP_Connection_State &);

    static std::string build_wreq_packet(const TFTP_Connection_State &);

    static std::string build_data_packet(const TFTP_Connection_State &);

    static std::string build_ack_packet(const TFTP_Connection_State &);

    static std::string build_error_packet(const TFTP_Connection_State &);
};

class Packet_Parser {
public:
    static TFTP_Connection_State parse_packet(const std::string &);

private:
    static TFTP_Connection_State parse_rreq_packet(const std::string &);

    static TFTP_Connection_State parse_wreq_packet(const std::string &);

    static TFTP_Connection_State parse_data_packet(const std::string &);

    static TFTP_Connection_State parse_ack_packet(const std::string &);

    static TFTP_Connection_State parse_error_packet(const std::string &);
};

#endif //PACKET_BUILDER_H
