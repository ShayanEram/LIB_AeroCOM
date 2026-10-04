#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { WORD_COMMAND, WORD_STATUS, WORD_DATA } MIL1553_WordType;

typedef struct {
    uint8_t rt_addr;   // 0..30
    uint8_t tr;        // 0=BC->RT, 1=RT->BC
    uint8_t subaddr;   // 0..30 data, 31=mode
    uint8_t wc_or_mode;// 0=32 words, else 1..31 or mode code
} MIL1553_Command;

typedef struct {
    uint8_t rt_addr;
    bool msg_error;
    bool service_request;
    bool busy;
    bool subsystem_flag;
    bool dynamic_bus_control;
    bool terminal_flag;
} MIL1553_Status;

typedef struct {
    uint16_t data; // 16-bit payload
} MIL1553_Data;


uint16_t mil1553_encode_command(MIL1553_Command c);
MIL1553_Command mil1553_decode_command(uint16_t w);
uint16_t mil1553_encode_status(MIL1553_Status s);
MIL1553_Status mil1553_decode_status(uint16_t w);
uint16_t mil1553_encode_data(MIL1553_Data d);
MIL1553_Data mil1553_decode_data(uint16_t w);
uint16_t mil1553_serialize(uint16_t word);
uint16_t mil1553_deserialize(uint16_t net_word);

#ifdef __cplusplus
}
#endif