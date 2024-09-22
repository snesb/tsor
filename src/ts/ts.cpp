#include <iomanip>
#include <cstring>

#include "ts.h"

namespace ts {
    Packet read(char* bytes) {
        Packet packet {
            bytes[0],                                                           // Synchronisation Byte (0x47, "G")
            static_cast<bool>((bytes[1] & 0x80) >> 7),                          // Transport Error Indicator Flag
            static_cast<bool>((bytes[1] & 0x40) >> 6),                          // Payload Unit Start Indicator Flag
            static_cast<bool>((bytes[1] & 0x20) >> 5),                          // Transport Priority Flag
            static_cast<unsigned int>(((bytes[1] << 8) | bytes[2]) & 0x1FFF),   // Packet Identifier
            static_cast<unsigned int>((bytes[3] & 0xC0) >> 6),                  // Transport Scrambling Control
            static_cast<unsigned int>((bytes[3] & 0x30) >> 4),                  // Adaptation Field Control
            static_cast<unsigned int>((bytes[3] & 0x0F)),                       // Continuity Counter
            ""
        };

        // Copy payload into struct memory
        std::memcpy(packet.data, &bytes[4], 184);

        return packet;
    }

    std::string info(Packet* p) {
        std::stringstream s;

        // Continuity Counter
        s << std::uppercase << std::hex << p->cont << " ";

        // Packet Identifier
        s << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << p->pid << " ";
        if (PIDMap.find(p->pid) != PIDMap.end())
            s << std::setfill(' ') << std::setw(8) << std::left << PIDMap[p->pid];
        else
            s << std::setfill(' ') << std::setw(8) << " ";

        // Packet Flags
        if (p->tei || p->pusi || p->pri) {
            s << (p->tei ? "TEI " : "");
            s << (p->pusi ? "PUSI " : "" );
            s << (p->pri ? "PRI" : "");
        }

        // Transport Scrambling Control
        if (p->tsc == 2 || p->tsc == 3) s << "TSC ";

        // Adaptation Field Control
        if (p->afc == 2 || p->afc == 3) s << "AFC ";

        return s.str();
    }
}
