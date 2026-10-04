#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AFDX_MAX_PAYLOAD 1471
#define AFDX_SEQ_MOD     16

typedef struct {
    uint16_t vl_id;                 // Virtual Link identifier
    uint8_t  seq_num;               // 4-bit sequence number
    uint16_t payload_len;           // length of payload
    uint8_t  payload[AFDX_MAX_PAYLOAD];
} AFDX_Frame;


AFDX_Frame afdx_build(uint16_t vl_id, uint8_t seq_num, const uint8_t* data, uint16_t len);
uint16_t afdx_serialize(const AFDX_Frame* f, uint8_t* out_buf);
AFDX_Frame afdx_deserialize(const uint8_t* buf, uint16_t len);
bool afdx_validate_seq(uint8_t expected, uint8_t incoming);

#ifdef __cplusplus
}
#endif