#include <iomanip>

#include "ts.h"

namespace ts {
    std::string info(Packet* p) {
        std::stringstream s;

        // Packet Identifier
        s << "PID: 0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << p->pid;
        if (PIDMap.find(p->pid) != PIDMap.end()) s << " (" << PIDMap[p->pid] << ")";

        return s.str();
    }
}
