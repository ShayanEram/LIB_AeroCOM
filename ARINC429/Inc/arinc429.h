#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// ARINC 429 word representation
typedef struct {
    uint8_t label;       // 8 bits
    uint8_t sdi;         // 2 bits
    uint32_t data;       // 19 bits (we'll store in 32 for convenience)
    uint8_t ssm;         // 2 bits
    bool parity;         // 1 bit
} ARINC429_Word;

uint32_t arinc429_build(uint8_t label, uint8_t sdi, uint32_t data, uint8_t ssm);
ARINC429_Word arinc429_parse(uint32_t raw);
int32_t arinc429_decode_bnr(uint32_t data);
uint32_t arinc429_encode_bnr(int32_t value);
int32_t arinc429_decode_bcd(uint32_t data);
uint32_t arinc429_encode_bcd(int value);
bool arinc429_get_discrete(uint32_t data, int bit);
uint32_t arinc429_set_discrete(uint32_t data, int bit, bool val);

#ifdef __cplusplus
}
#endif