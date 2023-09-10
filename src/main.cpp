#include <iostream>

#include "udp.h"
// port 38899

#include "jansson.h"

const uint16_t WIZ_PORT = 38899;

int main(int argc, char const *argv[])
{

        UDPSocket s;

        json_t *root = json_object();
        // json_object_set_new(root, "method", json_string("getDevInfo"));
        json_object_set_new(root, "method", json_string("setPilot"));

        json_t *data = json_object();
        json_object_set_new(data, "dimming", json_integer(20));
        json_object_set_new(data, "r", json_integer(0));
        json_object_set_new(data, "g", json_integer(0));
        json_object_set_new(data, "b", json_integer(255));
        json_object_set_new(root, "params", data);

        std::string msg = json_dumps(root, JSON_COMPACT);
        std::string res = s.SendMessage(msg, "192.168.1.69", WIZ_PORT);
        json_decref(root);

        std::cout << res << std::endl;

        return 0;
}
