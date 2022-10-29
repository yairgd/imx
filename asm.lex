%{
#include "instructions.h"

#include <stdio.h>
#include "y.tab.h"
int c;
extern int line;
extern int col;
extern char op_str[16];
#define MIN(a,b) ((a)<(b) ? (a) : (b))
#define OPCODE_FUNC(x) {col+=strlen(yytext);yylval.opcode=(x);strcpy (op_str,yytext);return OPCODE;}
%}
%%


[ \t\r]+ ; /* ignore whitespace */
"//".*           { return NOTE1; }
"/*"[^*/]*"*/"      { return NOTE2; }

[-()<>=+*/;{}.,:] {return *yytext;}
[\n] {line++;col=0;return *yytext;}


[0-9]+     	 {col+=strlen(yytext);yylval.iValue = atoi(yytext);return NUMBER;};
0x[0-9 a-f]+     {col+=strlen(yytext);  yylval.iValue = strtol(yytext, NULL, 16);return NUMBER;};

r[0-9]+   	 {col+=strlen(yytext);yylval.iValue = atoi(yytext+1);return REG_NUMBER;};


"andi"   OPCODE_FUNC(OP_ANDI)
"loop"   OPCODE_FUNC(OP_LOOP)
"st"     OPCODE_FUNC(OP_ST)
"ldi"    OPCODE_FUNC(OP_LDI)
"done"   OPCODE_FUNC(OP_DONE)
"cmpeqi" OPCODE_FUNC(OP_CMPEQI)
"addi"   OPCODE_FUNC(OP_ADDI)
"bt"     OPCODE_FUNC(OP_BT)
"mov"    OPCODE_FUNC(OP_MOV)
"add"    OPCODE_FUNC(OP_ADD)
"sub"    OPCODE_FUNC(OP_SUB)
"or"     OPCODE_FUNC(OP_OR)
"andn"   OPCODE_FUNC(OP_ANDN)
"and"    OPCODE_FUNC(OP_AND)
"cmpeq"  OPCODE_FUNC(OP_CMPEQ)
"cmplt"  OPCODE_FUNC(OP_CMPLT)
"cmphs"  OPCODE_FUNC(OP_CMPHS)


[a-zA-Z0-9]+  {col+=strlen(yytext);strncpy (yylval.string, yytext,16 );yylval.string[MIN(strlen(yytext),15)]=0; return LABEL;} 





	/*[0-9]     {
	   col+=strlen(yytext);
            c = yytext[0];
            yylval.a = c - '0';
            return(DIGIT);
          }*/
