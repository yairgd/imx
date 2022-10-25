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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
//#include "y.tab.h"

//http://blog.petri.us/sdma-hacking/part-1.html


static unsigned short memory[4096];
static int pc = 0;

static struct label {
	unsigned long hash;
	int pc;
} lables[256]={0};
 static int label_idx = 0;



struct instruction_s   instruction_set[] = {

	[OP_ANDI]   = {.instruction="00111rrriiiiiiii", .decode = decode_5x3r38i}, /* ldi r,i */
	[OP_LDI]    = {.instruction="00001rrriiiiiiii", .decode = decode_5x3r38i},
	[OP_ADDI]   = {.instruction="00011rrriiiiiiii", .decode = decode_5x3r38i}, 
	[OP_CMPEQI] = {.instruction="01001rrriiiiiiii", .decode = decode_5x3r38i}, 
	[OP_XORI]   = {.instruction="00010rrriiiiiiii", .decode = decode_5x3r38i}, 

	[OP_LOOP]   = {.instruction="011110ffnnnnnnnn", .decode = decode_loop}, /* loop n,ff*/

	[OP_ST]     = {.instruction="01011rrrdddddbbb", .decode = decode_5x3r5d3b},   /* st r,(d,b)*/
	[OP_LD]     = {.instruction="01010rrrdddddbbb", .decode = decode_5x3r5d3b},   /* ld r,(d,b)*/

	[OP_DONE]   = {.instruction="00000jjj00000000", .decode = decode_done},   /* done jjj*/

	[OP_BT]     = {.instruction="01111101pppppppp", .decode = decode_8x8p},   /* bt pppp*/
	[OP_BF]     = {.instruction="01111100pppppppp", .decode = decode_8x8p},   /* bf pppp*/
	[OP_BSF]    = {.instruction="01111110pppppppp", .decode = decode_8x8p},   /* bsf pppp*/
	[OP_BDF]    = {.instruction="01111111pppppppp", .decode = decode_8x8p},   /* bff pppp*/


	[OP_MOV]    = {.instruction="00000rrr10001sss", .decode = decode_5x3r5x3s},   /* mov r,s*/
	[OP_ADD]    = {.instruction="00000rrr10010sss", .decode = decode_5x3r5x3s},   /* add r,s*/
	[OP_SUB]    = {.instruction="00000rrr10011sss", .decode = decode_5x3r5x3s},   /* mov r,s*/
	[OP_OR]	    = {.instruction="00000rrr10101sss", .decode = decode_5x3r5x3s},   /* mov r,s*/
	[OP_ANDN]   = {.instruction="00000rrr10110sss", .decode = decode_5x3r5x3s},   /* mov r,s*/
	[OP_AND]    = {.instruction="00000rrr10111sss", .decode = decode_5x3r5x3s},   /* mov r,s*/
	[OP_CMPEQ]  = {.instruction="00000rrr11001sss", .decode = decode_5x3r5x3s},   /* mov r,s*/
	[OP_CMPLT]  = {.instruction="00000rrr11010sss", .decode = decode_5x3r5x3s},   /* mov r,s*/
	[OP_CMPHS]  = {.instruction="00000rrr11011sss", .decode = decode_5x3r5x3s},   /* mov r,s*/




	[OP_JMPR]    = {.instruction="00000rrr00001000", .decode = decode_3r},   /* jumpr r*/
	[OP_JSRR]    = {.instruction="00000rrr00001001", .decode = decode_3r},   /* jsrr r*/
	[OP_LDRPC]   = {.instruction="00000rrr00001010", .decode = decode_3r},   /* ldrpc r*/
	[OP_REVB]    = {.instruction="00000rrr00010000", .decode = decode_3r},   /* rev r */
	[OP_REVBLO]  = {.instruction="00000rrr00010001", .decode = decode_3r},   /* revblo r */
	[OP_RORB]    = {.instruction="00000rrr00010010", .decode = decode_3r},   /* rorb r */
	[OP_RORL]    = {.instruction="00000rrr00010100", .decode = decode_3r},   /* jmpr r */
	[OP_LSRL]    = {.instruction="00000rrr00010101", .decode = decode_3r},   /* jmpr r */
	[OP_ASRL]    = {.instruction="00000rrr00010110", .decode = decode_3r},   /* jmpr r */
	[OP_LSLL]    = {.instruction="00000rrr00010111", .decode = decode_3r},   /* jmpr r */
	

