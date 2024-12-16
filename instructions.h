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
#include <stdbool.h>


#define  OP_NAME_ANDI 	"andi"
#define  OP_NAME_LOOP 	"loop"  
#define  OP_NAME_ST  	"st"     
#define  OP_NAME_LDI	"ldi"   
#define  OP_NAME_DONE	"done"   
#define  OP_NAME_CMPEQI	"cmpeqi" 
#define  OP_NAME_ADDI	"addi"   
#define  OP_NAME_BT	"bt"     
#define  OP_NAME_MOV	"mov"    
#define  OP_NAME_ADD	"add"    
#define  OP_NAME_SUB	"sub"    
#define  OP_NAME_OR	"or"     
#define  OP_NAME_ANDN	"andn"  
#define  OP_NAME_AND	"and"    
#define  OP_NAME_CMPEQ	"cmpeq"  
#define  OP_NAME_CMPLT	"cmplt"  
#define  OP_NAME_CMPHS	"cmphs" 

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
	OP_BTSTI,
};



struct instruction_s {
	char op_name[16];
	enum OPCODES opcode;	
	char instruction[16];
	void (*encode) (enum OPCODES op,...);
	void (*decode) (int pc, struct instruction_s  *inst);

} ;

typedef void (*decode_t) (unsigned short op_code, struct instruction_s  *inst);
typedef void (*encode_t) (enum OPCODES op,...);


void push_label(char *label, int pc);
encode_t get_encode_function(enum OPCODES op,char *op_name);

void encode_5x3r38i(enum OPCODES opcode, ... );
void encode_5x3r5d3b(enum OPCODES  a1, ...);
void encode_8x8p(enum OPCODES  a1,...);
void encode_5x3r5x3s(enum OPCODES  a1, ...);
void encode_loop(enum OPCODES  a1, ...);
void encode_done(enum OPCODES  a1, ...);
void encode_label(char *);
void encode_3r(enum OPCODES a1, ...);
void encode_3r5n(enum OPCODES a1, ...);
void encode_invalid(enum OPCODES a1, ...);




void decode_5x3r38i(int pc, struct instruction_s  *inst);
void decode_5x3r5d3b(int pc, struct instruction_s  *inst);
void decode_8x8p(int pc, struct instruction_s  *inst) ;
void decode_5x3r5x3s(int pc, struct instruction_s  *inst) ;
void decode_loop(int pc, struct instruction_s  *inst) ;
void decode_done(int pc, struct instruction_s  *inst) ;
void decode_3r(int pc, struct instruction_s  *inst) ;


void push_to_memory(int type ,...);

#endif
