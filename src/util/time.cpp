#include <chrono>
#include <stdint.h>

namespace tsor
{
    int64_t get_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }
}
