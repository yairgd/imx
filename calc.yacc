%{
#include <stdio.h>
#include "instructions.h"


int regs[26];
int base;
int yydebug=1;
int line;
int col;

%}

%union {
      int iValue;
      int a;
      int opcode;
      char string[16];
};


%start list 

%token <opcode> OPCODE  
%token <iValue> NUMBER REG_NUMBER DIGIT NOTE1 NOTE2
%token  <string>     LABEL 

%type  <a>     number 



%%   /* beginning of rules section */

list:    /*empty */
         |
        list stat '\n'
         |
        list stat ';'
	 |
	list stat NOTE1 '\n'
	 |
	list stat NOTE2 '\n'
	 |
	list stat ';' NOTE1  '\n'
	 |
	list stat ';' NOTE2  '\n'
	 |
        list error '\n'
         {
           yyerrok;
         }
         ;
stat: 	OPCODE REG_NUMBER ',' NUMBER 
	{
		printf ("opcode %d %d,%d \n",$1,$2,$4);
	}
	|
	OPCODE REG_NUMBER ',' '(' REG_NUMBER ',' NUMBER ')' 
	{
		printf ("opcode %d r%d,(r%d , %d)\n",$1,$2,$5,$7);
	}
	|
	OPCODE NUMBER
	{
		printf ("opcode %d %d\n",$1,$2);
	}
	|
	LABEL 
	{
		printf ("label %s \n",$1);
	}

	OPCODE LABEL ',' NUMBER
	{
		printf ("opcode %d   %d)\n",$1,$4);

	}


	;


number:  DIGIT
         {
           $$ = $1;
           base = ($1==0) ? 8 : 10;
         }       
	 |
         number DIGIT
         {
           $$ = base * $1 + $2;
         }
         ;
%%



 yyerror(s)
char *s;
{
  fprintf(stderr, "%s, col:%d, line %d\n",s,col,line);
}

yywrap()
{
  return(1);
}
