#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

namespace tsor
{
    namespace gui
    {
        extern GLFWwindow* window;
        extern ImGuiIO* io;

        /**
         * Setup ImGui and GLFW/OpenGL
         */
        bool setup(int width, int height);

        /**
         * Update GUI frame buffer
         */
        void update();

        /**
         * Draw GUI controls
         */
        void draw();

        /**
         * Cleanup GUI resources
         */
        void cleanup();
    } // namespace gui
} // namespace tsor
