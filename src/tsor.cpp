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
    int run(int argc, char* argv[])
    {
        printf("%s v%s - %s\n", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION);
        printf("Built at %s (commit %s)\n\n", GIT_COMMIT_DATE, GIT_COMMIT_HASH);

        // Parse command line arguments
        cxxopts::ParseResult args = parse_args(argc, argv);
        std::string input = args["input"].as<std::string>();
        std::vector<uint> filter;
        const bool verbose = args["verbose"].as<bool>();

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
            if(!gui::setup(1280, 720))
                std::cout << "Failed to start GUI" << std::endl;

        std::cout << "Reading packets from \"" << input << "\" (" << std::dec << size << " bytes)" << std::endl;

        // Setup PID filtering
        if (args.count("filter")) filter = parse_filter(&args["filter"], args["verbose"].as<bool>());
        if (filter.size() != 0)
        {
            std::cout << "Filtering all PIDs except:";
            for (auto pid : filter)
                std::cout << " 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << pid;
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // Loop through packets in input file
        char buf[188];
        ts::Packet packet;
        while (!ifs.eof())
        {
            // Read bytes into Packet struct
            if(!ifs.read(buf, sizeof(buf)))
            {
                if (errno == 0 || errno == 11) { // EOF or Resource Temporarily Unavailable
                    if (verbose) std::cout << std::endl << "Reached end of file \"" << input << "\"" << std::endl;
                }
                else {
                    std::cout << input << ": " << std::strerror(errno) << " (" << std::dec << errno << ")"<< std::endl;
                }
                ifs.close();
                break;
            }

            // Check sync byte is present
            if (buf[0] != 0x47)
            {
                std::cout << "ERROR: Missing sync byte (0x47)" << std::endl;
                continue;
            }

            // Parse bytes into struct
            packet = ts::read(buf);

            // Skip null or corrupt packets
            if (packet.pid == ts::PID::FILL || packet.tei) continue;

            // Skip packets not in filter list
            if (!filter.empty())
                if (std::find(filter.begin(), filter.end(), packet.pid) == filter.end())
                    continue;

            // Print packet info
            if (verbose) std::cout << ts::info(&packet) << std::endl;

            // Parse payload by PID
            switch (packet.pid)
            {
                case ts::PID::PAT:
                case ts::PID::CAT:
                case ts::PID::NIT:
                case ts::PID::SDT:
                case ts::PID::TSDT:
                case ts::PID::IPMP:
                case ts::PID::EIT:  // or BAT
                case ts::PID::RST:
                case ts::PID::TDT:  // or TOT
                case ts::PID::NSYN:
                case ts::PID::RNT:
                case ts::PID::LLIS:
                case ts::PID::MEAS:
                case ts::PID::DIT:
                case ts::PID::SIT:
                default:
                    break;
            }

            // Update GUI
            if (gui::window != nullptr) gui::update();
        }

        // Kepe updating GUI after EOF
        if (args.count("gui"))
        {
            // Enable vertical sync (reduce CPU usage)
            glfwSwapInterval(1);

            while (!glfwWindowShouldClose(gui::window)) gui::update();
            gui::cleanup();
        }

        return 0;
    }
} // namespace tsor

int main(int argc, char* argv[])
{
    return tsor::run(argc, argv);
}
