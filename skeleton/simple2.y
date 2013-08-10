/*
 * $Header$
 *
 * A parser for a simple language consisting of a assignment statement:
 *     "set <variable> = <float or int>"
 *
 * $Log$
 *
 */


%{
	
// Application header file.	
#include "simple2_y.hpp"
#include "ParserInfo.h"
#define DEBUG


%}

// This defines the YYLVAL union that the lexor will use to return data.
// It contains all the different data types to return.
%union {
    int         itype;
    float       ftype;
    char       *stype;
    char        ctype;
    bool	btype;
}


// This section defines the data types (using the YYLVAL union types)
/*
%token <stype>      TK_Help
%token <stype>      TK_Usage
%token <itype>      TK_Int
%token <ftype>      TK_Float
%token <stype>      TK_VarName
%token <stype>      TK_EOL
%token <stype>      TK_Exit
*/

%token <itype>      TK_int_val
%token <ftype>      TK_float_val
%token <stype>      TK_ident
%token <stype>      TK_string_val

%token <itype>      TK_break
%token <itype>      TK_continue
%token <itype>      TK_else
%token <itype>      TK_float
%token <itype>      TK_if
%token <itype>      TK_int
%token <itype>      TK_return
%token <itype>      TK_void
%token <itype>      TK_while
%token <itype>      TK_string
%token <itype>      TK_bool
%token <itype>      TK_true
%token <itype>      TK_false

%token <itype>      TK_increment
%token <itype>      TK_decrement
%token <itype>      TK_less_eq
%token <itype>      TK_greater_eq
%token <itype>      TK_equal_to
%token <itype>      TK_not_equal
%token <itype>      TK_log_and
%token <itype>      TK_log_or

%token <itype>      TK_lp_round
%token <itype>      TK_rp_round
%token <itype>      TK_lp_square
%token <itype>      TK_rp_square
%token <itype>      TK_lp_brace
%token <itype>      TK_rp_brace
%token <itype>      TK_dot
%token <itype>      TK_log_not
%token <itype>      TK_plus
%token <itype>      TK_minus
%token <itype>      TK_times
%token <itype>      TK_divide
%token <itype>      TK_modulo
%token <itype>      TK_greater
%token <itype>      TK_less
%token <itype>      TK_equals
%token <itype>      TK_comma
%token <itype>      TK_semicolon


%token <btype>      bool_val


// This section defines each of the sub-sections of the parser below (T_Expr,T_Var etc are application
// types defined in the application header.
%type <itype>      start
%type <itype>      line_list
%type <itype>      line
%type <itype>      statement


%{

// Define the external stuff we need here.
extern int yyerror(char *str);
extern int yylex();
extern int line_num;



%}



%%

start:    line_list             { $$=0; }

line_list:
    line                        { $$=0; }
    | line_list line            { $$=0; }

line:
    ';'                { $$=0; }
    | statement	';'	{ $$=0; }

def_param_list:
	def_parameter
	| def_param_list ',' def_parameter

no_parameter:
	|  TK_void

def_parameter:
	TK_int TK_ident 	{
				    #ifdef DEBUG
				    printf( "Parameter of type int named %s\n", $2 );
				    #endif
				    free( $2 );
				}
	| TK_float TK_ident 	{
				    #ifdef DEBUG
				    printf( "Parameter of type float named %s\n", $2 );
				    #endif
				    free( $2 );
				}
	| TK_string TK_ident 	{
				    #ifdef DEBUG
				    printf( "Parameter of type string named %s\n", $2 );
				    #endif
				    free( $2 );
				}
	| TK_bool TK_ident 	{
				    #ifdef DEBUG
				    printf( "Parameter of type bool named %s\n", $2 );
				    #endif
				    free( $2 );
				}
	| TK_ident TK_ident 	{
				    #ifdef DEBUG
				    printf( "Parameter of type %s named %s\n", $1, $2 );
				    #endif
				    free( $1 );
				    free( $2 );
				}
bool_val:
	TK_true
	| TK_false

funcdef_params:
	def_param_list
	| no_parameter

funcdef:
	TK_int TK_ident '(' funcdef_params ')' {
				    #ifdef DEBUG
				    printf( "Function \"%s\" definition found\n", $2 );
				    #endif
				    Type *type = new Type();
				    type.setFunc( Type::TYPE_INT );

				    Symbol::makeSymbol

				    free( $2 );
				}
	|  TK_float TK_ident '(' funcdef_params ')' {
				    #ifdef DEBUG
				    printf( "Function \"%s\" definition found\n", $2 );
				    #endif
				    free( $2 );
				}
	|  TK_bool TK_ident '(' funcdef_params ')' {
				    #ifdef DEBUG
				    printf( "Function \"%s\" definition found\n", $2 );
				    #endif
				    free( $2 );
				}
	|  TK_ident TK_ident '(' funcdef_params ')' {
				    #ifdef DEBUG
				    printf( "Function \"%s\" definition found\n", $2 );
				    #endif
				    free( $2 );
				}
	|  TK_string TK_ident '(' funcdef_params ')' {
				    #ifdef DEBUG
				    printf( "Function \"%s\" definition found\n", $2 );
				    #endif
				    free( $2 );
				}
	|  TK_void TK_ident '(' funcdef_params ')' {
				    #ifdef DEBUG
				    printf( "Function \"%s\" definition found\n", $2 );
				    #endif
				    free( $2 );
				}
