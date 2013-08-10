/* 
 * $Header$
 *
 * Main program for Simple2 language.
 *
 * $Log$
 *
 */
#include <cstdio>
#include <ParserInfo.h>

extern int yyparse();
extern int yylex();
extern FILE* yyin;



int main(void){
    int token;

    FILE* fh=fopen("simple2.txt","r");
    if(fh==NULL){
	fh=stdin;
    }

    yyin=fh;

    // This is the parser.  Simple, eh?
    //while( yylex() );
    yyparse();
    printf("Finished parsing\n");

    if(fh!=stdin) fclose(fh);
}


/*
 * End of $RCSfile$
 */
