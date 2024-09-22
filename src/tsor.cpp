#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>

#include "ts.h"
#include "util/options.h"
#include "util/version.h"


int main(int argc, char* argv[]) {
    printf("%s v%s - %s\n", PROJECT_NAME, PROJECT_VERSION, PROJECT_DESCRIPTION);
    printf("Built at %s (commit %s)\n\n", GIT_COMMIT_DATE, GIT_COMMIT_HASH);

    // Parse command line arguments
    cxxopts::ParseResult args = parse_args(argc, argv);
    std::string input = args["input"].as<std::string>();
    bool verbose = args["verbose"].as<bool>();

    // Check input file extension
    if (input.substr(input.length() - 3) != ".ts") {
        std::cout << input << ": Expecting a \".ts\" file" << std::endl;
        return 1;
    }

    // Open input file (checking if it exists)
    std::ifstream ifs(input, std::ios::binary);
    ifs.unsetf(std::ios::skipws);
    if(!ifs) {
        std::cout << input << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    // Check file is not empty
    ifs.seekg(0, std::ios::end);
    std::streampos size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    if(size == 0) {
        std::cout << input << ": File is empty" << std::endl;
        ifs.close();
        return 1;
    }

    std::cout << "Reading packets from \"" << input << "\" (";
    std::cout << std::setprecision(4) << ((float)size / 1048576) << " MB)\n" << std::endl;

    // Read bytes from file
    char buf[188];
    while (!ifs.eof()) {
        if(!ifs.read((char*)&buf, sizeof(buf))) {
            if (errno == 0) {
                std::cout << input << ": Reached end of file" << std::endl;
                break;
            }
            else {
                std::cout << input << ": " << std::strerror(errno) << std::endl;
                ifs.close();
                return 1;
            }
        }

        printf("0x%X\n", buf[1]);
    }

    // Cleanup
    ifs.close();

    return 0;
}
