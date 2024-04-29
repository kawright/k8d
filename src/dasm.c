// Copyright (C) 2024  KA Wright

// dasm.c - Disassembly operations

#include <stdint.h>
#include <stdio.h>

#include "dasm.h"
#include "err.h"

void init_dasm(Dasm *this) {
    this->pc        = ADDR_PROG_START;
    this->prog_len  = 0;
    for (uint16_t i = 0; i <= ADDR_PROG_END; i++) {
        this->prog[i] = 0;
    }
}

void load_dasm(Dasm *this, const char *fname, Err *err) {
    FILE *fp = fopen(fname, "rb");
    if (fp == NULL) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not open file %s", fname);
        return;
    }
    fseek(fp, 0, SEEK_END);
    
    uint64_t fsz;           // Temp in case file too large for 16 bits
    fsz= ftell(fp);
    if (fsz > (ADDR_PROG_END - ADDR_PROG_START + 1)) {
        err->code = ERR_DATA;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "File %s too large", fname);
        return;
    }
    this->prog_len = fsz;
    
    fseek(fp, 0, SEEK_SET);
    if (fread(this->prog + ADDR_PROG_START, this->prog_len, 1, fp) != 1) {
        err->code = ERR_IO;
        snprintf(err->msg, MAX_ERR_MSG_LEN, "Could not read file %s", fname);
    }
    fclose(fp);
}

