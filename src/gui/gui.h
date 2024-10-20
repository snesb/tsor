#pragma once

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ts.h"

namespace tsor::gui
{
    extern GLFWwindow* window;
    extern ImGuiIO* io;

    /**
     * Update GUI frame buffer
     */
    void update(ts::Mux& mux, bool bail);

    /**
     * Setup ImGui and GLFW/OpenGL
     */
    bool setup(int width, int height, bool verbose);

    /**
     * Cleanup GUI resources
     */
    void cleanup();
} // namespace tsor::gui
