#include "arinc429.h"

static bool compute_parity(uint32_t word31) {
    int ones = 0;
    for (int i = 0; i < 31; i++) {
        if (word31 & (1u << i)) ones++;
    }
    // odd parity: total ones including parity bit must be odd
    return (ones % 2 == 0); // if even, parity bit must be 1
}

uint32_t arinc429_build(uint8_t label, uint8_t sdi, uint32_t data, uint8_t ssm) {
    uint32_t word = 0;
    word |= (label & 0xFF);
    word |= ((sdi & 0x3) << 8);
    word |= ((data & 0x7FFFF) << 10);
    word |= ((ssm & 0x3) << 29);

    if (compute_parity(word)) word |= (1u << 31);
    return word;
}

ARINC429_Word arinc429_parse(uint32_t raw) {
    ARINC429_Word w;
    w.label = raw & 0xFF;
    w.sdi   = (raw >> 8) & 0x3;
    w.data  = (raw >> 10) & 0x7FFFF;
    w.ssm   = (raw >> 29) & 0x3;

    bool parity_bit = (raw >> 31) & 0x1;
    bool expected   = compute_parity(raw & 0x7FFFFFFF);
    w.parity = (parity_bit == expected);
    return w;
}

__attribute__((weak))
int32_t arinc429_decode_bnr(uint32_t data) {
    // 19-bit two's complement
    if (data & (1u << 18)) { // sign bit
        return (int32_t)(data | ~0x7FFFF); // sign extend
    }
    return (int32_t)data;
}

__attribute__((weak))
uint32_t arinc429_encode_bnr(int32_t value) {
    return (uint32_t)(value & 0x7FFFF);
}

__attribute__((weak))
int32_t arinc429_decode_bcd(uint32_t data) {
    int result = 0;
    int multiplier = 1;
    for (int i = 0; i < 5; i++) { // up to 5 digits
        int digit = (data >> (i*4)) & 0xF;
        result += digit * multiplier;
        multiplier *= 10;
    }
    return result;
}

__attribute__((weak))
uint32_t arinc429_encode_bcd(int value) {
    uint32_t data = 0;
    for (int i = 0; i < 5 && value > 0; i++) {
        int digit = value % 10;
        data |= (digit & 0xF) << (i*4);
        value /= 10;
    }
    return data & 0x7FFFF;
}

__attribute__((weak))
bool arinc429_get_discrete(uint32_t data, int bit) {
    return (data >> bit) & 1;
}

__attribute__((weak))
uint32_t arinc429_set_discrete(uint32_t data, int bit, bool val) {
    if (val) data |= (1u << bit);
    else     data &= ~(1u << bit);
    return data & 0x7FFFF;
}