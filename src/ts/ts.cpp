#include "ts.h"

namespace ts {
    std::map<unsigned int,const char*> PIDMap = {
        {PID::PAT,  "PAT"},         // Program Association Table
        {PID::CAT,  "CAT"},         // Conditional Access Table
        {PID::TSDT, "TSDT"},        // Transport Stream Description Table
        {PID::IPMP, "IPMP"},        // Intellectual Property Management and Protection
        {PID::NIT,  "NIT"},         // DVB Network Information Table
        {PID::SDT,  "SDT/BAT"},     // DVB Service Description Table or DVB Bouquet Association Table
        {PID::EIT,  "EIT"},         // DVB Event Information Table
        {PID::RST,  "RST"},         // DVB Running Status Table
        {PID::TDT,  "TDT/TOT"},     // DVB Time and Date Table or DVB Time Offset Table
        {PID::RNT,  "RNT"},         // DVB Resolution Authority Record (RAR) Notification Table
        {PID::DIT,  "DIT"},         // DVB Discontinuity Information Table
        {PID::SIT,  "SIT"},         // DVB Selection Information Table
        {PID::FILL, "FILL"}         // Null Packet
    };
}