	[OP_BCLRI]    = {.instruction="00000rrr001nnnnn", .decode = decode_3r5n},   /* bclri r,n */
	[OP_BSETI]    = {.instruction="00000rrr010nnnnn", .decode = decode_3r5n},   /* nseti r,n */
	[OP_BTSTI]    = {.instruction="00000rrr011nnnnn", .decode = decode_3r5n},   /* btsti r,n */

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
void push_label(char *label) {
	lables[label_idx].pc = pc;
	lables[label_idx++].hash = hash((unsigned char*)label);

}

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



decode_t get_decode_function(enum OPCODES op) {
	return instruction_set[op].decode;
}
/**
 * Created  10/19/2022                         
 *                                                   1 2 3 4
 *                                                     r   i 
 * @brief   decodes this format 00111rrriiiiiiii -> op a , b 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void decode_5x3r38i(enum OPCODES a1, ...) {
	char instruction[16];
	int a2,a4;
   	va_list ap;

	memcpy (instruction,instruction_set[a1].instruction,16);

	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a4 = va_arg(ap, int);
	va_end(ap);



	modify_instruction_string(instruction, a2 , 'r'); 
	modify_instruction_string(instruction, a4 , 'i'); 
	instruction_string_to_word(instruction);

	printf ("opcode %d %d,%d \n",a1,a2,a4);
	

}


/**
 * Created  10/19/2022                         
 *                                                      1   2  3
 *                                                          r  i 
 * @brief   decodes this format 00000rrr001nnnnn ->  blrci r1, i 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void decode_3r5n(enum OPCODES a1, ...) {
	char instruction[16];
	int a2, a3;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a3 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,instruction_set[a1].instruction,16);
	modify_instruction_string(instruction, a2 , 'r'); 
	modify_instruction_string(instruction, a3 , 'i'); 
	instruction_string_to_word(instruction);

	printf ("opcode %d",a2);

}


/**
 * Created  10/19/2022                         
 *                                                      1  2  
 *                                                         r   
 * @brief   decodes this format 00000rrr00001000 -> jumpr r1 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void decode_3r(enum OPCODES a1, ...) {
	char instruction[16];
	int a2;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,instruction_set[a1].instruction,16);
	modify_instruction_string(instruction, a2 , 'r'); 
	instruction_string_to_word(instruction);

	printf ("opcode %d",a2);

}


/**
 * Created  10/19/2022                         
 *                                                   1 2  3 4 5  6 7 8
 *                                                      r     b    d
 * @brief   decodes this format 01011rrrdddddbbb -> st r1 , ( r2 , 2 ) 
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void decode_5x3r5d3b(enum OPCODES a1, ...) {
	char instruction[16];
	int a2,a5,a7;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a5 = va_arg(ap, int);
	a7 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,instruction_set[a1].instruction,16);
	modify_instruction_string(instruction, a2 , 'r'); 
	modify_instruction_string(instruction, a5 , 'b'); 
	modify_instruction_string(instruction, a7 , 'd'); 
	instruction_string_to_word(instruction);


	printf ("opcode %d r%d,(r%d , %d)\n",a1,a2,a5,a7);

}

/**
 * Created  10/19/2022                         
 *                                                   1  2    3 
 *                                                      r    s
 * @brief   decodes this format 00000rrr10001sss -> mov r1 , r2
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void decode_5x3r5x3s(enum OPCODES a1 ,...) {
	char instruction[16];
	int a2,a4;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	a4 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,instruction_set[a1].instruction,16);
	printf ("opcode %d %d,%d \n",a1,a2,a4);
}


/**
 * Created  10/19/2022                         
 *                                                   1  2    
 *                                                      p    
 * @brief   decodes this format 01111101pppppppp -> bt label
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void decode_8x8p(enum OPCODES a1,...) {
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
	

	memcpy (instruction,instruction_set[a1].instruction,16);
	modify_instruction_string(instruction, p - pc - 1 , 'p'); 
	instruction_string_to_word(instruction);
	printf ("opcode %d   %s\n",a1,a2);

}


/**
 * Created  10/19/2022                         
 *                                                   1    2     4
 *                                                        p     ff
 * @brief   decodes this format 011110ffnnnnnnnn -> loop label, ff
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */
void decode_loop(enum OPCODES a1 ,...) {
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
		printf("label %s not wans not defined\n",a2);
	}
	
	memcpy (instruction,instruction_set[a1].instruction,16);
	modify_instruction_string(instruction, pc - p + 1, 'n'); 
	modify_instruction_string(instruction, a4 , 'f'); 
	instruction_string_to_word(instruction);


	printf ("opcode %d   %s\n",a1,a2);

}

/**
 * Created  10/19/2022                         
 *                                                   1    2    
 *                                                        p    
 * @brief   decodes this format 00000jjj00000000 -> done  j
 * r=a2, i=a4
 * example for andi r2,12
 * a1=OP_ANDI, a2=2  , a4=12 -> OP_ANDI 2,12
 * @param   
 * @return  
 */

void decode_done(enum OPCODES a1 ,...) {
	char instruction[16];
	int a2;
   	va_list ap;


	va_start(ap, a1);
	a2 = va_arg(ap, int);
	va_end(ap);

	memcpy (instruction,instruction_set[a1].instruction,16);
	modify_instruction_string(instruction, a2 , 'j'); 
	instruction_string_to_word(instruction);



}






int main() {
	
	struct yy_buffer_state *my_string_buffer0 = yy_scan_string("start:;ldi r0, 4;loop exit, 0;st r4, (r5, 0);addi r5, 1;addi r4, 0x10;exit:;done 3;addi r4, 0x40;ldi r3, 0;cmpeqi r3, 0 ;bt start;mov r12,r15\n"); 
	yyparse(); 
	yy_delete_buffer(my_string_buffer0);


	struct yy_buffer_state *my_string_buffer1 = yy_scan_string("start:;ldi r0, 4;loop exit, 0;st r4, (r5, 0);addi r5, 1;addi r4, 0x10;exit:;done 3;addi r4, 0x40;ldi r3, 0;cmpeqi r3, 0 ;bt start;mov r12,r15\n"); 
	yyparse(); 
	yy_delete_buffer(my_string_buffer1);



	for (int i=0;i<pc;i++)
		printf("%x,",memory[i]);
	printf("\n");

	return 0;

}
