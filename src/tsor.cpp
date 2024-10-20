#include <fstream>
#include <iostream>

#include "gui/gui.h"
#include "ts.h"
#include "util/options.h"
#include "util/time.h"
#include "util/version.h"

namespace tsor
{
    bool verbose;                   // Verbose logging flag
    static std::ifstream file;      // Input file stream
    ts::Mux mux;                    // MPEG Transport Stream

    /**
     * Main application loop
     */
    int run(cxxopts::ParseResult& args)
    {
        auto input = args["input"].as<std::string>();
        verbose = args["verbose"].as<bool>();

        // Handle PID filter argument
        if (args.count("filter"))
        {
            mux.pid_filter = parse_pid_filter(&args["filter"], verbose);
            if (mux.pid_filter.size() != 0)
                std::cout << "Only processing PIDs " << print_pid_filter(mux.pid_filter) << std::endl;
        }
        if (verbose) std::cout << std::endl;

        // Setup GUI
        if (args.count("gui"))
        {
            if(!tsor::gui::setup(1400, 800, verbose))
                std::cout << "Failed to configure GUI" << std::endl;
            else
                // Fast initial render
                for (int i = 0; i < 4; i++)
                    gui::update(mux, false);
        }

        // Get start timestamp
        int64_t start_time = tsor::get_ms();

        ts::Packet packet;
        char buffer[TS_BUFFER_LENGTH];
        while (!file.eof() && !file.fail())
        {
            // Update GUI
            if (tsor::gui::window != nullptr)
            {
                if (glfwWindowShouldClose(gui::window)) break;
                gui::update(mux, true);
            }

            // Read bytes into buffer and handle errors
            if(!file.read(buffer, sizeof(buffer)))
            {
                // Handle error codes
                if (errno == 0)
                {
                    int64_t duration = tsor::get_ms() - start_time;
                    std::cout << std::endl << input << ": Reached end of file";
                    if (duration > 0) std::cout << " in " << duration << " ms";
                    std::cout << std::endl;
                }
                else
                {
                    std::cout << input << ": " << std::strerror(errno);
                    std::cout << " (ERRNO " << std::dec << errno << ")"<< std::endl;
                }

                break;
            }

            // Push packet bytes into mux
            try
            {
                if (mux.push(buffer, packet) && verbose)
                    std::cout << ts::packet_info(packet) << std::endl;
            }
            catch (const std::runtime_error& e)
            {
                if (verbose) std::cout << e.what() << std::endl;
            }
        }
        file.close();

        // Keep GUI running
        if (tsor::gui::window != nullptr)
        {
            while (!glfwWindowShouldClose(gui::window))
                gui::update(mux, false);
            gui::cleanup();
        }

        return 0;
    }
} // namespace tsor

/**
 * Handle command line arguments and input file stream
 */
int main(int argc, char* argv[])
{
    printf("%s v%s - %s\n", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION);
    printf("%s (commit %s)\n", GIT_COMMIT_DATE, GIT_COMMIT_HASH);

    // Parse command line arguments
    cxxopts::ParseResult args = tsor::parse_args(argc, argv);
    std::string input = args["input"].as<std::string>();
    std::cout << std::endl;

    // Check input file extension
    if (input.substr(input.length() - 3) != ".ts")
    {
        std::cout << input << ": Expecting a \".ts\" file" << std::endl;
        return 1;
    }

    // Open input file (checking if it exists)
    tsor::file.open(input, std::ios::binary);
    tsor::file.unsetf(std::ios::skipws);
    if(!tsor::file)
    {
        std::cout << input << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Check file is not empty
    tsor::file.seekg(0, std::ios::end);
    std::streampos size = tsor::file.tellg();
    tsor::file.seekg(0, std::ios::beg);
    if(size == 0)
    {
        std::cout << input << ": File is empty" << std::endl;
        tsor::file.close();
        return 1;
    }

    std::cout << input << ": Reading packets from file" << std::endl;
    return tsor::run(args);
}
