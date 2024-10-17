#include <iostream>
#include <stdexcept>
#include <vector>

#include "options.h"
#include "version.h"

namespace tsor {
    cxxopts::ParseResult parse_args(int argc, char* argv[]) {
        try {
            cxxopts::Options options(PROJECT_NAME);
            options.add_options()
                ("i,input", "Input file", cxxopts::value<std::string>())
                ("filter", "List of PIDs to process (e.g. 11,1210,2A0,3FF) in hexadecimal", cxxopts::value<std::string>())
                ("gui", "Show graphical user interface", cxxopts::value<bool>()->default_value("false"))
                ("v,verbose", "Enable verbose logging", cxxopts::value<bool>()->default_value("false"))
                ("h,help", "Print usage")
            ;
            options.parse_positional({"input"});
            cxxopts::ParseResult args = options.parse(argc, argv);

            // Print usage info if no args supplied or "input" arg is missing
            if (argc == 1 || args.count("help") || !args.count("input")) {
                std::cout << options.positional_help("<input>").help() << std::endl;
                exit(0);
            }

            return args;
        }
        catch (const cxxopts::exceptions::exception& e) {
            std::cout << "Error parsing options: " << e.what() << std::endl;
            exit(1);
        }
    }

    std::vector<uint16_t> parse_filter(const cxxopts::OptionValue* filter, bool verbose) {
        auto list = filter->as<std::string>();
        std::vector<uint16_t> pids = {};
        size_t start = 0, end;
        std::string token;

        if (list.length() == 0) return pids;
    
        while ((end = list.find(",", start)) != std::string::npos) {
            // Search for comma delimiter
            token = list.substr(start, end - start);
            start = end + 1;

            // Convert hex string to int and add to vector
            if (!add_filter_pid(&pids, token) && verbose) std::cerr << "Filter PID \"" << token << "\" is invalid" << std::endl;
        }

        // Handle last element in list
        token = list.substr(start);
        if (!add_filter_pid(&pids, token) && verbose) std::cerr << "Filter PID \"" << token << "\" is invalid" << std::endl;

        return pids;
    }

    bool add_filter_pid(std::vector<uint16_t>* v, std::string s) {
        // Basic input validation
        if (s.length() == 0 || s.find(" ", 0) != std::string::npos) return false;

        try {
            // Convert hex string to int
            uint16_t pid = std::stoi(s, 0, 16);

            // Maximum possible PID value (13 bits)
            if (pid > 0x1FFF) throw std::out_of_range("Must be less than 0x1FFF");
            v->push_back(pid);
        }
        catch (...) { return false; }   // std::out_of_range, std::invalid_argument

        return true;
    }
} // namespace tsor
