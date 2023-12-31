#include "pilot.h"
#include "jansson.h"

#include <iostream>

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
void Pilot::SetScene(int scene)
{
    m_set_scene = true;
    m_scene = scene;
}

void Pilot::SetTemp(int temp)
{
    m_set_temp = true;
    m_temp = temp;
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
    if (m_set_temp)
    {
        json_object_set_new(data, "temp", json_integer(m_temp));
    }

    if (m_set_scene)
    {
        if (m_scene > 0)
        {
            json_object_set_new(data, "sceneId", json_integer(m_scene));
        }
    }

    json_object_set_new(root, "params", data);
    std::string msg = json_dumps(root, JSON_COMPACT);
    json_decref(root);

    return msg;
}