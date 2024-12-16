/*
 * =====================================================================================
 *
 *       Filename:  instructions.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2022 05:11:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "instructions.h"
#include "y.tab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

//http://blog.petri.us/sdma-hacking/part-1.html


static unsigned short memory[4096];
static int pc = 0;

static struct label {
	unsigned long hash;
	int pc;
	char *name;
} lables[256]={0};
 static int label_idx = 0;

/// reffer here: 
/// i.MX 6SoloX Applications Processor Reference Manual
/// p. 3725 (55.5.2 SDMA Instruction Set)
/// or
/// i.MX 8M Dual/8M QuadLite/8M Quad Applications Processors Reference Manual
/// p. 1176 7.2.4.2 SDMA Instruction Set
///
struct instruction_s   instruction_set[] = {
	{.opcode = OP_ANDI, 	.instruction = "00111rrriiiiiiii", .encode = encode_5x3r38i, .op_name="andi",	.decode = decode_5x3r38i}, /* ldi r,i */
	{.opcode = OP_LDI,	.instruction = "00001rrriiiiiiii", .encode = encode_5x3r38i, .op_name="ldi", 	.decode = decode_5x3r38i},
	{.opcode = OP_ADDI,	.instruction = "00011rrriiiiiiii", .encode = encode_5x3r38i, .op_name="addi",	.decode = decode_5x3r38i}, 
	{.opcode = OP_CMPEQI,	.instruction = "01001rrriiiiiiii", .encode = encode_5x3r38i, .op_name="cmpeqi",	.decode = decode_5x3r38i}, 
	{.opcode = OP_XORI,	.instruction = "00010rrriiiiiiii", .encode = encode_5x3r38i, .op_name="xori",	.decode = decode_5x3r38i}, 

	{.opcode = OP_LOOP, 	.instruction = "011110ffnnnnnnnn", .encode = encode_loop, .op_name="loop",	.decode = decode_loop }, /* loop n,ff*/

	{.opcode = OP_ST, 	.instruction = "01011rrrdddddbbb", .encode = encode_5x3r5d3b, .op_name="st",	.decode = decode_5x3r5d3b},   /* st r,(d,b)*/
	{.opcode = OP_LD, 	.instruction = "01010rrrdddddbbb", .encode = encode_5x3r5d3b, .op_name="xori",	.decode = decode_5x3r5d3b},   /* ld r,(d,b)*/

	{.opcode = OP_DONE,	.instruction = "00000jjj00000000", .encode = encode_done, .op_name="done",	.decode = decode_done  },   /* done jjj*/

	{.opcode = OP_BT, 	.instruction = "01111101pppppppp", .encode = encode_8x8p,    .op_name="bt",	.decode = decode_8x8p},   /* bt pppp*/
	{.opcode = OP_BF, 	.instruction = "01111100pppppppp", .encode = encode_8x8p,    .op_name="bf",	.decode = decode_8x8p},   /* bf pppp*/
	{.opcode = OP_BSF, 	.instruction = "01111110pppppppp", .encode = encode_8x8p,    .op_name="bsf",	.decode = decode_8x8p},   /* bsf pppp*/
	{.opcode = OP_BDF, 	.instruction = "01111111pppppppp", .encode = encode_8x8p,    .op_name="bdf",	.decode = decode_8x8p},   /* bff pppp*/


	{.opcode = OP_MOV,	.instruction = "00000rrr10001sss", .encode = encode_5x3r5x3s, .op_name="mov",	.decode = decode_5x3r5x3s },   /* mov r,s*/
	{.opcode = OP_ADD, 	.instruction = "00000rrr10010sss", .encode = encode_5x3r5x3s, .op_name="add",	.decode = decode_5x3r5x3s },   /* add r,s*/
	{.opcode = OP_SUB, 	.instruction = "00000rrr10011sss", .encode = encode_5x3r5x3s, .op_name="sub",	.decode = decode_5x3r5x3s },   /* mov r,s*/
	{.opcode = OP_OR, 	.instruction = "00000rrr10101sss", .encode = encode_5x3r5x3s, .op_name="or",	.decode = decode_5x3r5x3s },   /* mov r,s*/
	{.opcode = OP_ANDN,	.instruction = "00000rrr10110sss", .encode = encode_5x3r5x3s, .op_name="andn",	.decode = decode_5x3r5x3s },   /* mov r,s*/
	{.opcode = OP_AND,	.instruction = "00000rrr10111sss", .encode = encode_5x3r5x3s, .op_name="and",	.decode = decode_5x3r5x3s },   /* mov r,s*/
	{.opcode = OP_CMPEQ, 	.instruction = "00000rrr11001sss", .encode = encode_5x3r5x3s, .op_name="cmpeq",	.decode = decode_5x3r5x3s },   /* mov r,s*/
	{.opcode = OP_CMPLT, 	.instruction = "00000rrr11010sss", .encode = encode_5x3r5x3s, .op_name="cmplt",	.decode = decode_5x3r5x3s },   /* mov r,s*/
	{.opcode = OP_CMPHS, 	.instruction = "00000rrr11011sss", .encode = encode_5x3r5x3s, .op_name="cmphs",	.decode = decode_5x3r5x3s },   /* mov r,s*/




	{.opcode = OP_JMPR, 	.instruction = "00000rrr00001000", .encode = encode_3r,    .op_name="jmpr",	.decode = decode_3r },   /* jumpr r*/
	{.opcode = OP_JSRR, 	.instruction = "00000rrr00001001", .encode = encode_3r,    .op_name="jsrr",	.decode = decode_3r},   /* jsrr r*/
	{.opcode = OP_LDRPC, 	.instruction = "00000rrr00001010", .encode = encode_3r,    .op_name="ldrpc",	.decode = decode_3r},   /* ldrpc r*/
	{.opcode = OP_REVB, 	.instruction = "00000rrr00010000", .encode = encode_3r,    .op_name="revb",	.decode = decode_3r},   /* rev r */
	{.opcode = OP_REVBLO, 	.instruction = "00000rrr00010001", .encode = encode_3r,    .op_name="revblo",	.decode = decode_3r},   /* revblo r */
	{.opcode = OP_RORB,	.instruction = "00000rrr00010010", .encode = encode_3r,    .op_name="rorb",	.decode = decode_3r},   /* rorb r */
	{.opcode = OP_RORL, 	.instruction = "00000rrr00010100", .encode = encode_3r,    .op_name="rorl",	.decode = decode_3r},   /* jmpr r */
	{.opcode = OP_LSRL, 	.instruction = "00000rrr00010101", .encode = encode_3r,    .op_name="lsrl",	.decode = decode_3r},   /* jmpr r */
	{.opcode = OP_ASRL, 	.instruction = "00000rrr00010110", .encode = encode_3r,    .op_name="arsl",	.decode = decode_3r},   /* jmpr r */
	{.opcode = OP_LSLL, 	.instruction = "00000rrr00010111", .encode = encode_3r,    .op_name="lsll",	.decode = decode_3r},   /* jmpr r */
	

	{.opcode = OP_BCLRI, 	.instruction = "00000rrr001nnnnn", .encode = encode_3r5n},   /* bclri r,n */
	{.opcode = OP_BSETI,	.instruction = "00000rrr010nnnnn", .encode = encode_3r5n},   /* nseti r,n */
	{.opcode = OP_BTSTI,	.instruction = "00000rrr011nnnnn", .encode = encode_3r5n},   /* btsti r,n */
	{0}

};

