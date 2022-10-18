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
	char *operand;
	char instruction[16];
}  instruction_set[] = {
	{.opcode = "mov",.operand="r,i" , .instruction="00000rrr10001sss"},
};



int main() {
 return(yyparse());
	
}