funccall_params:
	| funccall_param_list

funccall_param_list:
	funccall_param
	| funccall_param_list ',' funccall_param

funccall_param:
	  TK_float_val
	| TK_int_val
	| bool_val
	| TK_string_val
	| TK_ident		{
				    free( $1 );
				}

funccall:
	TK_ident '(' funccall_params ')' {
				    #ifdef DEBUG
				    printf( "found function call.\n" );
				    #endif
				}

obj_val:
       	  TK_ident		{
				    free( $1 );
				}
	| funccall

code_body:
	'{' code_body_middle

code_body_middle:
	'}'			{
				    #ifdef DEBUG
				    printf( "Found end of code body\n" );
				    #endif
				}
	| line code_body_middle

condition_expr:
	  bool_val

else_block:
	| TK_else if_block
	| TK_else code_body

if_block:
	TK_if '(' condition_expr ')' code_body else_block	{
				    #ifdef DEBUG
				    printf( "found if statement.\n" );
				    #endif
				}

statement:                      
	TK_int TK_ident		{ 
				    #ifdef DEBUG
				    printf( "int \"%s\" initialized without value.\n", $2 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_INT, std::string( $2 ), new ValueNode( 0 ) );    

				    free( $2 );
				}
	|  TK_int TK_ident '=' TK_int_val {
				    #ifdef DEBUG
				    printf( "int \"%s\" initialized with value %d\n", $2, $4 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_INT, std::string( $2 ), new ValueNode( $4 ) );    

				    free( $2 );
				}
	|  TK_float TK_ident	{ 
				    #ifdef DEBUG
				    printf( "float \"%s\" initialized without value.\n", $2 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_FLOAT, std::string( $2 ), new ValueNode( 0.0f ) );    

				    free( $2 );
				}
	|  TK_float TK_ident '=' TK_float_val {
				    #ifdef DEBUG
				    printf( "float \"%s\" initialized with value %f\n", $2, $4 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_FLOAT, std::string( $2 ), new ValueNode( $4 ) );    

				    free( $2 );
				}
	|  TK_string TK_ident	{ 
				    #ifdef DEBUG
				    printf( "string \"%s\" initialized without value.\n", $2 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_STRING, std::string( $2 ), new ValueNode( std::string( "" ) ) );

				    free( $2 );
				}
	|  TK_string TK_ident '=' TK_string_val {
				    #ifdef DEBUG
				    printf( "string \"%s\" initialized with value %s\n", $2, $4 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_STRING, std::string( $2 ), new ValueNode( std::string( $4 ) ) );    

				    free( $2 );
				    free( $4 );
				}
	|  TK_bool TK_ident	{ 
				    #ifdef DEBUG
				    printf( "bool \"%s\" initialized without value.\n", $2 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_BOOL, std::string( $2 ), new ValueNode( false ) );    

				    free( $2 );
				}
	|  TK_bool TK_ident '=' bool_val {
				    #ifdef DEBUG
				    printf( "bool \"%s\" initialized with value.\n", $2 );
				    #endif
				    Symbol::makeSymbol( Type::TYPE_BOOL, std::string( $2 ), new ValueNode( $4 ) );    

				    free( $2 );
				    //free( $4 );
				}
	|  TK_ident TK_ident	{
				    #ifdef DEBUG
				    printf( "variable %s of type %s created.\n", $2, $1 );
				    #endif
				    Type type = Type( std::string( $1 ) );
				    Symbol::makeSymbol( type, std::string( $2 ), new ValueNode( NULL, type ) );    

				    free( $1 );
				    free( $2 );
				}
	|  TK_ident '=' TK_int_val {
				    #ifdef DEBUG
				    printf( "\"%s\" set to value %d\n", $1, $3 );
				    #endif
				    nodeTarget.push_back( new SetNode( std::string( $1 ), new ValueNode( $3 ) ) );

				    free( $1 );
				}
	|  TK_ident '=' TK_float_val {
				    #ifdef DEBUG
				    printf( "\"%s\" set to value %f\n", $1, $3 );
				    #endif
				    nodeTarget.push_back( new SetNode( std::string( $1 ), new ValueNode( $3 ) ) );

				    free( $1 );
				}
	|  TK_ident '=' TK_string_val {
				    #ifdef DEBUG
				    printf( "\"%s\" set to value %s\n", $1, $3 );
				    #endif
				    nodeTarget.push_back( new SetNode( std::string( $1 ), new ValueNode( $3 ) ) );

				    free( $1 );
				    free( $3 );
				}
	|  TK_ident '=' bool_val {
				    #ifdef DEBUG
				    printf( "\"%s\" set to value %s\n", $1, ($3) ? "true" : "false" );
				    #endif
				    nodeTarget.push_back( new SetNode( std::string( $1 ), new ValueNode( $3 ) ) );

				    free( $1 );
				}
	| funcdef
	| funcdef code_body	{
				    #ifdef DEBUG
				    printf( "function body defined\n" );
				    #endif
				}
	| funccall
	| obj_val '.' funccall	{
				    #ifdef DEBUG
				    printf( "method called.\n" );
				    #endif
				}

/*
 * End of $RCSfile: basic.y,v $
 */
