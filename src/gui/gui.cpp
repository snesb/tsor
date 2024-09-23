#include <iostream>
#include <vector>

#include "gui.h"
#include "../util/version.h"

namespace tsor::gui
{
    GLFWwindow* window;
    ImGuiIO* io;

    static bool verbose;
    static std::vector<float> fps(100, 0);

    static void glfw_error_callback(int error, const char* description)
    {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    }

    bool setup(int width, int height, bool verbose)
    {
        // Initialise glfw
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) return false;
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // Create window with graphics context
        window = glfwCreateWindow(width, height, WINDOW_TITLE, nullptr, nullptr);
        if (window == nullptr) return false;
        glfwWindowHintString(GLFW_X11_CLASS_NAME, PROJECT_NAME);
        glfwWindowHintString(GLFW_X11_INSTANCE_NAME, PROJECT_NAME);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0); // Disable vertical sync

        // Position window on screen
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        //glfwMaximizeWindow(window);
        glfwSetWindowMonitor(
            window, NULL,
            (mode->width/2)-(width/2),
            (mode->height/2) - (height/2),
            width, height, GLFW_DONT_CARE
        );

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO(); (void)io;
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();
        tsor::gui::verbose = verbose;

        // Setup platform/renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        return true;
    }

    void update()
    {
        // Handle window events
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            return;
        }

        // Start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main Menu Bar
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) exit(0);
			ImGui::EndMenu();
		}
        ImGui::EndMainMenuBar();

        // Main Window
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - ImGui::GetFrameHeight()), ImGuiCond_FirstUseEver);
        ImGui::Begin(PROJECT_NAME, NULL, parent_window_flags);

        // Draw UI elements
        draw();

        ImGui::End(); // Main Window

        // Render Statistics
        if (verbose)
        {
            ImGui::SetNextWindowPos(ImVec2(viewport->Size.x - 250, viewport->Size.y - 75));
            ImGui::SetNextWindowSize(ImVec2(250, 75), ImGuiCond_FirstUseEver);
            ImGui::Begin("Render Statistics", NULL, child_window_flags);
                if (fps.size() == 100) fps.erase(fps.begin());
                fps.insert(fps.end(), io->Framerate);

                ImGui::Text("%.1f FPS (%.3f ms)", io->Framerate, 1000.0f / io->Framerate);
                ImGui::PlotLines("##FPS", fps.data(), 100, 0, "", 0.0f, 60.0f, ImVec2(230, 20));
            ImGui::End(); // Render Statistics
        }

        // Render frame
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front/back frame buffers
        glfwSwapBuffers(window);
    }

    void draw()
    {
        //ImGui::ShowDemoWindow();
    }

    void cleanup()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
} // namespace tsor::gui