void dasm_instr(Dasm *this, char *out) {
    if (dasm_eof(this)) {
        return;
    }
    uint8_t     nib_a;
    uint8_t     nib_b;
    uint8_t     nib_c;
    uint8_t     nib_d;
    uint8_t     kk;
    uint16_t    nnn;
    uint16_t    instr;
    instr = (this->prog[this->pc] << 8) + this->prog[this->pc + 1];
    
    nib_a   = (instr & 0xf000) >> 12;
    nib_b   = (instr & 0x0f00) >> 8;
    nib_c   = (instr & 0x00f0) >> 4;
    nib_d   = (instr & 0x000f);
    kk      = (nib_c << 4) + nib_d;
    nnn     = (nib_b << 8) + kk;

    switch (nib_a) {
        
        case 0x0:
        
        // 00e0 - CLS
        if (nnn == 0x0e0) {
            sprintf(out, "@%03x     0x%04x    CLS\n", this->pc, instr);
            break;
        
        }
 
        // 00ee - RET
        if (nnn == 0x0ee) {
            sprintf(out, "@%03x     0x%04x    RET\n", this->pc, instr);
            break;
        }
        
        // 0nnn - SYS addr
        sprintf(out, "@%03x     0x%04x    SYS     @%03x\n", this->pc, 
            instr, nnn);
        break;


        // 1nnn - JP addr
        case 0x1:
        sprintf(out, "@%03x     0x%04x    JP      @%03x\n", this->pc, 
            instr, nnn);
        break;

        
        // 2nnn - CALL addr
        case 0x2:
        sprintf(out, "@%03x     0x%04x    CALL    @%03x\n", this->pc, 
            instr, nnn);
        break;

        // 3xkk - SE Vx, byte
        case 0x3:
        sprintf(out, "@%03x     0x%04x    SE      V%01x 0x%02x\n", 
            this->pc, instr, nib_b, kk);
        break;

        // 4xkk - SNE Vx, byte
        case 0x4:
        sprintf(out, "@%03x     0x%04x    SNE     V%01x 0x%02x\n", 
            this->pc, instr, nib_b, kk);
        break;

        // 5xy0 - SE Vx, Vy
        case 0x5:
        sprintf(out, "@%03x     0x%04x    SE      V%01x V%01x\n", 
            this->pc, instr, nib_b, nib_c);
        break;

        // 6xkk - LD Vx, byte
        case 0x6:
        sprintf(out, "@%03x     0x%04x    LD      V%01x 0x%02x\n", 
            this->pc, instr, nib_b, kk);
        break;

        // 7xkk - ADD Vx, byte
        case 0x7:
        sprintf(out, "@%03x     0x%04x    ADD     V%01x 0x%02x\n", 
            this->pc, instr, nib_b, kk);
        break;

        case 0x8:
        switch (nib_d) {
            
            // 8xy0 - LD Vx, Vy
            case 0x0:
            sprintf(out, "@%03x     0x%04x    LD      V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xy1 - OR Vx, Vy
            case 0x1:
            sprintf(out, "@%03x     0x%04x    OR      V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xy2 - AND Vx, Vy
            case 0x2:
            sprintf(out, "@%03x     0x%04x    AND     V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xy3 - XOR Vx, Vy
            case 0x3:
            sprintf(out, "@%03x     0x%04x    XOR     V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xy4 - ADD Vx, Vy
            case 0x4:
            sprintf(out, "@%03x     0x%04x    ADD     V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xy5 - SUB Vx, Vy
            case 0x5:
            sprintf(out, "@%03x     0x%04x    SUB     V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xy6 - SHR Vx, {Vy}
            case 0x6:
            sprintf(out, "@%03x     0x%04x    SHR     V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xy7 - SUBN Vx, Vy
            case 0x7:
            sprintf(out, "@%03x     0x%04x    SUBN    V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // 8xye - SHL Vx, {Vy}
            case 0xe:
            sprintf(out, "@%03x     0x%04x    SHL     V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

            // Illegal opcode
            default:
            sprintf(out, "@%03x     0x%04x    ???     V%01x V%01x\n", 
                this->pc, instr, nib_b, nib_c);
            break;

        }
        break;

        // 9xy0 - SNE Vx, Vy
        case 0x9:
        sprintf(out, "@%03x     0x%04x    SNE     V%01x V%01x\n", 
            this->pc, instr, nib_b, nib_c);
        break;

        // annn - LD I, addr
        case 0xa:
        sprintf(out, "@%03x     0x%04x    LD      I @%03x\n", this->pc, 
            instr, nnn);
        break;

        // bnnn - JP V0, addr
        case 0xb:
        sprintf(out, "@%03x     0x%04x    JP      V0 @%03x\n", this->pc, 
            instr, nnn);
        break;

        // cxkk - RND Vx, byte
        case 0xc:
        sprintf(out, "@%03x     0x%04x    RND     V%01x 0x%02x\n", 
            this->pc, instr, nib_b, kk);
        break;

        // dxyn - DRW Vx, Vy, nibble
        case 0xd:
        sprintf(out, "@%03x     0x%04x    DRW     V%01x V%01x 0x%01x\n", 
            this->pc, instr, nib_b, nib_c, nib_d);
        break; 

        case 0xe:
        switch (kk) {
            
            // ex9e - SKP Vx
            case 0x9e:
            sprintf(out, "@%03x     0x%04x    SKP     V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // exa1 - SKNP Vx
            case 0xa1:
            sprintf(out, "@%03x     0x%04x    SKNP    V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // Illegal opcode
            default:
            sprintf(out, "@%03x     0x%04x    ???     0x%01x\n", this->pc, 
                instr, kk);
        }
        break;

        case 0xf:
        switch (kk) {

            // fx07 - LD Vx, DT
            case 0x07:
            sprintf(out, "@%03x     0x%04x    LD      V%01x DT\n", this->pc, 
                instr, nib_b);
            break;

            // fx0a - LD Vx, K
            case 0x0a:
            sprintf(out, "@%03x     0x%04x    LD      V%01x K\n", this->pc, 
                instr, nib_b);
            break;

            // fx15 - LD DT, Vx
            case 0x15:
            sprintf(out, "@%03x     0x%04x    LD      DT V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // fx18 - LD ST, Vx
            case 0x18:
            sprintf(out, "@%03x     0x%04x    LD      ST V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // fx1e - ADD I, Vx
            case 0x1e:
            sprintf(out, "@%03x     0x%04x    ADD     I V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // fx29 - LD F, Vx
            case 0x29:
            sprintf(out, "@%03x     0x%04x    LD      F V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // fx33 - LD B, Vx
            case 0x33:
            sprintf(out, "@%03x     0x%04x    LD      B V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // fx55 - LD [I], Vx
            case 0x55:
            sprintf(out, "@%03x     0x%04x    LD      [I] V%01x\n", this->pc, 
                instr, nib_b);
            break;

            // fx65 - LD Vx, [I]
            case 0x65:
            sprintf(out, "@%03x     0x%04x    LD      V%01x [I]\n", this->pc, 
                instr, nib_b);
            break;

            default:
            sprintf(out, "@%03x     0x%04x    ???     0x%02x\n", this->pc, 
                instr, kk);
            break;
            
        }
        break;
    }
    this->pc += 2;
}

bool dasm_eof(const Dasm *this) {
    return this->pc >= (ADDR_PROG_START + this->prog_len);
}
