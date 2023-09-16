#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "wizard.h"
#include "scenes.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int BUF_SZ = 4096;

bool live = false;

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

int main(int argc, char const *argv[])
{

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
    wiz.SetBulbIP("192.168.1.69");

    char ip_addr[BUF_SZ] = "192.168.1.69";

    State state;

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

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
