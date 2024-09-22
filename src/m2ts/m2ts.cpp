#include "m2ts.h"

std::map<unsigned int,const char*> M2TS_PID = {
    {0x0000, "PAT"},        // Program Association Table
    {0x0001, "CAT"},        // Conditional Access Table
    {0x0002, "TSDT"},       // Transport Stream Description Table
    {0x0003, "IPMP"},       // Intellectual Property Management and Protection
    {0x0010, "NIT"},        // DVB Network Information Table
    {0x0011, "SDT/BAT"},    // DVB Service Description Table or DVB Bouquet Association Table
    {0x0012, "EIT"},        // DVB Event Information Table
    {0x0013, "RST"},        // DVB Running Status Table
    {0x0014, "TDT/TOT"},    // DVB Time and Date Table or DVB Time Offset Table
    {0x0016, "RNT"},        // DVB Resolution Authority Record (RAR) Notification Table
    {0x001E, "DIT"},        // DVB Discontinuity Information Table
    {0x001F, "SIT"},        // DVB Selection Information Table
    {0x1FFF, "FILL"}        // Null Packet
};

Packet packet_test() {
    Packet p = { 0x47, false, false, false, 0x1FFF, 1, 1, 4, 0};
    return p;
}
