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
    bool m_set_scene = false;
    int m_scene;

    bool m_set_temp = false;
    int m_temp;

public:
    void SetState(bool state);
    void SetRGB(int r, int g, int b);
    void SetBrightness(int brightness);
    void SetScene(int scene);
    void SetTemp(int temp);

    std::string Build();
};