// Copyright (C) 2024  KA Wright

// dasm.h - Disassembly operations

#ifndef __DASM_H__
#define __DASM_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "err.h"

#define ADDR_PROG_START     0x200
#define ADDR_PROG_END       0xfff
#define MAX_DASM_LN_LEN     127

// Stores info needed to disassemble a chip-8 binary.
typedef struct __DASM__ {
    uint16_t        pc;
    uint16_t        prog_len;
    uint8_t         prog[ADDR_PROG_END + 1];
} Dasm;

// Initialize a Dasm. Must be called before loading.
void init_dasm(Dasm *this);

// Load a binary file into a Dasm.
void load_dasm(Dasm *this, const char *fname, Err *err);

// Disassemble one instruction and write it to the out param. Return
// immediately if at EOF.
void dasm_instr(Dasm *this, char *out);

// Is a Dasm at EOF?
bool dasm_eof(const Dasm *this);

#endif
