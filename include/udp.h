#pragma once
#include <vector>
#include <string>
#include <stdint.h>

class UDPSocket
{
public:
    UDPSocket();
    ~UDPSocket();

    bool InitSocket();

    std::string SendAndRecv(const std::string &msg, const std::string &target_ip, uint16_t port);
    bool SendAndRecvAll(const std::string &msg, const std::string &target_ip, uint16_t port, std::vector<std::string> *dst);

private:
    int m_socket;

    void cleanup();
};