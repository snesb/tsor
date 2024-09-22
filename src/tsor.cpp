#include <iostream>

#include "ts.h"
#include "util/options.h"
#include "util/version.h"


int main(int argc, char* argv[]) {
    printf("%s v%s - %s\n", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION);
    printf("Built at %s (commit %s)\n", GIT_COMMIT_DATE, GIT_COMMIT_HASH);

    // Parse command line arguments
    cxxopts::ParseResult args = parse_args(argc, argv);
    std::string input = args["input"].as<std::string>();
    bool verbose = args["verbose"].as<bool>();

    // Check input file extension
    if (input.substr(input.length() - 3) != ".ts") {
        printf("\nInput \"%s\" is not a Transport Stream file\n", input.c_str());
        return 1;
    }
    else {
        printf("\nReading packets from \"%s\"\n", input.c_str());

        ts::Packet p = ts::packet_test();
        std::cout << "PID: " << ts::PIDMap[p.pid] << std::endl;
    }

    return 0;
}
