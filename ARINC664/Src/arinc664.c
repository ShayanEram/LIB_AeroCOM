#include "arinc664.h"

__attribute__((weak))
AFDX_Frame afdx_build(uint16_t vl_id, uint8_t seq_num, const uint8_t* data, uint16_t len) {
    AFDX_Frame f;
    f.vl_id = vl_id;
    f.seq_num = seq_num % AFDX_SEQ_MOD;
    f.payload_len = (len > AFDX_MAX_PAYLOAD) ? AFDX_MAX_PAYLOAD : len;
    for (int i = 0; i < f.payload_len; i++) {
        f.payload[i] = data[i];
    }
    return f;
}

// Serialize to raw buffer for sending over TCP/UDP
__attribute__((weak))
uint16_t afdx_serialize(const AFDX_Frame* f, uint8_t* out_buf) {
    uint16_t offset = 0;
    out_buf[offset++] = (f->vl_id >> 8) & 0xFF;
    out_buf[offset++] = f->vl_id & 0xFF;
    out_buf[offset++] = f->seq_num & 0x0F;
    out_buf[offset++] = (f->payload_len >> 8) & 0xFF;
    out_buf[offset++] = f->payload_len & 0xFF;
    for (int i = 0; i < f->payload_len; i++) {
        out_buf[offset++] = f->payload[i];
    }
    return offset;
}

__attribute__((weak))
AFDX_Frame afdx_deserialize(const uint8_t* buf, uint16_t len) {
    AFDX_Frame f;
    f.vl_id = (buf[0] << 8) | buf[1];
    f.seq_num = buf[2] & 0x0F;
    f.payload_len = (buf[3] << 8) | buf[4];
    if (f.payload_len > AFDX_MAX_PAYLOAD) f.payload_len = AFDX_MAX_PAYLOAD;
    for (int i = 0; i < f.payload_len && (5+i) < len; i++) {
        f.payload[i] = buf[5+i];
    }
    return f;
}

__attribute__((weak))
bool afdx_validate_seq(uint8_t expected, uint8_t incoming) {
    return ((expected + 1) % AFDX_SEQ_MOD) == incoming;
}