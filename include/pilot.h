#pragma once

#include <string>

class Pilot
{
private:
    bool m_set_state = false;
    bool m_state;

    bool m_set_rgb = false;
    int m_r;
    int m_g;
    int m_b;

    bool m_set_brightness = false;
    int m_brightness;

public:
    void SetState(bool state);
    void SetRGB(int r, int g, int b);
    void SetBrightness(int brightness);

    std::string Build();
};