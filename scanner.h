#ifndef __SCANNER_H_
#define __SCANNER_H_

typedef union {
	int ival;
	double dval;
} YYVAL;

typedef struct {
	int firstcolumn;
	int firstline;
	int lastcolumn;
	int lastline;
} YYPOS;

#define T_EOF 0 
#define T_ERROR 258 
#define T_DOT 259 
#define T_SEMICOLON 260 
#define T_EQUAL 261 
#define T_LEFTCURLY 262
#define T_RIGHTCURLY 263
#define T_LEFTBRACKET 264 
#define T_RIGHTBRACKET 265 
#define T_LEFTBRACE 266 
#define T_RIGHTBRACE 267 
#define T_COMMA 268 
#define T_LESSTHAN 269 
#define T_GREATERTHAN 270 
#define T_ADD 271 
#define T_SUB 272 
#define T_STAR 273 
#define T_DIV 274
#define T_IF 276 
#define T_ELSE 277
#define T_NOTEQUAL 279
#define T_OR 280
#define T_AND 283
#define T_MOD 285
#define T_NOT 286
#define T_WHILE 293
#define T_FOR 294
#define T_RETURN 296
#define T_REALNUM 298
#define T_FUNCTION 299
#define T_STRINGCONST 301
#define T_INTCONST 302
#define T_REALCONST 303
#define T_IDENTIFIER 304
#define T_TYPEINT 305
#define T_TYPEFLOAT 306
#define T_TYPESTRING 307
#define T_NAUTILUS 308

#define NB_SYMBOLS 33

extern YYVAL yyval;
extern YYPOS yypos;

#endif