/* yacc functions definitions */
struct yy_buffer_state;
void yy_delete_buffer (struct yy_buffer_state *  );
struct yy_buffer_state * yy_scan_string(const char*);
int yyparse (void);

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
/**
 * Created  10/24/2022
 * @brief   push label as hash value
 * @param   
 * @return  
 */
void push_label(char *label,int _pc) {
	static int label_id = 0;
	char buf[32];

	if (_pc == -1)  _pc = pc;
	lables[label_idx].pc = _pc;
	if (label) {	
		lables[label_idx].name = strdup (label);
		lables[label_idx++].hash = hash((unsigned char*)label);
	} else {
		snprintf(buf, sizeof(buf),"noname%d",label_id++);
		lables[label_idx].name = strdup(buf);
		lables[label_idx++].hash = hash((unsigned char*)buf);
	}

	
}



/**
 * Created  11/05/2022
 * @brief   returs program counter value for a givven  label
 * @param   
 * @return  
 */
int get_pc_from_label(char *label) {
	int p = -1;

	for (int i = 0; i< label_idx;i++){
		if (lables[i].hash == hash((unsigned char*)label)) {
			p = lables[i].pc;
		}

	}
	return p;
}


/**
 * Created  11/05/2022
 * @brief   returns label by a givven program counter
 * @param   pc - vaue of proegram counter
 * @return  null pomter if no lable is defined, lebal name if it is defined
 */
