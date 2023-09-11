#include <unistd.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <iostream>
#include <cerrno>
#include <cstring>

#include "jansson.h"

#include "udp.h"

const int MAX_BUF = 4096;

void UDPSocket::cleanup()
{
#ifdef _WIN32
    closesocket(m_socket);
    WSACleanup();
#else
    close(m_socket);
#endif
}

UDPSocket::UDPSocket()
{
    InitSocket();
}
UDPSocket::~UDPSocket()
{
    if (m_socket > 0)
    {
        this->cleanup();
    }
}

bool UDPSocket::InitSocket()
{
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cerr << "ERROR: could not initialize WSA, code: " << WSAGetLastError() << std::endl;
        return false;
    }
#endif

    if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "ERROR: could not create socket, " << std::strerror(errno) << std::endl;
        return false;
    }

    int broadcast = 1;

#ifdef _WIN32
    if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast)) < 0)
#else
    if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
#endif
    {
        std::cerr << "ERROR: could not set broadcast, " << std::strerror(errno) << std::endl;
        this->cleanup();
        m_socket = -1;
        return false;
    }

    timeval recv_timeout;
    recv_timeout.tv_usec = 0;
    recv_timeout.tv_sec = 8;

#ifdef _WIN32
    DWORD timeout = recv_timeout.tv_sec * 1000;
    if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
#else
    if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout)) < 0)
#endif
    {
        std::cerr << "ERROR: could not set broadcast, " << std::strerror(errno) << std::endl;
        this->cleanup();
        m_socket = -1;
        return false;
    }

    return true;
}

std::string UDPSocket::SendAndRecv(const std::string &msg, const std::string &target_ip, uint16_t port)
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
    inet_pton(AF_INET, target_ip.c_str(), &dest_addr.sin_addr);

    // inet_aton(target_ip.c_str(), &dest_addr.sin_addr);

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
    int flags = 0;
#ifndef _WIN32
    flags = MSG_WAITALL;
#endif
    int b_recv = recvfrom(m_socket, buf, MAX_BUF, flags, (struct sockaddr *)&from_addr, &len);

    if (b_recv < 0)
    {
        std::cerr << "ERROR: response timed out" << std::endl;
        return "";
    }

    std::cout << "received " << b_recv << " bytes" << std::endl;

    buf[b_recv] = '\0';

    // append address to result

    json_error_t error;
    json_t *root = json_loads(buf, JSON_COMPACT, &error);
    char addr_buf[MAX_BUF];
    inet_ntop(from_addr.sin_family, &from_addr.sin_addr, addr_buf, MAX_BUF);
    json_object_set_new(root, "ip", json_string(addr_buf));

    std::string res = json_dumps(root, JSON_COMPACT);

    json_decref(root);

    return res;
}