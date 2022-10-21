%{
#include "instructions.h"

#include <stdio.h>
#include "y.tab.h"
int c;
extern int line;
extern int col;
#define MIN(a,b) ((a)<(b) ? (a) : (b))
//extern int yylval;
%}
%%


[ \t\r]+ ; /* ignore whitespace */
"//".*           { return NOTE1; }
"/*"[^*/]*"*/"      { return NOTE2; }

[-()<>=+*/;{}.,:] {return *yytext;}
[\n] {line++;col=0;return *yytext;}

[0-9]+     {col+=strlen(yytext);yylval.iValue = atoi(yytext);return NUMBER;};
0x[0-9 a-f]+     {col+=strlen(yytext);  yylval.iValue = strtol(yytext, NULL, 16);return NUMBER;};

r[0-9]+    {col+=strlen(yytext);yylval.iValue = atoi(yytext+1);return REG_NUMBER;};


"andi"  {col+=strlen(yytext);yylval.opcode=OP_ANDI;return OPCODE;}
"loop"  {col+=strlen(yytext);yylval.opcode=OP_LOOP;return OPCODE;}
"st"  {col+=strlen(yytext);yylval.opcode=OP_ST;return OPCODE;}
"ldi"  {col+=strlen(yytext);yylval.opcode=OP_LDI;return OPCODE;}
"done"  {col+=strlen(yytext);yylval.opcode=OP_DONE;return OPCODE;}
"cmpeqi"  {col+=strlen(yytext);yylval.opcode=OP_CMPEQI;return OPCODE;}
"addi"  {col+=strlen(yytext);yylval.opcode=OP_ADDI;return OPCODE;}
"bt"  {col+=strlen(yytext);yylval.opcode=OP_BT;return OPCODE;}

"mov"  {col+=strlen(yytext);yylval.opcode=OP_MOV;return OPCODE;}
"add"  {col+=strlen(yytext);yylval.opcode=OP_ADD;return OPCODE;}
"sub"  {col+=strlen(yytext);yylval.opcode=OP_SUB;return OPCODE;}
"or"  {col+=strlen(yytext);yylval.opcode=OP_OR;return OPCODE;}
"andn"  {col+=strlen(yytext);yylval.opcode=OP_ANDN;return OPCODE;}
"and"  {col+=strlen(yytext);yylval.opcode=OP_AND;return OPCODE;}
"cmpeq"  {col+=strlen(yytext);yylval.opcode=OP_CMPEQ;return OPCODE;}
"cmplt"  {col+=strlen(yytext);yylval.opcode=OP_CMPLT;return OPCODE;}
"cmphs"  {col+=strlen(yytext);yylval.opcode=OP_CMPHS;return OPCODE;}


[a-zA-Z0-9]+  {col+=strlen(yytext);strncpy (yylval.string, yytext,16 );yylval.string[MIN(strlen(yytext),15)]=0; return LABEL;} 





	/*[0-9]     {
	   col+=strlen(yytext);
            c = yytext[0];
            yylval.a = c - '0';
            return(DIGIT);
          }*/
