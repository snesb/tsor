#include <cxxopts.hpp>

namespace tsor {
    /**
     * Parse command line arguments with cxxopts
     */
    cxxopts::ParseResult parse_args(int argc, char* argv[]);

    /**
     * Create vector of PIDs from hex string list
     */
    std::vector<uint16_t> parse_pid_filter(const cxxopts::OptionValue* filter, bool verbose);

    /**
     * Add hex string to vector as integer, handling out-of-range exceptions
     */
    bool add_pid_filter(std::vector<uint16_t>* v, std::string s);

    /**
     * Stringify PID filter list
     */
    std::string print_pid_filter(const std::vector<uint16_t>& pids);
} // namespace tsor
