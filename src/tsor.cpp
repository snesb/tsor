#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "gui/gui.h"
#include "ts.h"
#include "util/options.h"
#include "util/version.h"

namespace tsor
{
    /**
     * Main application loop
     */
    int run(cxxopts::ParseResult args, std::string input, std::ifstream* ifs)
    {
        const bool verbose = args["verbose"].as<bool>();
        ts::Mux mux;                // Transport Stream object
        char buffer[188];           // Input packet buffer
        std::string info;           // Verbose info string

        // Parse PID filter into vector
        if (args.count("filter"))
            mux.pid_filter = parse_filter(&args["filter"], args["verbose"].as<bool>());

        // Print PID filter info
        if (mux.pid_filter.size() != 0)
        {
            std::cout << "Filtering all PIDs except:";
            for (auto pid : mux.pid_filter)
                std::cout << " 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << pid;
            std::cout << std::endl;
        }

        // Loop through packets in input file
        std::cout << std::endl;
        while (!ifs->eof())
        {
            // Read bytes into buffer
            if(!ifs->read(buffer, sizeof(buffer)))
            {
                if (errno == 0 || errno == 11) // EOF or Resource Temporarily Unavailable
                {
                    std::cout << std::endl << input << ": Reached end of file" << std::endl;
                }
                else
                {
                    std::cout << input << ": " << std::strerror(errno) << " (" << std::dec << errno << ")"<< std::endl;
                }
                break;
            }

            // Push packet bytes into mux
            try
            {
                mux.push(buffer, verbose ? &info : nullptr);
                if (verbose) std::cout << info << std::endl;
            }
            catch (const std::runtime_error& e)
            {
                if (verbose) std::cout << e.what() << std::endl;
            }

            // Update user interface
            if (args.count("gui") && gui::window != nullptr)
                gui::update();
        }
        ifs->close();

        //TODO: Decouple file and GUI loops (std::thread?)

        // Keep GUI running after file EOF
        if (args.count("gui") && gui::window != nullptr)
        {
            // Enable vertical sync (reduce CPU usage) after EOF
            glfwSwapInterval(1);
            while (!glfwWindowShouldClose(gui::window)) gui::update();
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
    printf("Built at %s (commit %s)\n", GIT_COMMIT_DATE, GIT_COMMIT_HASH);

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
    std::ifstream ifs(input, std::ios::binary);
    ifs.unsetf(std::ios::skipws);
    if(!ifs)
    {
        std::cout << input << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Check file is not empty
    ifs.seekg(0, std::ios::end);
    std::streampos size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    if(size == 0)
    {
        std::cout << input << ": File is empty" << std::endl;
        ifs.close();
        return 1;
    }

    // Setup GUI
    if (args.count("gui"))
        if(!tsor::gui::setup(1280, 720, args["verbose"].as<bool>()))
            std::cout << "Failed to configure GUI" << std::endl;

    std::cout << "Reading packets from \"" << input << "\" (" << std::dec << size << " bytes)" << std::endl;
    return tsor::run(args, input, &ifs);
}
