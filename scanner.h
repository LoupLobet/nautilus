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

#define L_EOF 0 
#define L_ERROR 258 
#define L_DOT 259 
#define L_SEMICOLON 260 
#define L_EQUAL 261 
#define L_LEFTCURLY 262
#define L_RIGHTCURLY 263
#define L_LEFTBRACKET 264 
#define L_RIGHTBRACKET 265 
#define L_LEFTBRACE 266 
#define L_RIGHTBRACE 267 
#define L_COMMA 268 
#define L_LESSTHAN 269 
#define L_GREATERTHAN 270 
#define L_ADD 271 
#define L_SUB 272 
#define L_STAR 273 
#define L_DIV 274
#define L_IF 276 
#define L_ELSE 277
#define L_NOTEQUAL 279
#define L_OR 280
#define L_AND 283
#define L_MOD 285
#define L_NOT 286
#define L_WHILE 293
#define L_FOR 294
#define L_RETURN 296
#define L_REALNUM 298
#define L_FUNCTION 299
#define L_STRINGCONST 301
#define L_INTCONST 302
#define L_REALCONST 303
#define L_IDENTIFIER 304
#define L_TYPEINT 305
#define L_TYPEFLOAT 306
#define L_TYPESTRING 307

#define NB_SYMBOLS 33

extern YYVAL yyval;
extern YYPOS yypos;

#endif