char * label__get_by_pc(int pc) {	
	struct label *label = lables; 
	while (label->name) {
		if (label->pc == pc) {
			return label->name;
		}
		label++;
	}
	return 0;	
}

/**
 * Created  10/21/2022
 * @brief   modify charcates in the instruction string
 * @param  str - the instruction string
 * @param  v - the requted value
 * @param  c - the character to look for
 * @return  
 */
int modify_instruction_string(char *str, int v, char c)
{
	int i, j;

	for (i = 0; i < 16;i++) {
		if (str[i] == c)
			break;
	};
	if (i==16)
		return -1;

	for (j = i; j < 16; j++) {
		if (str[j] != c)
			break;
	}
	
	for ( j = j - 1;j >= i ;j--) {
		str[j] = (v&0x1) ? '1' : '0';
		v = v >> 1;
	}
	return 0;

}

int extract_operand_from_instruction(struct instruction_s  *inst, char c, unsigned short opcode)  {
	int v = 0;
	int i = 0,j=0;
	char *instruction_str = inst->instruction;

	while (instruction_str[i] != c && i < 16)
		i++;
	if (i < 16) {
		j = i;
		do {
			v |= (opcode & (1<< (15-j))) ==0 ? 0 : 1;
			v <<= 1;
			j++;
		} while  (j < 16 && instruction_str[j] == c);
		v >>= 1;
	}
	return v;
}

/**
 * Created  10/21/2022
 * @brief   converts instruction string to word
 * @param   
 * @return  
 */
void instruction_string_to_word(char *str)
{
	unsigned short v = 0;

	for (int i=0; i < 16; i++) {
		v |= (str[i]=='1') ? 1 : 0;
		v = v<<1;
	}
	memory[pc++] = v>>1;
}



/**
 * Created  10/28/2022
 * @brief   get encode function by opcode
 * @param   
 * @return  
 */
encode_t get_encode_function(enum OPCODES op,char *op_name) {
	struct instruction_s   *inst = instruction_set;
	
	while (inst->opcode) {
		if (inst->opcode == op) {
			// get the op code string name from the lexer definitions (see asm.lex)
			strncpy(inst->op_name, op_name, 16);
			return inst->encode;
		}
		inst++;
	}
	return encode_invalid;
}


/**
 * Created  10/28/2022
 * @brief   get instruction by opcode
 * @param   
 * @return  
 */
char * get_instruction_pattern(enum OPCODES op) {
	struct instruction_s   *inst = instruction_set;
	
	while (inst->opcode) {
		if (inst->opcode == op)
			return inst->instruction;
		inst++;
	}
	return "xxxxxxxxxxxxxxxx";
}


/**
 * Created  10/28/2022
 * @brief   
 * @param   
 * @return  
 */
void encode_invalid(enum OPCODES a1, ...) {
	printf ("invalid opcode %d \n",a1);
}

