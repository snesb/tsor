#include <iostream>

#include "options.h"

/**
 * Parse command line arguments with cxxopts
 */
cxxopts::ParseResult parse_args(int argc, char* argv[]) {
    try {
        cxxopts::Options options(argv[0]);
        options.add_options()
            ("i,input", "Input file", cxxopts::value<std::string>())
            ("v,verbose", "Enable verbose logging", cxxopts::value<bool>()->default_value("false"))
            ("h,help", "Print usage")
        ;
        options.parse_positional({"input"});
        cxxopts::ParseResult args = options.parse(argc, argv);

        // Print usage info if no args supplied or "input" arg is missing
        if (argc == 1 || args.count("help") || !args.count("input")) {
            std::cout << options.positional_help("<input>").help() << std::endl;
            exit(1);
        }

        return args;
    }
    catch (const cxxopts::exceptions::exception& e) {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}
