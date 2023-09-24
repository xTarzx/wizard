#pragma once

#include <string>
#include <vector>
#include <mutex>

#include "udp.h"
#include "pilot.h"

const uint16_t WIZ_PORT = 38899;

class Wizard
{
private:
    UDPSocket m_socket;

    std::mutex m_mtx;

    std::string m_bulb_ip;

public:
    std::vector<std::string> m_bulb_ips = {};
    bool FindBulb();
    bool FindBulbs();
    void SetBulbIP(const char *ip);
    std::string GetBulbIP();
    void SetState(bool state); // turn on off
    void SetPilot(Pilot pilot);

    void Test();
};