#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

namespace tsor::gui
{
    extern GLFWwindow* window;
    extern ImGuiIO* io;

    const ImGuiWindowFlags parent_window_flags = 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    const ImGuiWindowFlags child_window_flags = 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse;

    /**
     * Update GUI frame buffer
     */
    void update();

    /**
     * Setup ImGui and GLFW/OpenGL
     */
    bool setup(int width, int height, bool verbose);

    /**
     * Cleanup GUI resources
     */
    void cleanup();
} // namespace tsor::gui
