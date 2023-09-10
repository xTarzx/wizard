#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <cerrno>
#include <cstring>

#include "jansson.h"

#include "udp.h"

const int MAX_BUF = 4096;

UDPSocket::UDPSocket()
{
    InitSocket();
}
UDPSocket::~UDPSocket()
{
    if (m_socket > 0)
    {
        close(m_socket);
    }
}

bool UDPSocket::InitSocket()
{
    if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "ERROR: could not create socket, " << std::strerror(errno) << std::endl;
        return false;
    }

    int broadcast = 1;

    if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        std::cerr << "ERROR: could not set broadcast, " << std::strerror(errno) << std::endl;
        close(m_socket);
        m_socket = -1;
        return false;
    }

    timeval recv_timeout;
    recv_timeout.tv_usec = 0;
    recv_timeout.tv_sec = 8;
    if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout)) < 0)
    {
        std::cerr << "ERROR: could not set broadcast, " << std::strerror(errno) << std::endl;
        close(m_socket);
        m_socket = -1;
        return false;
    }

    return true;
}

std::string UDPSocket::SendMessage(const std::string &msg, const std::string &target_ip, uint16_t port)
{
    if (m_socket < 0)
    {
        std::cerr << "ERROR: socket is not initialized" << std::endl;
        return "";
    }

    int msg_len = strlen(msg.c_str());

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_aton(target_ip.c_str(), &dest_addr.sin_addr);

    int b_sent = sendto(m_socket, msg.c_str(), msg_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (b_sent < 0)
    {
        std::cerr << "ERROR: could not send, " << std::strerror(errno) << std::endl;
        return "";
    }

    struct sockaddr_in from_addr;
    memset(&from_addr, 0, sizeof(from_addr));
    socklen_t len = sizeof(from_addr);

    char buf[MAX_BUF];
    int b_recv = recvfrom(m_socket, buf, MAX_BUF, MSG_WAITALL, (struct sockaddr *)&from_addr, &len);

    if (b_recv < 0)
    {
        std::cerr << "ERROR: response timed out" << std::endl;
        return "";
    }

    std::cout << "received " << b_recv << " bytes" << std::endl;

    buf[b_recv] = '\0';

    return buf;
}