/**
 * Created  10/19/2022                         
 *                                                   1 2 3 4
 *                                                     r   i 
 * @brief   encodes this format 00111rrriiiiiiii -> op a , b 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void encode_5x3r38i(enum OPCODES a1, ...) {
	char instruction[16];
	int a2,a4;
   	va_list ap;

	memcpy (instruction,get_instruction_pattern(a1),16);

	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a4 = va_arg(ap, int);
	va_end(ap);

	modify_instruction_string(instruction, a2 , 'r'); 
	modify_instruction_string(instruction, a4 , 'i'); 
	instruction_string_to_word(instruction);

	//printf ("opcode %d %d,%d \n",a1,a2,a4);
}


/**
 * Created  10/29/2022
 * @brief   decodes to 5x3r38i instruction format
 * @param   
 * @return  
 */
void decode_5x3r38i(int pc, struct instruction_s  *inst) {
	unsigned short op_code = memory[pc];

	int r = extract_operand_from_instruction(inst, 'r', op_code);
	r &= 0x0007;

	int i = extract_operand_from_instruction(inst, 'i', op_code);
	i &= 0x00ff;

	printf("%04x  %04x  |  %s r%d,0x%02x\n",pc, op_code, inst->op_name, r,i);
}


/**
 * Created  10/29/2022
 * @brief   decodes to 5x3r38i instruction format
 * @param   
 * @return  
 */
void decode_5x3r5d3b(int pc, struct instruction_s  *inst) {
	unsigned short op_code = memory[pc];
	
	int r = extract_operand_from_instruction(inst, 'r', op_code);
	r &= 0x0007;

	int d = extract_operand_from_instruction(inst, 'd', op_code);
	d &= 0x0007;

	int b = extract_operand_from_instruction(inst, 'b', op_code);
	b &= 0x001f;


	printf("%04x  %04x  |  %s r%d, (r%d,%d)\n",pc, op_code, inst->op_name, r,b,d);
}



/**
 * Created  11/04/2022
 * @brief   decodes to 8x8p instruction format
 * @param   
 * @return  
 */
void decode_8x8p(int pc, struct instruction_s  *inst) {
	unsigned short op_code = memory[pc];
	char  buf[32];

	int v = extract_operand_from_instruction(inst, 'p', op_code);
	v =  (signed char)(v&0xff);


	// translate relative address to program counter value and get its lable value
	char *label = label__get_by_pc(pc+v+1);
	if (!label) {
		push_label(0, pc+v+1);
		label = label__get_by_pc(pc+v+1);
	}
	printf("%04x  %04x  |  %s %s\n",pc, op_code, inst->op_name, label);
	
}

void decode_5x3r5x3s(int pc, struct instruction_s  *inst) {
	unsigned short op_code = memory[pc];

	int r = extract_operand_from_instruction(inst, 'r', op_code);
	r &= 0x0007;

	int s = extract_operand_from_instruction(inst, 's', op_code);
	s &= 0x0007;

	printf("%04x  %04x  |  %s r%d,%d\n",pc, op_code, inst->op_name, r,s);
}

void decode_3r(int pc, struct instruction_s  *inst) {
	unsigned short op_code = memory[pc];

	int r = extract_operand_from_instruction(inst, 'r', op_code);
	r &= 0x0007;

	printf("%04x  %04x  |  %s r%d\n",pc, op_code, inst->op_name, r);
}




void decode_loop(int pc, struct instruction_s  *inst) {
	unsigned short op_code = memory[pc];
	int n = extract_operand_from_instruction(inst, 'n', op_code);
	n =  (signed char)(n&0xff);
		
	char *label = label__get_by_pc(pc+n+1);
	if (!label) {
		push_label(0, pc+n+1);
		label = label__get_by_pc(pc+n+1);
	}

	printf("%04x  %04x  |  %s %s\n",pc, op_code, inst->op_name, label);
}


void decode_done(int pc, struct instruction_s  *inst) {
	unsigned short op_code = memory[pc];

	int j = extract_operand_from_instruction(inst, 'j', op_code);
	j &= 0x0007;

	printf("%04x  %04x  |  %s %d\n",pc, op_code, inst->op_name, j);
}







