#ifndef __AST_H__
#define __AST_H__

/*
 * <PROGRAM>               ::= *<EXTERNAL_DECLARATION>
 * <EXTERNAL_DECLARATION>  ::= <FUNC_DECLARATION> | <FUNC_DEFINITION> | <VAR_DECLARATION> | <VAR_DEFINITION>
 * <STATEMENT>             ::= <IF> | <ELSE> | <RETURN> | <WHILE> | <FOR> | <INSTRUCTION> | <BLOCK> | <VAR_DECLARATION> | <VAR_DEFINITION>
 * <INSTRUCTION>           ::= <EXPRESSION> ";"
 * <RETURN>                ::= "return" <EXPRESSION>
 * <IF>                    ::= "if" "(" <EXPRESSION> ")" <BLOCK>
 * <ELSE>                  ::= "else" <BLOCK>
 * <ELSE_IF>               ::= "else" "if" "(" <EXPRESSION> ")" <BLOCK
 * <WHILE>                 ::= "while" "(" <EXPRESSION> ")" <BLOCK>
 * <FOR>                   ::= "for" "(" <EXPRESSION> ";" <EXPRESSION> ";" <EXPRESSION> ")" <BLOCK>
 * <FUNC_DECLARATION>      ::= "func" "(" <TYPED_ARG_LIST> ")" *(<TYPE> *("," <TYPE>)) <BLOCK>
 * <FUNC_DEFINITION>       ::= "func" "(" (<TYPED_ARG_LIST> | <TYPE_LIST>) ")" *(<TYPE> *("," <TYPE>)) ";"
 * <VAR_DECLARATION>       ::= <TYPE> <IDENTIFIER> "=" <EXPRESSION> ";"
 * <VAR_DEFINITION>        ::= <TYPE> <IDENTIFIER> ";"
 * <ARG_LIST>              ::= *(<EXPRESSION> *("," <EXPRESSION>))
 * <TYPED_ARG>             ::= <TYPE> <IDENTIFIER>
 * <TYPED_ARG_LIST>        ::= *(<TYPED_ARG> *("," <TYPED_ARG>))
 * <TYPE_LIST>             ::= *(<TYPE> *("," <TYPE>))
 * <EXPRESSION>            ::= <ASSIGN> | <IDENTIFIER> | <BINARY_RELATION> | <BINARY_OPERATION> | <UNARY_OPERATION> | <FUNC_CALL>
 * <ASSIGN>                ::= <IDENTIFIER> "=" <EXPRESSION>
 * <RIGHT_UNARY_OPERATION> ::= <EXPRESSION> <UNARY_OP>
 * <LEFT_UNARY_OPERATION>  ::= <UNARY_OP> <EXPRESSION>
 * <LEFT_UNARY_OP>         ::= "!" | "-"
 * <RIGHT_UNARY_OP>        ::= "++" | "--"
 * <BINARY_RELATION>       ::= <EXPRESSION> <BINARY_REL> <EXPRESSION>
 * <BINARY_REL>            ::= ">" | "<" | "<=" | ">=" | "==" | "!=" | "&&" | "||"
 * <BINARY_OPERATION>      ::= <EXPRESSION> <BINARY_OP> <EXPRESSION>
 * <BINARY_OP>             ::= "+" | "-" | "*" | "/" | "|" | "^" | "&" | "%"
 * <BLOCK>                 ::= "{" *<STATEMENT> "}"
 * <IDENTIFIER>            ::= ("a"-"z" | "A"-"Z" | "_") *("a"-"z" | "A"-"Z" | "0"-"9" | "_" | "-")
 * <FUNC_CALL>             ::= <IDENTIFIER> "(" <ARG_LIST> ")"
 * <TYPE>                  ::= <IDENTIFIER>
 */

enum Node {
	N_ASSIGN,
	N_ARG_LIST,
	N_BINARY_OPERATION,
	N_BINARY_RELATION,
	N_BLOCK,
	N_ELSE,
	N_ELSE_IF,
	N_EXPRESSION,
	N_EXTERNAL_DECLARATION,
	N_FOR,
	N_FUNC_CALL,
	N_FUNC_DECLARATION,
	N_FUNC_DEFINITION,
	N_ID,
	N_IF,
	N_INSTRUCTION,
	N_LEFT_UNARY_OPERATION,
	N_PROGRAM,
	N_RETURN,
	N_RIGHT_UNARY_OPERATION,
	N_STATEMENT,
	N_TYPE,
	N_TYPE_LIST,
	N_TYPED_ARG,
	N_TYPED_ARG_LIST,
	N_VAR_DECLARATION,
	N_VAR_DEFINITION,
	N_WHILE,
};

