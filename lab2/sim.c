#include <stdio.h>
#include "shell.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


uint8_t op;
uint32_t instruction;
uint32_t addr_rs, addr_rt, addr_rd;
uint32_t rs, rt, rd;      //to save the register value
uint8_t func;
uint16_t itemp;         //for save immidiate number
short offset = 0; //declare a short to be used in bgtz!!!

  //please take a loot at https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_Green_Sheet.pdf
  //				 or
  //  https://web.cse.ohio-state.edu/~crawfis.3/cse675-02/Slides/MIPS%20Instruction%20Set.pdf

void fetch()
{
	printf("fetch \n");
	instruction = mem_read_32(CURRENT_STATE.PC); //the instruction 
	NEXT_STATE.PC = CURRENT_STATE.PC + 4;

}

void decode()
{
	//printf("decode ");
	op = instruction >> 26; //find the 31-26 bit


  //  printf("the op: %x ",op);
	// if op==0, it is R type
	if (op == 0) {

		addr_rs = (instruction >> 21) & 0x1f;
		addr_rt = (instruction >> 16) & 0x1f;
		addr_rd = (instruction >> 11) & 0x1f;
		rs = CURRENT_STATE.REGS[addr_rs];   //2^5=32
		rt = CURRENT_STATE.REGS[addr_rt];
		rd = CURRENT_STATE.REGS[addr_rd];
		func = instruction & 0x3f;

	}


	if (op == 8) {          //addi:001000       
		printf("addi instruction \n");
		addr_rs = (instruction >> 21) & 0x1f;
		addr_rt = (instruction >> 16) & 0x1f;
		rs = CURRENT_STATE.REGS[addr_rs];   //2^5=32
		rt = CURRENT_STATE.REGS[addr_rt];
		itemp = instruction & 0x0000ffff;

		if (itemp > 32767) {    //2^15-1=32767 is the largest number CHECKING OVERFLOW
			itemp = ~itemp + 1;
			rt = rs - itemp;
			NEXT_STATE.FLAG_V = 1; //overflow flag!
		}
		else {
			rt = rs + itemp;
		}
		NEXT_STATE.REGS[addr_rt] = rt; // It is fine if you just execute here instead of executing in execute() function. 
	}

	if(op == 9) //addiu: 001001
	{
		printf("addiu instruction \n"); // same as addi without overflow checking!
		addr_rs = (instruction >> 21) & 0x1f;	//isolate rs (SAME FOR ALL I TYPE)
		addr_rt = (instruction >> 16) & 0x1f;	//isolate rt (SAME FOR ALL I TYPE)
		rs = CURRENT_STATE.REGS[addr_rs];   //2^5=32
		itemp = instruction & 0x0000ffff;	//isolate offset (SAME FOR ALL I TYPE)

		rt = rs + itemp;
		NEXT_STATE.REGS[addr_rt] = rt; // It is fine if you just execute here instead of executing in execute() function. 
	}

	if(op == 35) //lw instruction
	{
		printf("lw instruction \n");
		addr_rs = (instruction >> 21) & 0x1f; //base address
		addr_rt = (instruction >> 16) & 0x1f; //destination register#
		itemp = instruction & 0x0000ffff; //offset

		rs = CURRENT_STATE.REGS[addr_rs];
		NEXT_STATE.REGS[addr_rt] = mem_read_32(rs+itemp+0x10000000); //offset + address value !!NEEDED TO ADD OFFSET OTHERWISE WOULD NEED TO USE A 32 BIT IMMEDIATE WHICH ADDS EXTRA INSTRUCTIONS NOT SPECIFIED BY THE ASSIGNEMT
	}	//IN ORDER TO USE 16 BIT IMMEDIATE THE HEX OFFSET MUST BE ADDED!

	if(op == 43) //sw instruction
	{
		printf("sw instruction \n");
		addr_rs = (instruction >> 21) & 0x1f; //base register
		addr_rt = (instruction >> 16) & 0x1f; //register containing value to be storrs
		itemp = instruction & 0x0000ffff;
		
		rs = CURRENT_STATE.REGS[addr_rs];
		rt = CURRENT_STATE.REGS[addr_rt];
		mem_write_32(rs+itemp+0x10000000, rt);//offset + address value !!NEEDED TO ADD OFFSET OTHERWISE WOULD NEED TO USE A 32 BIT IMMEDIATE WHICH ADDS EXTRA INSTRUCTIONS NOT SPECIFIED BY THE ASSIGNEMT
	}	//IN ORDER TO USE 16 BIT IMMEDIATE THE HEX OFFSET MUST BE ADDED!

	if(op == 7) //bgtz instruction
	{
		printf("bgtz instruction \n");
		addr_rs = (instruction >> 21) & 0x1f; //base address
		//addr_rt = (instruction >> 16) & 0x1f; //destination address
		itemp = (instruction & 0x0000ffff); //jump distance

		offset = itemp; //create a short to sign offset it! CASTING TO A SHORT TO SIMULATE SIGN OFFSET

		if(CURRENT_STATE.REGS[addr_rs] > 0) //ACTUAL LOGIC FOR GTZ
		{
			NEXT_STATE.PC = ((int32_t) NEXT_STATE.PC)+4*offset; //MULTIPLY OFFSET BY 4 TO BIT TO BYTE

		}
	}
	if(op == 2)
	{
		printf("j instruction \n"); //j instruction
		
		uint32_t big_temp = instruction & 0x3FFFFFF; //isolates the last 26
		big_temp <<=2; //mlt by 4
		uint32_t effective_addr = (NEXT_STATE.PC & 0xF0000000) | big_temp; 
		NEXT_STATE.PC = effective_addr;//set next state to the calculated value

		// First, the immediate is extracted from the instruction.
		// Since the effective address is 32 bits, the 16 bit immediate
		// must be promoted to 32 bits.
		// Then, the proper bit arithmetic is performed to calculate
		// the final absolute address.
	}

}

