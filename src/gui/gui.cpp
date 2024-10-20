#include <iostream>
#include <vector>

#include "font/Inter.h"
#include "gui.h"
#include "ts.h"
#include "../util/version.h"

namespace tsor::gui
{
    GLFWwindow* window;
    ImGuiIO* io;

    static bool verbose;
    static std::vector<float> fps(100, 0);
    float frame_timer = 0.0f;

    static const ImGuiWindowFlags parent_window_flags = 
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    static const ImGuiWindowFlags child_window_flags = 
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse;

    static const ImGuiTableFlags table_flags =
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_NoSavedSettings |
        ImGuiTableFlags_Resizable;
    
    static const ImGuiTreeNodeFlags tree_node_flags =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    /**
     * Draw UI elements
     */
    static void draw(const ImGuiViewport* viewport, ts::Mux& mux)
    {
        // Program List
        {
            ImGui::BeginChild("Programs",
                ImVec2(
                    ImGui::GetContentRegionAvail().x * 0.2f,
                    ImGui::GetContentRegionAvail().y * 0.5f
                ),
                ImGuiChildFlags_Borders, child_window_flags
            );

            for (int i = 0; i < 6; i++)
            {
                ImGui::TreeNodeEx((void*)(intptr_t)i, tree_node_flags, "Program %i", i);
            }

            ImGui::EndChild();
        }

        // Packet Identifier Table
        {
            ImGui::BeginTable("PID Table", 2, table_flags,
                ImVec2(
                    ImGui::GetContentRegionAvail().x * 0.2f,
                    ImGui::GetContentRegionAvail().y
                )
            );
            ImGui::TableSetupColumn("PID");
            ImGui::TableSetupColumn("Count");
            ImGui::TableHeadersRow();

            for (auto pid : mux.pid_count)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                // Change text colour of PIDs not in filter
                if (!mux.is_filtered(pid.first))
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 128, 128, 255));

                // Show table names for common PIDs
                if (ts::PIDMap.find(pid.first) != ts::PIDMap.end())
                    ImGui::Text("0x%04X (%s)", pid.first, ts::PIDMap[pid.first]);
                else
                    ImGui::Text("0x%04X", pid.first);

                // Packets for this PID
                ImGui::TableNextColumn();
                ImGui::Text("%ld", pid.second);

                if (!mux.is_filtered(pid.first)) ImGui::PopStyleColor();
            }

            ImGui::EndTable();
        }

        ImGui::ShowDemoWindow();
    }

    void update(ts::Mux& mux, bool bail)
    {
        // Handle window events
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            return;
        }

        // Early out if not ready for update
        //FIXME: I know this is bad, I'm not dealing with threads yet
        if (glfwGetTime() - frame_timer < 1.0f && bail)
            return;
        else
            frame_timer = glfwGetTime();

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
        draw(viewport, mux);

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
        glfwSwapInterval(1); // Enable vertical sync

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
        io->Fonts->AddFontFromMemoryCompressedTTF((void*)InterRegular_compressed_data, 220332, 15);
        //io->Fonts->AddFontFromMemoryCompressedTTF((void*)InterBold_compressed_data, 226425, 15);
        tsor::gui::verbose = verbose;

        // Setup platform/renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        return true;
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
