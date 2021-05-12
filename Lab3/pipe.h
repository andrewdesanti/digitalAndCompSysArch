//Andrew DeSanti, Joshua Hornilla
//I pledge my honor that I have abided by the Stevens Honor System: AD,JD

#ifndef _PIPE_H_
#define _PIPE_H_

#include "shell.h"
#include "stdbool.h"
#include <limits.h>

typedef struct Pipe_Reg_IFtoDE {    //store 2 pc for pipeline
    uint32_t ins1;
    uint32_t PC;
} Pipe_Reg_IFtoDE;

typedef struct Pipe_Reg_DEtoEX {
    uint32_t PC; //
    uint8_t ex; //regdest, aluop1,aluop2,aluscr
    uint8_t wb_mem;
     //1:PC+4 2:read from register1 3:read from register1 4:immediate num
    uint32_t ins;
    uint32_t rs_value;
    uint32_t rt_value;
    uint32_t inum;

    uint8_t addr_rd;     //instruction 11-20
    uint8_t addr_rt;
    uint8_t addr_rs;

} Pipe_Reg_DEtoEX;

typedef struct Pipe_Reg_EXtoMEM {
    uint8_t wb_mem;     //mem: branch,mem_read,mem_write wb:reg_write, mem_to_reg
    uint32_t newPC1;    // for branch
	uint32_t newPC2;    // for jump
    uint32_t AluResult;
    uint32_t rt_value;
    int zero;   //only one bit, true=1, false=0;
    uint8_t addr_rd;     //5 bits
} Pipe_Reg_EXtoMEM;

typedef struct Pipe_Reg_MEMtoWB {
    uint8_t wb; //only 2 bit to indicate reg_write, mem_to_reg
	uint8_t wbCopy; //only 2 bit to indicate reg_write, mem_to_reg
    uint32_t rt_value;  // read data: tempary save data from memory
    uint32_t AluResult; // result from R type
	uint32_t AluResultCopy; // for forwarding
    uint8_t addr_rd;     //actually 5 bit    address of register
} Pipe_Reg_MEMtoWB;

typedef struct CPU_State {
	/* register file state */
	uint32_t REGS[MIPS_REGS];
	int FLAG_N;        /* flag N */
	int FLAG_Z;        /* flag Z */
	int FLAG_V;        /* flag V */
	int FLAG_C;        /* flag C */

	/* program counter in fetch stage */
	uint32_t PC;
	
} CPU_State;

int RUN_BIT;

/* global variable -- pipeline state */
extern CPU_State CURRENT_STATE;

/* this function calls the others */
void pipe_cycle();

/* each of these functions implements one stage of the pipeline */
void pipe_stage_fetch();
void pipe_stage_decode();
void pipe_stage_execute();
void pipe_stage_mem();
void pipe_stage_wb();

#endif
