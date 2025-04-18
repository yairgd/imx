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


\"[^\"]*\"    { col+=strlen(yytext);strncpy (yylval.string, yytext,16 );return BYTE_STRING; }

[0-9]+     	 {col+=strlen(yytext);yylval.iValue = atoi(yytext);return NUMBER;};
0x[0-9 a-f]+     {col+=strlen(yytext);  yylval.iValue = strtol(yytext, NULL, 16);return NUMBER;};

r[0-9]+   	 {col+=strlen(yytext);yylval.iValue = atoi(yytext+1);return REG_NUMBER;};


".byte" {col+=strlen(yytext);yylval.iValue = BYTE;return BYTE;};
".word" {col+=strlen(yytext);yylval.iValue = WORD;return WORD;};
".dword" {col+=strlen(yytext);yylval.iValue = DWORD;return DWORD;};


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
"jmpr"   OPCODE_FUNC(OP_RORL)
"jsrr"   OPCODE_FUNC(OP_RORL)
"ldrpc"  OPCODE_FUNC(OP_RORL)
"revb"   OPCODE_FUNC(OP_RORL)
"revblo" OPCODE_FUNC(OP_RORL)
"rorb"   OPCODE_FUNC(OP_RORL)
"rorl"   OPCODE_FUNC(OP_RORL)
"lsrl"   OPCODE_FUNC(OP_RORL)
"arsl"   OPCODE_FUNC(OP_RORL)
"lsll"   OPCODE_FUNC(OP_RORL)


[a-zA-Z0-9]+  {col+=strlen(yytext);strncpy (yylval.string, yytext,16 );yylval.string[MIN(strlen(yytext),15)]=0; return LABEL;} 





	/*[0-9]     {
	   col+=strlen(yytext);
            c = yytext[0];
            yylval.a = c - '0';
            return(DIGIT);
          }*/
