%{
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "scanner.h"

int column = 0;
int line = 0;

YYPOS yypos;
YYVAL yyval;

int
yyerror(const char *s)
{
	printf("error: %s\n", s);
	return 0;
}

long
estrtol(const char *nptr, int base)
{
	char *endptr = NULL;
	errno = 0;
	long out;

	out = strtol(nptr, &endptr, base);
	if ((endptr == nptr) || ((out == LONG_MAX || out == LONG_MIN)
	&& errno == ERANGE)) {
		return LONG_MAX;
	}
	return out;
}

double
estrtod(const char *nptr)
{
	char *endptr = NULL;
	errno = 0;
	double out;

	out = strtod(nptr, &endptr);
	if ((endptr == nptr) || ((out == HUGE_VAL || out == 0 - HUGE_VAL)
	&& errno == ERANGE)) {
		return HUGE_VAL;
	}
	return out;
}

%}

%option yylineno
%option outfile = "scanner.c"
%option nounput
%option noinput
%option noyywrap

DIGIT [0-9]
%%
[ \t]+			column += yyleng;

\. 			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_DOT;
			}
;			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_SEMICOLON;
			}
=			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_EQUAL;
			}
\{			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_LEFTCURLY;
			}
\}			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_RIGHTCURLY;
			}
\(			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_LEFTBRACE;
			}
\)			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_RIGHTBRACE;
			}
\[			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_LEFTBRACKET;
			}
\]			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_RIGHTBRACKET;
			}
,			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_COMMA;
			}
\<			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_LESSTHAN;
			}
\>			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_GREATERTHAN;
			}
\+			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_ADD;
			}
\-			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_SUB;
			}
\*			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_STAR;
			}
\@			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_NAUTILUS;
			}
\/			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_DIV;
			}
if			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_IF;
			}
else			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_ELSE;
			}
"!="			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_NOTEQUAL;
			}
"||"			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_OR;
			}
"&&"			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_AND;
			}
!			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_NOT;
			}
\%			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_MOD;
			}
while			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_WHILE;
			}
for			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_FOR;
			}
return			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_RETURN;
			}
func			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_FUNCTION;
			}
int			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_TYPEINT;
			}
float			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_TYPEFLOAT;
			}
string			{
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_TYPESTRING;
			}
[0-9]+			{
				/* litteral integer constant */
				long val;
				
				if ((val = estrtol(yytext, 10)) == LONG_MAX)
					yyerror("Integer out of range");
				yyval.ival = val;
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_INTCONST;
			}
(({DIGIT}+\.|\.{DIGIT}+|{DIGIT}+\.{DIGIT}+)(e[+-]?{DIGIT}+)?)|({DIGIT}+e[+-]?{DIGIT}+) {
				/* litteral integer constant */
				double val;

				if ((val = estrtod(yytext)) == HUGE_VAL)
					yyerror("Floating point number out of range");
				yyval.dval = val;
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
                		column += yyleng;
				return T_REALCONST;
        		}

\"([^\"\n]|\"\")*[\"\n]	{
				/* litteral string constant */
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				if (strchr(yytext, '\n')) {
					yyerror("Newline in string");
					column = 0;
				} else {
					/* here need to insert string in symbol table */
					return T_STRINGCONST;
				}
			}
[a-zA-Z_][a-zA-Z0-9_]*	{
				/* here need to insert identifier in symbol table */
				yypos.firstline = yylineno;
				yypos.firstcolumn = column;
				column += yyleng;
				return T_IDENTIFIER;
			}
\n			{
				column = 0;
			}
.			yyerror("Illegal symbol");

<<EOF>>			yyterminate();

