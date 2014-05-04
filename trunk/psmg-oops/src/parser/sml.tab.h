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
     INT_VAL = 259,
     FLOAT_VAL = 260,
     INFINITE = 261,
     COEFF = 262,
     COVER = 263,
     OBJ = 264,
     DEFAULT = 265,
     FROM = 266,
     TO = 267,
     TO_COME = 268,
     MODELTYPE = 269,
     DOTDOT = 270,
     NET_IN = 271,
     NET_OUT = 272,
     DIMEN = 273,
     ORDERED = 274,
     CIRCULAR = 275,
     REVERSED = 276,
     SYMBOLIC = 277,
     ARC = 278,
     INTEGER = 279,
     BINARY = 280,
     CHECK = 281,
     CLOSE = 282,
     DISPLAY = 283,
     DROP = 284,
     INCLUDE = 285,
     PRINT = 286,
     PRINTF = 287,
     QUIT = 288,
     RESET = 289,
     RESTORE = 290,
     SOLVE = 291,
     UPDATE = 292,
     WRITE = 293,
     SHELL = 294,
     MODEL = 295,
     DATA = 296,
     OPTION = 297,
     LET = 298,
     SOLUTION = 299,
     FIX = 300,
     UNFIX = 301,
     END = 302,
     FUNCTION = 303,
     PIPE = 304,
     FORMAT = 305,
     SETOF = 306,
     BY = 307,
     LESS = 308,
     MOD = 309,
     DIV = 310,
     MIN = 311,
     MAX = 312,
     IF = 313,
     THEN = 314,
     ELSE = 315,
     AND = 316,
     OR = 317,
     EXISTS = 318,
     FORALL = 319,
     NOT = 320,
     WITHIN = 321,
     WHILE = 322,
     REPEAT = 323,
     FOR = 324,
     CARD = 325,
     NEXT = 326,
     NEXTW = 327,
     PREV = 328,
     PREVW = 329,
     FIRST = 330,
     LAST = 331,
     MEMBER = 332,
     ORD = 333,
     ORD_ZERO = 334,
     VAR = 335,
     PARAM = 336,
     SET = 337,
     MAXIMIZE = 338,
     MINIMIZE = 339,
     OBJECTIVE = 340,
     SUBJECTTO = 341,
     SUM = 342,
     PROD = 343,
     IN = 344,
     POWER = 345,
     NE = 346,
     LE = 347,
     GE = 348,
     EQ = 349,
     LT = 350,
     GT = 351,
     SIN = 352,
     COS = 353,
     UNION = 354,
     DIFF = 355,
     CROSS = 356,
     INTER = 357,
     SYMDIFF = 358,
     LBRACE = 359,
     RBRACE = 360,
     COMMA = 361,
     SEMICOLON = 362,
     LSBRACKET = 363,
     RSBRACKET = 364,
     COLON = 365,
     LBRACKET = 366,
     RBRACKET = 367,
     DEFINED = 368,
     SUFFIX = 369,
     BLOCK = 370,
     USING = 371,
     DETERMINISTIC = 372,
     EXPECTATION = 373,
     STOCHASTIC = 374,
     STAGES = 375,
     ANCESTOR = 376,
     IDREF = 377,
     IDREFM = 378,
     VALUE = 379,
     LIST = 380,
     INDEX = 381,
     DVAR = 382,
     DOT = 383,
     STAGE = 384,
     NODE = 385,
     STRING = 386,
     ASSIGN = 387,
     MINUS = 388,
     PLUS = 389,
     DIVID = 390,
     TIMES = 391
   };
#endif
/* Tokens.  */
#define ID 258
#define INT_VAL 259
#define FLOAT_VAL 260
#define INFINITE 261
#define COEFF 262
#define COVER 263
#define OBJ 264
#define DEFAULT 265
#define FROM 266
#define TO 267
#define TO_COME 268
#define MODELTYPE 269
#define DOTDOT 270
#define NET_IN 271
#define NET_OUT 272
#define DIMEN 273
#define ORDERED 274
#define CIRCULAR 275
#define REVERSED 276
#define SYMBOLIC 277
#define ARC 278
#define INTEGER 279
#define BINARY 280
#define CHECK 281
#define CLOSE 282
#define DISPLAY 283
#define DROP 284
#define INCLUDE 285
#define PRINT 286
#define PRINTF 287
#define QUIT 288
#define RESET 289
#define RESTORE 290
#define SOLVE 291
#define UPDATE 292
#define WRITE 293
#define SHELL 294
#define MODEL 295
#define DATA 296
#define OPTION 297
#define LET 298
#define SOLUTION 299
#define FIX 300
#define UNFIX 301
#define END 302
#define FUNCTION 303
#define PIPE 304
#define FORMAT 305
#define SETOF 306
#define BY 307
#define LESS 308
#define MOD 309
#define DIV 310
#define MIN 311
#define MAX 312
#define IF 313
#define THEN 314
#define ELSE 315
#define AND 316
#define OR 317
#define EXISTS 318
#define FORALL 319
#define NOT 320
#define WITHIN 321
#define WHILE 322
#define REPEAT 323
#define FOR 324
#define CARD 325
#define NEXT 326
#define NEXTW 327
#define PREV 328
#define PREVW 329
#define FIRST 330
#define LAST 331
#define MEMBER 332
#define ORD 333
#define ORD_ZERO 334
#define VAR 335
#define PARAM 336
#define SET 337
#define MAXIMIZE 338
#define MINIMIZE 339
#define OBJECTIVE 340
#define SUBJECTTO 341
#define SUM 342
#define PROD 343
#define IN 344
#define POWER 345
#define NE 346
#define LE 347
#define GE 348
#define EQ 349
#define LT 350
#define GT 351
#define SIN 352
#define COS 353
#define UNION 354
#define DIFF 355
#define CROSS 356
#define INTER 357
#define SYMDIFF 358
#define LBRACE 359
#define RBRACE 360
#define COMMA 361
#define SEMICOLON 362
#define LSBRACKET 363
#define RSBRACKET 364
#define COLON 365
#define LBRACKET 366
#define RBRACKET 367
#define DEFINED 368
#define SUFFIX 369
#define BLOCK 370
#define USING 371
#define DETERMINISTIC 372
#define EXPECTATION 373
#define STOCHASTIC 374
#define STAGES 375
#define ANCESTOR 376
#define IDREF 377
#define IDREFM 378
#define VALUE 379
#define LIST 380
#define INDEX 381
#define DVAR 382
#define DOT 383
#define STAGE 384
#define NODE 385
#define STRING 386
#define ASSIGN 387
#define MINUS 388
#define PLUS 389
#define DIVID 390
#define TIMES 391




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 68 "sml.tab.ypp"

  int optype;
  objType otype;
  long *ival;
  double *fval;
  char* string;
  AmplModel* model;
  SyntaxNode *opPtr;



/* Line 2068 of yacc.c  */
#line 334 "../src/parser/sml.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


