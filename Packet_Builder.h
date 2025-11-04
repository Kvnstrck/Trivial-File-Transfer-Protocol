//
// Created by kecs on 24.10.25.
//

#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H
#include "TFTP_Connection_State.h"
#include "utils/TFTP_Utils.h"


class Packet_Builder {
public:
    static std::string build_packet(utils::TFTP_MESSAGE_TYPE, const TFTP_Connection_State &);

private:
    static std::string build_rreq_packet(const TFTP_Connection_State &);

    static std::string build_wreq_packet(const TFTP_Connection_State &);

    static std::string build_data_packet(const TFTP_Connection_State &);

    static std::string build_ack_packet(const TFTP_Connection_State &);

    static std::string build_error_packet(const TFTP_Connection_State &);
};


#endif //PACKET_BUILDER_H
