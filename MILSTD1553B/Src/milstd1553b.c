#include "milstd1553b.h"

#include <arpa/inet.h> // for htons/ntohs

static bool compute_parity(uint16_t word) {
    int ones = 0;
    for (int i = 0; i < 16; i++) if (word & (1u << i)) ones++;
    return (ones % 2 == 0); // odd parity → parity bit must be 1 if ones are even
}

__attribute__((weak))
uint16_t mil1553_encode_command(MIL1553_Command c) {
    uint16_t w = 0;
    w |= (c.rt_addr & 0x1F) << 11;
    w |= (c.tr & 0x1) << 10;
    w |= (c.subaddr & 0x1F) << 5;
    w |= (c.wc_or_mode & 0x1F);
    return w;
}

__attribute__((weak))
MIL1553_Command mil1553_decode_command(uint16_t w) {
    MIL1553_Command c;
    c.rt_addr    = (w >> 11) & 0x1F;
    c.tr         = (w >> 10) & 0x1;
    c.subaddr    = (w >> 5) & 0x1F;
    c.wc_or_mode = w & 0x1F;
    return c;
}

__attribute__((weak))
uint16_t mil1553_encode_status(MIL1553_Status s) {
    uint16_t w = 0;
    w |= (s.rt_addr & 0x1F) << 11;
    w |= (s.msg_error ? 1 : 0) << 10;
    w |= (s.service_request ? 1 : 0) << 8;
    w |= (s.busy ? 1 : 0) << 5;
    w |= (s.subsystem_flag ? 1 : 0) << 4;
    w |= (s.dynamic_bus_control ? 1 : 0) << 3;
    w |= (s.terminal_flag ? 1 : 0) << 2;
    return w;
}

__attribute__((weak))
MIL1553_Status mil1553_decode_status(uint16_t w) {
    MIL1553_Status s = {0};
    s.rt_addr         = (w >> 11) & 0x1F;
    s.msg_error       = (w >> 10) & 1;
    s.service_request = (w >> 8) & 1;
    s.busy            = (w >> 5) & 1;
    s.subsystem_flag  = (w >> 4) & 1;
    s.dynamic_bus_control = (w >> 3) & 1;
    s.terminal_flag   = (w >> 2) & 1;
    return s;
}

uint16_t mil1553_encode_data(MIL1553_Data d) {
    return d.data;
}

MIL1553_Data mil1553_decode_data(uint16_t w) {
    MIL1553_Data d;
    d.data = w;
    return d;
}

// Serialize
uint16_t mil1553_serialize(uint16_t word) {
    return htons(word);
}

// Deserialize
uint16_t mil1553_deserialize(uint16_t net_word) {
    return ntohs(net_word);
}
