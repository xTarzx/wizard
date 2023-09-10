#include <iostream>

#include "udp.h"
// port 38899

#include "jansson.h"

const uint16_t WIZ_PORT = 38899;

int main(int argc, char const *argv[])
{

        UDPSocket s;

        json_t *root = json_object();
        json_object_set_new(root, "method", json_string("getDevInfo"));

        std::string msg = json_dumps(root, JSON_COMPACT);
        std::string res = s.SendMessage(msg, "192.168.1.255", WIZ_PORT);
        json_decref(root);

        std::cout << res << std::endl;

        return 0;
}
