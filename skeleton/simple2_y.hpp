
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_int_val = 258,
     TK_float_val = 259,
     TK_ident = 260,
     TK_string_val = 261,
     TK_break = 262,
     TK_continue = 263,
     TK_else = 264,
     TK_float = 265,
     TK_if = 266,
     TK_int = 267,
     TK_return = 268,
     TK_void = 269,
     TK_while = 270,
     TK_string = 271,
     TK_bool = 272,
     TK_true = 273,
     TK_false = 274,
     TK_increment = 275,
     TK_decrement = 276,
     TK_less_eq = 277,
     TK_greater_eq = 278,
     TK_equal_to = 279,
     TK_not_equal = 280,
     TK_log_and = 281,
     TK_log_or = 282,
     TK_lp_round = 283,
     TK_rp_round = 284,
     TK_lp_square = 285,
     TK_rp_square = 286,
     TK_lp_brace = 287,
     TK_rp_brace = 288,
     TK_dot = 289,
     TK_log_not = 290,
     TK_plus = 291,
     TK_minus = 292,
     TK_times = 293,
     TK_divide = 294,
     TK_modulo = 295,
     TK_greater = 296,
     TK_less = 297,
     TK_equals = 298,
     TK_comma = 299,
     TK_semicolon = 300
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 23 "C:\\Dev\\PhysicsWorld\\skeleton\\simple2.y"

    int         itype;
    float       ftype;
    char       *stype;
    char        ctype;



/* Line 1676 of yacc.c  */
#line 106 "C:\\Dev\\PhysicsWorld\\skeleton\\simple2_y.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


