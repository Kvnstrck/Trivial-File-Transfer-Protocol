//
// Created by kecs on 24.10.25.
//

#ifndef PACKET_BUILDER_H
#define PACKET_BUILDER_H
#include <utility>
#include <vector>

#include "TFTP_Connection.h"

class Packet {
private:


public:
    u_int16_t opcode;

    explicit Packet (const u_int16_t opcode): opcode(opcode) {
    }

    virtual std::string toString();

    virtual std::vector<uint8_t> toByteArray();

    virtual ~Packet();
};

class RREQ_Packet : public Packet {
private:
    std::string file_name;
    std::string mode;
public:
    /**
     * Builds a new RREQ Packet.
     * @param file_name The name of the file to request from the server.
     * @param mode the transmission mode which should be used.
     */
    explicit RREQ_Packet (std::string file_name, std::string mode): Packet(1), file_name(std::move(file_name)), mode(std::move(mode)) {}

    std::vector<uint8_t> toByteArray() override;

    std::string toString() override;

    ~RREQ_Packet() override;
};

class WREQ_Packet : public Packet {
private:
    std::string file_name;
    std::string mode;
public:
    /**
     * Builds a new WREQ Packet.
     * @param file_name The name of the file to be sent to the server.
     * @param mode the transmission mode which should be used.
     */
    explicit WREQ_Packet (std::string file_name, std::string mode): Packet(2), file_name(std::move(file_name)), mode(std::move(mode)) {}

    std::vector<uint8_t> toByteArray() override;

    std::string toString() override;

    ~WREQ_Packet() override;
};

class DATA_Packet : public Packet {
private:
    uint32_t block_number;
    std::vector<uint8_t> block_data;
public:
    explicit DATA_Packet (const uint16_t block_number, std::vector<uint8_t> block_data): Packet(3), block_number(block_number), block_data(std::move(block_data)) {}

    std::vector<uint8_t> toByteArray() override;

    std::string toString() override;

    ~DATA_Packet() override;
};

class ACK_Packet : public Packet {
private:
    uint32_t block_number;

public:
    explicit ACK_Packet (const uint16_t block_number): Packet(4), block_number(block_number) {}
    
    std::vector<uint8_t> toByteArray() override;

    std::string toString() override;

    ~ACK_Packet() override;
};

class ERROR_Packet : public Packet {
private:
    uint16_t error_code;
    std::string error_message;

public:
    explicit ERROR_Packet (const u_int16_t error_code, std::string error_message): Packet(5), error_code(error_code), error_message(std::move(error_message)) {}

    std::vector<uint8_t> toByteArray() override;

    std::string toString() override;

    ~ERROR_Packet() override;
};


#endif //PACKET_BUILDER_H
