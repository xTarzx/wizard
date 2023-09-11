#include "pilot.h"
#include "jansson.h"

void Pilot::SetState(bool state)
{
    m_set_state = true;
    m_state = state;
}
void Pilot::SetRGB(int r, int g, int b)
{
    m_set_rgb = true;
    m_r = r;
    m_g = g;
    m_b = b;
}
void Pilot::SetBrightness(int brightness)
{
    m_set_brightness = true;
    m_brightness = brightness;
}

std::string Pilot::Build()
{
    json_t *root = json_object();
    json_object_set_new(root, "method", json_string("setPilot"));
    json_t *data = json_object();

    if (m_set_state)
    {
        json_object_set_new(data, "state", json_boolean(m_state));
    }

    json_object_set_new(root, "params", data);
    std::string msg = json_dumps(root, JSON_COMPACT);
    json_decref(root);

    return msg;
}