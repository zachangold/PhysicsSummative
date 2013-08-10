%{
/* 
 * $Header$
 *
 * Lexical analyser
 *
 * $Log$
 *
 */

/* Include any application header files here. */
#include "simple2_y.hpp"
#include <string.h>
#include <stdio.h>

/* Include any external references to data structures here. */

/* Local defines get declared here. */
//#define DEBUG

/* Local variables get declared here. */

%}

/* Define a digit in our implemented language (abbreviations) */
d        [0-9]

/* Define any START states here (so you can implement languages within languages) */



%%
\/\/[^\n]*\n	    {
			// comment. ignore.
		    }
[ \t\n]+	    {
			// Got whitespace: just ignore it (it has no meaning to us).
                    }
break		    {
			#ifdef DEBUG
			printf( "    lexer found TK_break\n" );
			#endif
			return(TK_break);
                    }
continue	    {
			#ifdef DEBUG
			printf( "    lexer found TK_continue\n" );
			#endif
			return(TK_continue);
                    }
else		    {
			#ifdef DEBUG
			printf( "    lexer found TK_else\n" );
			#endif
			return(TK_else);
                    }
float		    {
			#ifdef DEBUG
			printf( "    lexer found TK_float\n" );
			#endif
			return(TK_float);
                    }
if		    {
			#ifdef DEBUG
			printf( "    lexer found TK_if\n" );
			#endif
			return(TK_if);
                    }
int		    {
			#ifdef DEBUG
			printf( "    lexer found TK_int\n" );
			#endif
			return(TK_int);
                    }
return		    {
			#ifdef DEBUG
			printf( "    lexer found TK_return\n" );
			#endif
			return(TK_return);
                    }
void		    {
			#ifdef DEBUG
			printf( "    lexer found TK_void\n" );
			#endif
			return(TK_void);
                    }
while		    {
			#ifdef DEBUG
			printf( "    lexer found TK_while\n" );
			#endif
			return(TK_while);
                    }
bool		    {
			#ifdef DEBUG
			printf( "    lexer found TK_bool\n" );
			#endif
			return(TK_bool);
                    }
true		    {
			#ifdef DEBUG
			printf( "    lexer found TK_true\n" );
			#endif
			return(TK_true);
                    }
false		    {
			#ifdef DEBUG
			printf( "    lexer found TK_false\n" );
			#endif
			return(TK_false);
                    }
string		    {
			#ifdef DEBUG
			printf( "    lexer found TK_string\n" );
			#endif
			return(TK_string);
                    }
\+\+		    {
			#ifdef DEBUG
			printf( "    lexer found TK_++\n" );
			#endif
			return(TK_increment);
                    }
\-\-		    {
			#ifdef DEBUG
			printf( "    lexer found TK_--\n" );
			#endif
			return(TK_decrement);
                    }
\<\=		    {
			#ifdef DEBUG
			printf( "    lexer found TK_<=\n" );
			#endif
			return(TK_less_eq);
                    }
\>\=		    {
			#ifdef DEBUG
			printf( "    lexer found TK_>=\n" );
			#endif
			return(TK_greater_eq);
                    }
\=\=		    {
			#ifdef DEBUG
			printf( "    lexer found TK_==\n" );
			#endif
			return(TK_equal_to);
                    }
\!\=		    {
			#ifdef DEBUG
			printf( "    lexer found TK_!=\n" );
			#endif
			return(TK_not_equal);
                    }
\&\&		    {
			#ifdef DEBUG
			printf( "    lexer found TK_&&\n" );
			#endif
			return(TK_log_and);
                    }
\|\|		    {
			#ifdef DEBUG
			printf( "    lexer found TK_||\n" );
			#endif
			return(TK_log_or);
                    }
\"[^\"]*\"	    {
			#ifdef DEBUG
			printf( "    lexer found TK_string_val\n" );
			#endif
			yylval.stype = strdup( yytext );
			return(TK_string_val);
		    }
[a-zA-Z_][a-zA-Z0-9_]*    {
			#ifdef DEBUG
			printf( "    lexer found TK_ident\n" );
			#endif
			yylval.stype=strdup(yytext);
			return(TK_ident);	// Got variable name.
                    }
{d}+\.{d}*	    {
			#ifdef DEBUG
			printf( "    lexer found TK_float_val\n" );
			#endif
			yylval.ftype=atof(yytext);
			return(TK_float_val);	// One or floating point number.
                    }
{d}+		    {
			#ifdef DEBUG
			printf( "    lexer found TK_int_val\n" );
			#endif
			yylval.itype=atoi(yytext);
			return(TK_int_val);        // One or more digits(+) is an integer.
                    }
.                   {
			#ifdef DEBUG
			printf( "    lexer found TK_%c\n", yytext[ 0 ] );
			#endif
                        return(yytext[0]);
                    }
%%

// YYWRAP() takes care of what to do if we reach an end-of-file.
int yywrap(void){
    return 1;
}


int yyerror(char *str){
    printf( "yyerror: %s\n", str );
    return 0;
}

/*
// For debug purposes you can run the lexor from this file (or the C file generated from it.) but
// generally you want to run it from elsewhere.
int main(void){
    int token;

    FILE* fh=fopen("skeleton.txt","r");
    if(fh==NULL){
	fh=stdin;
    }

    yyin=fh;
	
    while(token=yylex()){
	printf("Token=%d\n",token);
	switch(token){
	    case 1000:
		printf("    VariableName=%s\n",yytext);
		break;
	    case 1001:
	    case 1002:
		printf("    Token='%s'\n",yytext);
		break;
	    case 1003:
		printf("    INT Token=%s\n",yytext);
		break;
	    case 1004:
		printf("    FP Token=%s\n",yytext);
		break;
	    default:
		printf("    Unexpected character='%s'\n",yytext);
		break;
	}
    };

    if(fh!=stdin) fclose(fh);
}

*/

/*
 * End of $RCSfile$
 */