/*
 * <BINARY_OP> ::= "+" | "-" | "*" | "/" | "|" | "^" | "&" | "%"
 */
enum BinaryOp {
	BOP_PLUS,
	BOP_MINUS,
	BOP_TIME,
	BOP_DIV,
	BOP_BWOR,
	BOP_BWAND,
	BOP_BWXOR,
	BOP_MOD,
};

/*
 * <BINARY_REL> ::= ">" | "<" | "<=" | ">=" | "==" | "!=" | "&&" | "||"
 */
enum BinaryRel {
	BREL_GT,
	BREL_LT,
	BREL_GEQ,
	BREL_LEQ,
	BREL_EQ,
	BREL_NEQ,
	BREL_AND,
	BREL_OR,
};

/*
 * <EXTERNAL_DECLARATION> ::= <FUNC_DECLARATION>
 */
enum ExternalDeclaration {
	EDEC_FUNC,
};

/*
 * <LEFT_UNARY_OP> ::= "!" | "-"
 */
enum LeftUnaryOp {
	LUOP_NOT,
	LUOP_MINUS,
};

/*
 * <RIGHT_UNARY_OP> ::= "++" | "--"
 */
enum RightUnaryOp {
	RUOP_INC,
	RUOP_DEC,
};

/*
 * <STATEMENT> ::= <IF> | <ELSE> | <RETURN> | <WHILE> | <FOR> | <INSTRUCTION> | <BLOCK>
 */
enum Statement {
	ST_IF,
	ST_ELSE,
	ST_RETURN,
	ST_WHILE,
	ST_FOR,
	ST_INSTRUCTION,
	ST_BLOCK,
};

typedef struct nodeAssign NodeAssign;
typedef struct nodeArgList NodeArgList;
typedef struct nodeBinaryOperation NodeBinaryOperation;
typedef struct nodeBinaryRelation NodeBinaryRelation;
typedef struct nodeBlock NodeBlock;
typedef struct nodeElse NodeElse;
typedef struct nodeElseIf NodeElseIf;
typedef struct nodeExpression NodeExpression;
typedef struct nodeExternalDeclaration NodeExternalDeclaration;
typedef struct nodeFor NodeFor;
typedef struct nodeFuncCall NodeFuncCall;
typedef struct nodeFuncDefinition NodeFuncDefinition;
typedef struct nodeFuncDeclaration NodeFuncDeclaration;
typedef struct nodeId NodeId;
typedef struct nodeIf NodeIf;
typedef struct nodeInstruction NodeInstruction;
typedef struct nodeLeftUnaryOperation NodeLeftUnaryOperation;
typedef struct nodeProgram NodeProgram;
typedef struct nodeReturn NodeReturn;
typedef struct nodeRightUnaryOperation NodeRightUnaryOperation;
typedef struct nodeStatement NodeStatement;
typedef struct nodeType NodeType;
typedef struct nodeTypeList NodeTypeList;
typedef struct nodeTypedArg NodeTypedArg;
typedef struct nodeTypedArgList NodeTypedArgList;
typedef struct nodeVarDeclaration NodeVarDeclaration;
typedef struct nodeVarDefinition NodeVarDefinition;
typedef struct nodeWhile NodeWhile;

/*
 * <ASSIGN> ::= <IDENTIFIER> "=" <EXPRESSION>
 */
struct nodeAssign {
	
};

/*
 * <ARG_LIST> ::= *(<EXPRESSION> *("," <EXPRESSION>))
 */
struct nodeArgList {
	int narg;
	NodeExpression **args;
};

/*
 * <BINARY_OPERATION> ::= <EXPRESSION> <BINARY_OP> <EXPRESSION>
 */
struct nodeBinaryOperation {
	enum BinaryOp op;
	NodeExpression *left;
	NodeExpression *right;
};

/*
 * <BINARY_RELATION> ::= <EXPRESSION> <BINARY_REL> <EXPRESSION>
 */
struct nodeBinaryRelation {
	enum BinaryRel op;
	NodeExpression *left;
	NodeExpression *right;
};

/*
 * <BLOCK> ::= "{" *<STATEMENT> "}"
 */
struct nodeBlock {
	int nstatement;
	NodeStatement **statements;
};

/*
 * <ELSE> ::= "else" <BLOCK>
 */
struct nodeElse {
	NodeBlock *block;
};

/*
 * <ELSE_IF> ::= "else" "if" "(" <EXPRESSION> ")" <BLOCK>
 */
