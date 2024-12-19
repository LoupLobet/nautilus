#ifndef __SCANNER_H_
#define __SCANNER_H_

typedef union {
	int ival;
	double dval;
	char cval;
} YYVAL;

typedef struct {
	int firstcolumn;
	int firstline;
	int lastcolumn;
	int lastline;
} YYPOS;

enum LEXEME {
	L_EOF = 0,       // *EOF*
	L_ERROR = 258,   // *error*
	// separators
	L_DOT,           // . 
	L_SEMICOLON,     // ;
	L_COMMA,         // ,
	// brackets
	L_LEFTCURLY,     // {
	L_RIGHTCURLY,    // }
	L_LEFTBRACKET,   // [
	L_RIGHTBRACKET,  // ]
	L_LEFTBRACE,     // (
	L_RIGHTBRACE,    // )
	// binary op
	L_ASSIGN,        // =
	L_ADD,           // +
	L_SUB,           // -
	L_STAR,          // *
	L_DIV,           // /
	L_MOD,           // %
	// unary op
	L_NOT,           // !
	L_INC,           // ++
	L_DEC,           // --
	// binary relations
	L_EQ,            // ==
	L_NEQ,           // !=
	L_OR,            // ||
	L_AND,           // &&
	L_LT,            // <
	L_GT,            // >
	L_LEQ,           // <=
	L_GEQ,           // >=
	// statements
	L_IF,            // if
	L_ELSE,          // else
	L_WHILE,         // while
	L_FOR,           // for
	L_RETURN,        // return
	// consts
	L_CHARCONST,     // e.g. 'n'
	L_INTCONST,      // e.g. 123
	L_REALCONST,     // e.g. 0.123
	L_IDENTIFIER,    // e.g. myvar
	// types
	L_TYPECHAR,      // char
	L_TYPEINT,       // int
	L_TYPEFLOAT,     // float
	// misc
	L_FUNCTION,      // func
};

extern YYVAL yyval;
extern YYPOS yypos;

#endif
