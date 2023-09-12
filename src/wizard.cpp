#include <iostream>
#include <thread>

#include "wizard.h"
#include "pilot.h"

#include "jansson.h"

// #include "threading.h"

void Wizard::SetBulbIP(const char *ip)
{
    m_bulb_ip = ip;
}

bool Wizard::FindBulb()
{
    json_t *root = json_object();
    json_object_set_new(root, "method", json_string("getDevInfo"));
    std::string msg = json_dumps(root, JSON_COMPACT);
    std::string res = m_socket.SendAndRecv(msg, "192.168.1.255", WIZ_PORT);
    json_decref(root);

    if (res.empty())
    {
        return false;
    }

    json_error_t error;
    root = json_loads(res.c_str(), JSON_COMPACT, &error);
    json_t *j_ip = json_object_get(root, "ip");
    std::string ip = json_string_value(j_ip);
    json_decref(root);

    m_bulb_ip = ip;

    return true;
}

void Wizard::Test()
{
    json_t *root = json_object();
    json_object_set_new(root, "method", json_string("getScene"));
    std::string msg = json_dumps(root, JSON_COMPACT);
    std::string res = m_socket.SendAndRecv(msg, "192.168.1.69", WIZ_PORT);
    json_decref(root);
}

void Wizard::SetState(bool state)
{
    if (m_bulb_ip.empty())
    {
        std::cout << "no bulb" << std::endl;
        return;
    }

    Pilot pilot;
    pilot.SetState(state);
    std::string msg = pilot.Build();

    std::thread t(&UDPSocket::SendAndRecv, &m_socket, msg, m_bulb_ip.c_str(), WIZ_PORT);
    t.detach();
    // m_socket.SendAndRecv(msg, m_bulb_ip.c_str(), WIZ_PORT);
}

void Wizard::SetPilot(Pilot pilot)
{
    if (m_bulb_ip.empty())
    {
        std::cout << "no bulb" << std::endl;
        return;
    }
    std::string msg = pilot.Build();

    std::thread t(&UDPSocket::SendAndRecv, &m_socket, msg, m_bulb_ip.c_str(), WIZ_PORT);
    t.detach();
}

// UDPSocket s;
// json_t *root = json_object();
// json_object_set_new(root, "method", json_string("getDevInfo"));
// json_object_set_new(root, "method", json_string("setPilot"));

// json_t *data = json_object();
// json_object_set_new(data, "dimming", json_integer(20));
// json_object_set_new(data, "r", json_integer(0));
// json_object_set_new(data, "g", json_integer(0));
// json_object_set_new(data, "b", json_integer(255));
// json_object_set_new(root, "params", data);
// std::string msg = json_dumps(root, JSON_COMPACT);
// std::string res = s.SendAndRecv(msg, "192.168.1.255", WIZ_PORT);
// json_decref(root);
// std::cout << res << std::endl;