struct nodeElseIf {
	NodeExpression *cond;
	NodeBlock *block;
};

/*
 * <EXPRESSION> ::= <ASSIGN> | <IDENTIFIER> | <BINARY_RELATION> | <BINARY_OPERATION> | <UNARY_OPERATION> | <FUNC_CALL>
 */
struct nodeExpression {
	enum Node type;
	void *expression;
};

/*
 * <EXTERNAL_DECLARATION> ::= <FUNC_DECLARATION>
 */
struct nodeExternalDeclaration {
	enum ExternalDeclaration type;
	void *decl;
};

/*
 * <FOR> ::= "for" "(" <EXPRESSION> ";" <EXPRESSION> ";" <EXPRESSION> ")" <BLOCK>
 */
struct nodeFor {
	NodeExpression *init;
	NodeExpression *cond;
	NodeExpression *inc;
	NodeBlock *block;
};


/*
 * <FUNC_CALL> ::= <IDENTIFIER> "(" <ARG_LIST> ")"
 */
struct nodeFuncCall {
	NodeId *id;
	NodeArgList *arglist;
};

/*
 * <FUNC_DECLARATION> ::= "func" "(" <TYPED_ARG_LIST> ")" *(<TYPE> *("," <TYPE>)) <BLOCK>
 */
struct nodeFuncDeclaration {
	NodeTypedArgList *arglist;
	NodeTypeList *retlist;
};

/*
 * <FUNC_DEFINITION> ::= "func" "(" <TYPE_LIST> ")" *(<TYPE> *("," <TYPE>)) ";"
 */
struct nodeFuncDefinition {
	NodeTypeList *arglist;
	NodeTypeList *retlist;
};

/*
 * <IDENTIFIER> ::= ("a"-"z" | "A"-"Z" | "_") *("a"-"z" | "A"-"Z" | "0"-"9" | "_" | "-")
 */
struct nodeId {
	char *text;
};

/*
 * <IF> ::= "if" "(" <EXPRESSION> ")" <BLOCK>
 */
struct nodeIf {
	NodeExpression *cond;
	NodeBlock *block;
};

/*
 * <INSTRUCTION> ::= <EXPRESSION> ";"
 */
struct nodeInstruction {
	NodeExpression *expr;
};

/*
 * <LEFT_UNARY_OPERATION> := <UNARY_OP> <EXPRESSION>
 */
struct nodeLeftUnaryOperation {
	enum LeftUnaryOp op;
	NodeExpression *expr;
};

/*
 * <PROGRAM> ::= *<EXTERNAL_DECLARATION>
 */
struct nodeProgram {
	int ndecl;
	NodeExternalDeclaration **decls;
};

/*
 * <RETURN> ::= "return" <EXPRESSION>
 */
struct nodeReturn {
	NodeExpression *expr;
};

/*
 * <RIGHT_UNARY_OPERATION> := <EXPRESSION> <UNARY_OP>
 */
struct nodeRightUnaryOperation {
	enum RightUnaryOp op;
	NodeExpression *expr;
};

/*
 * <STATEMENT> ::= <IF> | <ELSE> | <RETURN> | <WHILE> | <FOR> | <INSTRUCTION> | <BLOCK>
 */
struct nodeStatement {
	enum Statement type;
	void *statement;
};

/*
 * <TYPE> ::= <IDENTIFIER>
 */
struct nodeType {
	NodeId *id;
};

/*
 * <TYPE_LIST> ::= *(<TYPE> *("," <TYPE>))
 */
struct nodeTypeList {
	int ntype;
	NodeType **types;
};

/*
 * <TYPED_ARG> ::= <TYPE> <IDENTIFIER>
 */
struct nodeTypedArg {
	NodeType *type;
	NodeId *id;
};

/*
 * <TYPED_ARG_LIST> ::= *(<TYPED_ARG> *("," <TYPED_ARG>))
 */
struct nodeTypedArgList {
	int narg;
	NodeTypedArg **args;
};

/*
 * <VAR_DECLARATION> ::= <TYPE> <IDENTIFIER> "=" <EXPRESSION> ";"
 */
struct nodeVarDeclaration {
	NodeType *type;
	NodeId *id;
	NodeExpression *value;
};

/*
 * <VAR_DEFINITION> ::= <TYPE> <IDENTIFIER> ";"
 */
struct nodeVarDefinition {
	NodeType *type;
	NodeId *id;
};

/*
 * <WHILE> ::= "while" "(" <EXPRESSION> ")" <BLOCK>
 */
struct nodeWhile {
	NodeExpression *cond;
	NodeBlock *block;
};

#endif