/**
 * Created  10/19/2022                         
 *                                                      1   2  3
 *                                                          r  i 
 * @brief   encodes this format 00000rrr001nnnnn ->  blrci r1, i 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void encode_3r5n(enum OPCODES a1, ...) {
	char instruction[16];
	int a2, a3;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a3 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,get_instruction_pattern(a1),16);
	modify_instruction_string(instruction, a2 , 'r'); 
	modify_instruction_string(instruction, a3 , 'i'); 
	instruction_string_to_word(instruction);

//	printf ("opcode %d",a2);

}


/**
 * Created  10/19/2022                         
 *                                                      1  2  
 *                                                         r   
 * @brief   encodes this format 00000rrr00001000 -> jumpr r1 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void encode_3r(enum OPCODES a1, ...) {
	char instruction[16];
	int a2;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,get_instruction_pattern(a1),16);
	modify_instruction_string(instruction, a2 , 'r'); 
	instruction_string_to_word(instruction);

//	printf ("opcode %d",a2);

}


/**
 * Created  10/19/2022                         
 *                                                   1 2  3 4 5  6 7 8
 *                                                      r     b    d
 * @brief   encodes this format 01011rrrdddddbbb -> st r1 , ( r2 , 2 ) 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void encode_5x3r5d3b(enum OPCODES a1, ...) {
	char instruction[16];
	int a2,a5,a7;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a5 = va_arg(ap, int);
	a7 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,get_instruction_pattern(a1),16);
	modify_instruction_string(instruction, a2 , 'r'); 
	modify_instruction_string(instruction, a5 , 'b'); 
	modify_instruction_string(instruction, a7 , 'd'); 
	instruction_string_to_word(instruction);


	//printf ("opcode %d r%d,(r%d , %d)\n",a1,a2,a5,a7);

}

/**
 * Created  10/19/2022                         
 *                                                   1  2    3 
 *                                                      r    s
 * @brief   encodes this format 00000rrr10001sss -> mov r1 , r2
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void encode_5x3r5x3s(enum OPCODES a1 ,...) {
	char instruction[16];
	int a2,a4;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a4 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,get_instruction_pattern(a1),16);
	//printf ("opcode %d %d,%d \n",a1,a2,a4);
}


/**
 * Created  10/19/2022                         
 *                                                   1  2    
 *                                                      p    
 * @brief   encodes this format 01111101pppppppp -> bt label
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void encode_8x8p(enum OPCODES a1,...) {
	char instruction[16];
	char *a2;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, char*);
	va_end(ap);

	int p = get_pc_from_label(a2);
	if (p == -1) {
		printf("label %s not wans not defined\n",a2);
	}
	

	memcpy (instruction,get_instruction_pattern(a1),16);
	modify_instruction_string(instruction, p - pc - 1 , 'p'); 
	instruction_string_to_word(instruction);
	//printf ("opcode %d   %s\n",a1,a2);

}


/**
 * Created  10/19/2022                         
 *                                                   1    2     4
 *                                                        p     ff
 * @brief   encodes this format 011110ffnnnnnnnn -> loop label, ff
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void encode_loop(enum OPCODES a1 ,...) {
	char instruction[16];
	char *a2;
	int a4;
   	va_list ap;
	int p;


	va_start(ap, a1);
	a2 = va_arg(ap, char*);
	a4 = va_arg(ap,int);
	va_end(ap);

	p = get_pc_from_label(a2);
	if (p == -1) {
		//printf("label %s not wans not defined\n",a2);
	}
	
	memcpy (instruction,get_instruction_pattern(a1),16);
	modify_instruction_string(instruction, pc - p + 1, 'n'); 
	modify_instruction_string(instruction, a4 , 'f'); 
	instruction_string_to_word(instruction);


	//printf ("opcode %d   %s\n",a1,a2);

}


/**
 * Created  10/24/2024                         
 * @brief   pushes integr to memory, this instruction pushes  list of comman sepataed int into memory, 
 * it is usealy flooed by a label
 * .array: 1,2,3,4
 * .string: hellow world
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void push_to_memory(int type ,...) {
	static int p = 0;
	char * str;
   	va_list ap;
	int i;
	int  num ;

	va_start(ap, type);
	if (type == BYTE_STRING) {
		str = va_arg(ap, char*);
		for (i =1;i < strlen(str)-1;i++) {
			*((char*)&memory[pc]+p) = str[i];
			p++;
			if (p == 2) {
				p = 0;
				pc++;
				memory[pc] = 0;
			}
		}
		
	} else if (type == NUMBER) {
		num = va_arg(ap, int);		
		*((char*)&memory[pc]+p) = num & 0xff;
		p++;
		if (p == 2) {
			p = 0;
			pc++;
			memory[pc] = 0;
		}
		
	}
	va_end(ap);


	*((char*)(&memory[pc]+p)) = 
	printf ("push type %d\n",type);
}

/**
 * Created  10/19/2022                         
 *                                                   1    2    
 *                                                        p    
 * @brief   encodes this format 00000jjj00000000 -> done  j
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */

