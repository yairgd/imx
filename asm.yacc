%{
#include <stdio.h>
#include "instructions.h"


int regs[26];
int base;
int yydebug=1;
int line;
int col;
char op_str[16];

%}

%union {
      int iValue;
      int a;
      int opcode;
      char string[16];
};


%start list 

%token <opcode> OPCODE  
%token <iValue> NUMBER REG_NUMBER DIGIT NOTE1 NOTE2 BYTE WORD DWORD STRING
%token  <string>     LABEL  BYTE_STRING 

	/*%type  <a>     number */



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
	 
byte_list: /* empty */
	|
	NUMBER
	{
		push_to_memory(NUMBER,$1);		
//		printf("a %d \n", $1);	
	}
	|
	BYTE_STRING
	{
		push_to_memory(BYTE_STRING,$1);	
//		printf("b %s \n", $1);			
	}
	|
	byte_list ',' NUMBER
	{
		push_to_memory(NUMBER,$3);		
	//	printf("c %d \n", $3);			
	}
	|
	byte_list ',' BYTE_STRING
	{
		push_to_memory(BYTE_STRING,$3);	
	//	printf("d %s \n", $3);			
	}
	;

stat: 	OPCODE REG_NUMBER ',' NUMBER 
	{
		//encode_5x3r38i($1, $2,  $4);
		/*printf ("opcode %d %d,%d \n",$1,$2,$4);*/
		encode_t f = get_encode_function($1, op_str);
		f($1, $2,$4);

	//	printf("!!!!!!!!!!!!!!!!1 %s ",op_str);

	}
	|
	OPCODE REG_NUMBER ',' REG_NUMBER 
	{
		//encode_5x3r5x3s($1,$2,$4);
		/*printf ("opcode %d %d,%d \n",$1,$2,$4);*/
		encode_t f = get_encode_function($1, op_str);
		f($1, $2,$4);

	}
	|
	OPCODE REG_NUMBER ',' '(' REG_NUMBER ',' NUMBER ')' 
	{
		encode_5x3r5d3b($1,$2,$5,$7);
		/*printf ("opcode %d r%d,(r%d , %d)\n",$1,$2,$5,$7);*/
	}
	|
	OPCODE REG_NUMBER 
	{
		encode_3r($1,$2);
	}
	|
	OPCODE NUMBER
	{
		//printf ("opcode %d %d\n",$1,$2);
		encode_t f = get_encode_function($1, op_str);
		f($1, $2);
	}
	|
	LABEL ':' 
	{
		//printf ("label %s \n",$1);
		push_label($1, -1);
	}
	|
	OPCODE LABEL ',' NUMBER
	{
		//printf ("opcode %d   %d\n",$1,$4);
		//encode_loop($1, $2,$4);
		encode_t f = get_encode_function($1, op_str);
		f($1, $2,$4);


	}
	|
	OPCODE LABEL 
	{
		encode_t f = get_encode_function($1, op_str);
		f($1, $2,0);
		//encode_loop($1, $2,0);
		//printf ("opcode %d   %s\n",$1,$2);

	}
	|
	BYTE  byte_list
	{
//		 push_to_memory($1,$2);
	}
	
	;

	/*
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
	*/


%%



 int yyerror(s)
char *s;
{
  fprintf(stderr, "%s, col:%d, line %d\n",s,col,line);
  return 0;
}

int yywrap()
{
  return(1);
}
