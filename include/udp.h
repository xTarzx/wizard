#pragma once

#include <stdint.h>

class UDPSocket
{
public:
    UDPSocket();
    ~UDPSocket();

    bool InitSocket();

    std::string SendMessage(const std::string &msg, const std::string &target_ip, uint16_t port);

private:
    int m_socket;
};