void encode_done(enum OPCODES a1 ,...) {
	char instruction[16];
	int a2;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction, get_instruction_pattern(a1), 16);
	modify_instruction_string(instruction, a2 , 'j'); 
	instruction_string_to_word(instruction);



}


/**
 * Created  11/03/2022
 * @brief   locates instruction for a giver opode
 * @param   
 * @return  
 */
struct instruction_s * find_instruction(unsigned short op0) {
	struct instruction_s * inst = instruction_set;
	int i;
	unsigned short op;

	while (inst->opcode) {
		op = op0;
		for (i = 0;i < 16; i++) {
			if (inst->instruction[i] != ((op&0x8000) ?  '1' : '0')  && (inst->instruction[i]=='0' ||  inst->instruction[i]=='1') ) {
				 break;
			}
			op = op << 1;
		}
		if (i==16)
			return inst;
		inst++;
	}
	return 0;
}

/**
 * Created  10/29/2022
 * @brief   this functions converts binary bytes into assembly code
 * @param   
 * @return  
 */
void decode() {
	struct instruction_s *inst;
	char *label_name;

	for (int i = 0;i < pc; i ++) {
		 label_name = label__get_by_pc(i);
		 if (label_name)
			 printf("%s:\n", label_name);
		 inst =  find_instruction(memory[i]);
		 if (inst && inst->decode) { 
			 inst->decode(i, inst);
		 }

	}

	for (int i=0;i<pc;i++)
		printf("%x,",memory[i]);
	printf("\n");

}



void encode(char *asm_file1) {	
	char *asm_file = "start11:;ldi r0, 4;loop exit, 0;start:;st r4, (r5, 0);addi r5, 1;rorl r4;addi r4, 0x10;exit:;done 3;addi r4, 0x40;ldi r3, 0;cmpeqi r3, 0 ;bt start;mov r12,r15;data:; .byte \"hello\",0,0x12,0x34  ;";

//        char *asm_file = "start11:;ldi r0, 4;loop exit, 0;start:;st r4, (r5, 0);addi r5, 1;rorl r4;addi r4, 0x10;exit:;done 3;addi r4, 0x40;ldi r3, 0;cmpeqi r3, 0 ;bt start;mov r12,r15; data: .byte \"hello world\" ; \n";
	
	//char *asm_file = "start11: ; ldi r0, 4; ; \n";
	pc = 0;
	struct yy_buffer_state *my_string_buffer0 = yy_scan_string(asm_file); 
	yyparse(); 
	yy_delete_buffer(my_string_buffer0);
	pc++;

}

int main() {

#if 0
	memory[0]=0x804;
	memory[1]=0x7803;
	memory[2]=0x5c05;
	memory[3]=0x1d01;
	memory[4]=0x1c10;
	memory[5]=0x300;
	memory[6]=0x1c40;
	memory[7]=0xb00;
	memory[8]=0x4b00;
	memory[9]=0x7df8;
 	 pc=10;	
#else
	encode(0);
#endif


	decode();
	decode();
	
	return 0;

}
