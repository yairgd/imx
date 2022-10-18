%{
#define OP_ANDI 456

#include <stdio.h>
#include "y.tab.h"
int c;
extern int line;
extern int col;
//extern int yylval;
%}
%%


[ \t\r]+ ; /* ignore whitespace */
"//".*           { return NOTE1; }
"/*"[^*/]*"*/"      { return NOTE2; }

[-()<>=+*/;{}.:,] {return *yytext;}
[\n] {line++;col=0;return *yytext;}

[0-9]+     {col+=strlen(yytext);yylval.iValue = atoi(yytext);return NUMBER;};
r[0-9]+    {col+=strlen(yytext);yylval.iValue = atoi(yytext+1);return REG_NUMBER;};


	/* [a-z]+  {col+=strlen(yytext);strncpy (yylval.opcode, yytext,16);return OPCODE;} */

andi  {col+=strlen(yytext);yylval.opcode=OP_ANDI;return OPCODE;}

e[0-9]     {
	   col+=strlen(yytext);
            c = yytext[0];
            yylval.a = c - '0';
            return(DIGIT);
          }
