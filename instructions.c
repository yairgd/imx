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




int main() {
 return(yyparse());
	
}
