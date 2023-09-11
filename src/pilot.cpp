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
    if (m_set_rgb)
    {
        json_object_set_new(data, "r", json_integer(m_r));
        json_object_set_new(data, "g", json_integer(m_g));
        json_object_set_new(data, "b", json_integer(m_b));
    }
    if (m_set_brightness)
    {
        json_object_set_new(data, "dimming", json_integer(m_brightness));
    }

    json_object_set_new(root, "params", data);
    std::string msg = json_dumps(root, JSON_COMPACT);
    json_decref(root);

    return msg;
}