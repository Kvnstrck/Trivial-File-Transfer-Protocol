//
// Created by kecs on 24.10.25.
//

#include "Packet_Helper.h"

#include <stdexcept>
#include <string>

std::string Packet_Helper::build_packet(const utils::TFTP_MESSAGE_TYPE message_type,
                                         const TFTP_Connection_State &connection_state_state) {
    switch (message_type) {
        case utils::READ_REQUEST:
            return build_rreq_packet(connection_state_state);
        case utils::WRITE_REQUEST:
            return build_wreq_packet(connection_state_state);
        case utils::ACKNOWLEDGMENT:
            return build_ack_packet(connection_state_state);
        case utils::DATA:
            return build_data_packet(connection_state_state);
        case utils::ERROR:
            return build_error_packet(connection_state_state);
        default:
            return "";
    }
}

std::string Packet_Helper::build_rreq_packet(const TFTP_Connection_State &connection_state) {
    //build RREQ packet as per definition in the RFC
    /*2 bytes     string    1 byte     string   1 byte
    *       ------------------------------------------------
    *      | Opcode |  Filename  |   0  |    Mode    |   0  |
    *       ------------------------------------------------
    */

    std::string packet;

    //append Opcode
    packet.append("1");
    //append Filename
    packet.append(connection_state.get_file_name());
    //append string seperator
    packet.append("0");
    //append transmission mode
    packet.append(connection_state.get_transmission_mode());
    //append string seperator
    packet.append("0");

    return packet;
}

std::string Packet_Helper::build_wreq_packet(const TFTP_Connection_State &connection_state) {
    //build WREQ packet as per definition in the RFC
    /*2 bytes     string    1 byte     string   1 byte
    *       ------------------------------------------------
    *      | Opcode |  Filename  |   0  |    Mode    |   0  |
    *       ------------------------------------------------
    */

    std::string packet;

    //append Opcode
    packet.append("2");
    //append Filename
    packet.append(connection_state.get_file_name());
    //append string seperator
    packet.append("0");
    //append transmission mode
    packet.append(connection_state.get_transmission_mode());
    //append string seperator
    packet.append("0");

    return packet;
}

std::string Packet_Helper::build_data_packet(const TFTP_Connection_State &connection_state) {
    //build DATA packet as per definition in the RFC
    std::string packet;

    //append Opcode
    packet.append("3");
    //append block number
    packet.append(std::to_string(connection_state.get_block_number()));
    //append data
    packet.append(connection_state.get_block_data());

    return packet;
}

std::string Packet_Helper::build_ack_packet(const TFTP_Connection_State &connection_state) {
    //build ACK packet as per definition in the RFC
    std::string packet;

    //append Opcode
    packet.append("4");
    //append block number
    packet.append(std::to_string(connection_state.get_block_number()));

    return packet;
}

std::string Packet_Helper::build_error_packet(const TFTP_Connection_State &connection_state) {
    //build ERROR packet as per definition in the RFC
    std::string packet;

    //append Opcode
    packet.append("5");
    //append error code
    packet.append("TBD");

    return packet;
}

/**
 * Parses the given packet string into a TFTP_Connection_State object.
 * @param packet The string representation of the packet to parse.
 * @return a TFTP_Connection_State object representing the parsed packet.
 * @throws std::runtime_error when the first character of the parsed packet is not a valid opcode.
 */
TFTP_Connection_State Packet_Parser::parse_packet(const std::string &packet) {
    switch (packet[0]) {
        case '1':
            return parse_rreq_packet(packet);
        case '2':
            return parse_wreq_packet(packet);
        case '3':
            return parse_ack_packet(packet);
        case '4':
            return parse_data_packet(packet);
        case '5':
            return parse_error_packet(packet);
        default:
            throw std::runtime_error("Undefined opcode found while parsing packet.");
    }
}
