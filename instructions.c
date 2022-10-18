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
//http://blog.petri.us/sdma-hacking/part-1.html
struct instruction {
	char *opcode;
	char instruction[16];
}  instruction_set[] = {
	{.opcode = "mov"  , .instruction="00000rrr10001sss"},

	{.opcode = "andi" , .instruction="00111rrriiiiiiii"}, /* ldi r,i */
	{.opcode = "ldi"  , .instruction="00111rrriiiiiiii"},
	{.opcode = "addi" , .instruction="00111rrriiiiiiii"}, 
	{.opcode = "cmpeqi",.instruction="00111rrriiiiiiii"}, 
	{.opcode = "xori" , .instruction="00111rrriiiiiiii"}, 

	{.opcode = "loop" , .instruction="011110ffnnnnnnnn"}, /* loop n,ff*/

	{.opcode = "st"   , .instruction="01011rrrdddddbbb"},   /* st r,(d,b)*/
	{.opcode = "ld"   , .instruction="01011rrrdddddbbb"},   /* ld r,(d,b)*/

	{.opcode = "done"   , .instruction="00000jjj00000000"},   /* done jjj*/

	{.opcode = "bt"   , .instruction="01111101pppppppp"},   /* bt pppp*/
	{.opcode = "bf"   , .instruction="01111100pppppppp"},   /* bf pppp*/
	{.opcode = "bsf"  , .instruction="01111110pppppppp"},   /* bsf pppp*/
	{.opcode = "bff"  , .instruction="01111111pppppppp"},   /* bff pppp*/

};

/*
 

   */
/*
                              | start:
0000 0804 (0000100000000100) |     ldi r0, 4
0001 7803 (0111100000000011) |     loop exit, 0
0002 5c05 (0101110000000101) |     st r4, (r5, 0) # Address r5
0003 1d01 (0001110100000001) |     addi r5, 1
0004 1c10 (0001110000010000) |     addi r4, 0x10
                             | exit:
0005 0300 (0000001100000000) |     done 3
0006 1c40 (0001110001000000) |     addi r4, 0x40
0007 0b00 (0000101100000000) |     ldi r3, 0
0008 4b00 (0100101100000000) |     cmpeqi r3, 0 # Always true
0009 7df6 (0111110111110110) |     bt start # Always branches

------------ CUT HERE -----------

static const int sdma_code_length = 5;
static const u32 sdma_code[5] = {
 0x08047803, 0x5c051d01, 0x1c100300, 0x1c400b00, 0x4b007df6,
};

    ldi r0, 4;loop exit, 0;st r4, (r5, 0);addi r5, 1;addi r4, 0x10;exit:;done 3;addi r4, 0x40;ldi r3, 0;cmpeqi r3, 0 ;bt start

*/



int main() {
 return(yyparse());
	
}
