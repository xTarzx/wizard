#pragma once

#include <string>
#include "udp.h"
#include "pilot.h"

const uint16_t WIZ_PORT = 38899;

class Wizard
{
private:
    UDPSocket m_socket;
    std::string m_bulb_ip;

public:
    bool FindBulb();
    void SetBulbIP(const char *ip);
    void SetState(bool state); // turn on off
    void SetPilot(Pilot pilot);

    void Test();
};