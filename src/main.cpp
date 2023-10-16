#include <iostream>
#include <cstring>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "wizard.h"
#include "scenes.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int BUF_SZ = 4096;

bool live = false;

bool capture = false;

struct State
{
    float rgb[3] = {0, 0, 1};
    int brightness = 100;
    int sel_scene = 0;
    int scene_brightness = 100;
};

void set_rgbB(Wizard *wiz, State *state)
{
    Pilot pilot;
    pilot.SetRGB(state->rgb[0] * 255, state->rgb[1] * 255, state->rgb[2] * 255);
    pilot.SetBrightness(state->brightness);
    wiz->SetPilot(pilot);
}

bool voice_toggle = false;
int cd = 0;
ma_int16 avg = 0;
ma_int16 havg = 0;

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    cd -= 1;

    if (!capture)
        return;

    Wizard *wiz = (Wizard *)pDevice->pUserData;

    ma_int16 *input = (ma_int16 *)pInput;
    ma_int16 sum = 0;

    for (ma_uint32 i = 0; i < frameCount; ++i)
    {
        sum += input[i];
    }

    avg = sum / frameCount;

    if (avg < 0)
        avg = 0;

    if (avg > havg)
        havg = avg;

    if (avg > 33 && cd < 0)
    {
        cd = 50;
        voice_toggle = !voice_toggle;
        wiz->SetState(voice_toggle);
    }
}

int main(int argc, char const *argv[])
{
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_s16;
    config.capture.channels = 1;
    config.sampleRate = 16000;
    config.dataCallback = data_callback;
    config.pUserData = nullptr;

    ma_device device;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    {
        return 1;
    }

    ma_device_start(&device);

    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "WiZard", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = true;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Wizard wiz;
    const char *bip = "192.168.1.69";
    wiz.SetBulbIP(bip);
    char ip_addr[BUF_SZ] = {};
    std::strcpy(ip_addr, bip);

    device.pUserData = &wiz;

    State state;

    state.sel_scene = 12;

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Bulbs");

        if (ImGui::InputText("IP", ip_addr, BUF_SZ))
        {
            wiz.SetBulbIP(ip_addr);
        }

        if (ImGui::Button("Find"))
        {
            wiz.FindBulbs();
        }

        for (std::string &ip : wiz.m_bulb_ips)
        {
            if (ImGui::Button(ip.c_str()))
            {
                wiz.SetBulbIP(ip.c_str());
                std::strcpy(ip_addr, wiz.GetBulbIP().c_str());
            }
        }

        ImGui::End(); // Bulbs

        ImGui::Begin("Control");

        if (ImGui::Button("on"))
        {
            wiz.SetState(true);
        }
        ImGui::SameLine();
        if (ImGui::Button("off"))
        {
            wiz.SetState(false);
        }
        ImGui::SameLine();

        ImGui::Checkbox("live", &live);

        if (ImGui::ColorPicker3("RGB", state.rgb, ImGuiColorEditFlags_Uint8) && live)
        {
            set_rgbB(&wiz, &state);
        }
        if (ImGui::SliderInt("Brightness", &state.brightness, 10, 100) && live)
        {
            set_rgbB(&wiz, &state);
        }

        if (ImGui::Button("set"))
        {
            set_rgbB(&wiz, &state);
        }

        ImGui::End(); // Control

        ImGui::Begin("Scenes");

        for (int i = 1; i < SceneCount; i++)
        {
            ImGui::RadioButton(SceneStr[i], &state.sel_scene, i);
            if (i % 2 != 0)
                ImGui::SameLine();
        }

        ImGui::SliderInt("Brightness", &state.scene_brightness, 10, 100);

        if (ImGui::Button("set"))
        {
            Pilot pilot;
            pilot.SetScene(state.sel_scene);
            pilot.SetBrightness(state.scene_brightness);
            wiz.SetPilot(pilot);
        }

        ImGui::End(); // Scenes

        ImGui::Begin("Audio");

        if (ImGui::Checkbox("on", &capture))
        {
            if (capture)
            {
                wiz.SetState(voice_toggle);
            }
        };

        ImGui::Text("peak: %d", havg);
        ImGui::Text("%d", avg);

        ImGui::End();

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ma_device_uninit(&device);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
