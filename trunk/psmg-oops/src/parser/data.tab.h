/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
     ID = 258,
     INT = 259,
     FLOAT = 260,
     DATA = 261,
     SET = 262,
     PARAM = 263,
     TR = 264,
     DEFINE = 265,
     PLUS = 266,
     MINUS = 267,
     DOT = 268,
     SEMICOLON = 269,
     COLON = 270,
     LBRACE = 271,
     RBRACE = 272,
     LSBRACKET = 273,
     RSBRACKET = 274,
     LBRACKET = 275,
     RBRACKET = 276,
     COMMA = 277
   };
#endif
/* Tokens.  */
#define ID 258
#define INT 259
#define FLOAT 260
#define DATA 261
#define SET 262
#define PARAM 263
#define TR 264
#define DEFINE 265
#define PLUS 266
#define MINUS 267
#define DOT 268
#define SEMICOLON 269
#define COLON 270
#define LBRACE 271
#define RBRACE 272
#define LSBRACKET 273
#define RSBRACKET 274
#define LBRACKET 275
#define RBRACKET 276
#define COMMA 277




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 60 "data.tab.ypp"

   long ival;
   double fval;
   char* string;
 



/* Line 2068 of yacc.c  */
#line 103 "../src/parser/data.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE datalval;


