#include <cxxopts.hpp>

namespace tsor {
    /**
     * Parse command line arguments with cxxopts
     */
    cxxopts::ParseResult parse_args(int argc, char* argv[]);

    /**
     * Create vector of PIDs from hex string list
     */
    std::vector<uint> parse_filter(const cxxopts::OptionValue* filter, bool verbose);

    /**
     * Add hex string to vector as integer, handling out-of-range exceptions
     */
    bool add_filter_pid(std::vector<uint>* v, std::string s);
} // namespace tsor
