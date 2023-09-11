#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "wizard.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int BUF_SZ = 4096;

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

        float rgb[3] = {0, 0, 1};
        int brightness = 100;

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

                ImGui::ColorPicker3("RGB", rgb, ImGuiColorEditFlags_Uint8);
                ImGui::SliderInt("Brightness", &brightness, 10, 100);

                if (ImGui::Button("set"))
                {
                        Pilot pilot;
                        pilot.SetRGB(rgb[0] * 255, rgb[1] * 255, rgb[2] * 255);
                        pilot.SetBrightness(brightness);
                        wiz.setPilot(pilot);
                }

                ImGui::End(); // Control

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