void execute()
{
	printf("execute\n");
	if (op == 0) {
		switch (func) {
		case 32:      //add:100000
			
			printf("add instruction \n");
			rd = 0;
			//      if(rt>2147483647)
			rd = rt + rs;
			NEXT_STATE.FLAG_V = rd > 2147483647;
			NEXT_STATE.REGS[addr_rd] = rd;
			break;

		case 12:   //system call:001100
			if (CURRENT_STATE.REGS[2] == 10) {  //v0==10 then exit
				printf("systemcall: exit\n");
				RUN_BIT = FALSE;
			}
			if (CURRENT_STATE.REGS[2] == 1) {   //v0==1: print int
				rt = CURRENT_STATE.REGS[4];   //a0 is 4th register
				printf("\n print the number:   %d \n ", rt);

			}
			break;
		default:
			break;
		
		
		case 33: //addu: 100001
			printf("addu instruction \n"); //quite literally add without error checking
			rd = 0;
			rd = rt + rs;
			NEXT_STATE.REGS[addr_rd] = rd;//set next state to the calculated value
			break;
		
		case 34:      //sub:100010
			printf("sub instruction \n");
			rd = 0;
			rd = rs - rt; //quite literaly the add instuction just with - instead of plus
			
			NEXT_STATE.FLAG_V = rd > 2147483647; //set overflow bit
			NEXT_STATE.REGS[addr_rd] = rd; //set next state to the calculated value
			break;
		case 35: //subu 100011
			printf("subu instruction \n");	//same as sub without error checking
			rd = 0;
			rd = rs - rt;
			NEXT_STATE.REGS[addr_rd] = rd;//set next state to the calculated value
			break;

		case 36: //and 100100
			printf("and instruction \n");
			rd = 0;
			rd = rt & rs; 		//same as add and sub just with logical and instead of + or -
			NEXT_STATE.REGS[addr_rd] = rd;//set next state to the calculated value
			break;
		}

	}



}
void process_instruction()
{
	/* execute one instruction here. You should use CURRENT_STATE and modify
	 * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
	 * access memory. */
	fetch();
	decode();
	execute();

}
