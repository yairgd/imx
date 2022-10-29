/*
 * =====================================================================================
 *
 *       Filename:  instructions.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2022 05:13:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef INSRUCTIONS
#define INSRUCTIONS 
enum OPCODES {
	INVALID,
	OP_ANDI, 	
	OP_LOOP, 	
	OP_ST,		
	OP_LDI,		
	OP_DONE,		
	OP_CMPEQI,	
	OP_ADDI, 	
	OP_BT,	 	
	OP_XORI,	 
	OP_LD,
	OP_MOV,
	OP_BF,
	OP_BSF,
	OP_BDF,
	OP_ADD,
	OP_SUB,
	OP_OR,
	OP_ANDN,
	OP_AND,
	OP_CMPEQ,
	OP_CMPLT,
	OP_CMPHS,
	OP_JMPR,
	OP_JSRR,
	OP_LDRPC,
	OP_REVB,
	OP_REVBLO,
	OP_RORB,
	OP_RORL,
	OP_LSRL,
	OP_ASRL,
	OP_LSLL,
	OP_BCLRI,
	OP_BSETI,
	OP_BTSTI
};


typedef void (*decode_t) (enum OPCODES op,...);

struct instruction_s {
	char op_name[16];
	enum OPCODES opcode;	
	char instruction[16];
	decode_t decode;
} ;
void decode_5x3r38i(enum OPCODES opcode, ... );
void decode_5x3r5d3b(enum OPCODES  a1, ...);
void decode_8x8p(enum OPCODES  a1,...);
void decode_5x3r5x3s(enum OPCODES  a1, ...);
void decode_loop(enum OPCODES  a1, ...);
void decode_done(enum OPCODES  a1, ...);
void decode_label(char *);
void push_label(char *label);
void decode_3r(enum OPCODES a1, ...);
void decode_3r5n(enum OPCODES a1, ...);
void decode_invalid(enum OPCODES a1, ...);

decode_t get_decode_function(enum OPCODES op,char *op_name);



#endif
