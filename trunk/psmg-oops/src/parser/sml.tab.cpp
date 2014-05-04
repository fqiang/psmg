/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 19 "sml.tab.ypp"

    #define YYERROR_VERBOSE
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    #include <typeinfo>
    #include <iostream>
    #include "../model/ObjComp.h"
    #include "../model/ModelComp.h" 
    #include "../model/AmplModel.h"
    #include "../model/SyntaxNode.h"
    #include "../model/SyntaxNodeOP.h"
    #include "../model/SyntaxNodeID.h"
    #include "../model/SyntaxNodeValue.h"
    #include "../model/SyntaxNodeIDREF.h"
    #include "../model/SyntaxNodeIDREFM.h"
    #include "../st_model/StochModel.h"

    #include "../util/global_util_functions.h"
    
    using namespace std;

    void print_index_dummy_vars();
    void record_dummy_var();
    void restore_dummy_var();
 
    void begin_model(const string& name, SyntaxNode *indexing, SyntaxNode* stochsets=NULL);
    void end_model(const string& name);

    extern int yylineno;
    int yylex(void);
    void yyerror(const char *s);

    static AmplModel *current_model = NULL;    /* this is the model currently active */
                                       /* this is the GLOBAL context */

	vector<string> index_dummy_vars;    
	vector<int> dummy_var_size;                                            
	bool isDummyVar(string&);
	                                                
    /* ---------------- stochastic global variables:------------------------ */
    static SyntaxNode *curr_stage_indexset;
    extern FILE *yyin;

	bool isInStochModel();
    void addStochInfo(ModelComp *newmc, SyntaxNode*);


/* Line 268 of yacc.c  */
#line 120 "../src/parser/sml.tab.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
#line 68 "sml.tab.ypp"

  int optype;
  objType otype;
  long *ival;
  double *fval;
  char* string;
  AmplModel* model;
  SyntaxNode *opPtr;



/* Line 293 of yacc.c  */
#line 440 "../src/parser/sml.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 452 "../src/parser/sml.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   572

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  154
/* YYNRULES -- Number of states.  */
#define YYNSTATES  275

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   391

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   137,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    25,    26,    27,    37,    38,    39,    54,
      55,    56,    65,    66,    68,    69,    73,    74,    76,    81,
      82,    85,    87,    91,    93,    97,    99,   103,   105,   109,
     111,   115,   116,   124,   125,   127,   129,   132,   135,   136,
     140,   143,   145,   146,   154,   155,   157,   159,   162,   164,
     166,   168,   171,   174,   177,   179,   181,   183,   185,   187,
     189,   190,   198,   199,   201,   203,   207,   209,   211,   214,
     217,   220,   223,   226,   230,   231,   240,   242,   246,   250,
     253,   256,   259,   262,   269,   271,   273,   275,   279,   281,
     282,   287,   291,   295,   297,   299,   301,   303,   305,   309,
     313,   317,   320,   324,   326,   328,   330,   332,   334,   336,
     338,   340,   342,   344,   351,   356,   357,   362,   364,   368,
     370,   372,   376,   379,   383,   387,   391,   395,   399,   404,
     411,   416,   421,   422,   429,   431,   433,   435,   437,   439,
     441,   443,   445,   447,   449
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     139,     0,    -1,    -1,   139,   140,    -1,   160,    -1,   166,
      -1,   172,    -1,   177,    -1,   181,    -1,   141,    -1,   145,
      -1,   142,    -1,   148,    -1,    -1,    -1,   115,     3,   143,
     153,   110,   144,   104,   139,   105,    -1,    -1,    -1,   115,
       3,   146,   153,   119,   116,   111,   156,   112,   110,   147,
     104,   139,   105,    -1,    -1,    -1,   120,   149,   154,   110,
     150,   104,   139,   105,    -1,    -1,   117,    -1,    -1,   120,
     152,   154,    -1,    -1,   154,    -1,   104,   156,   155,   105,
      -1,    -1,   110,   187,    -1,   157,    -1,   156,   106,   157,
      -1,   191,    -1,   158,    89,   191,    -1,     4,    -1,   194,
      15,   194,    -1,     3,    -1,   111,   159,   112,    -1,     3,
      -1,   159,   106,     3,    -1,    -1,    82,     3,   161,   153,
     151,   162,   107,    -1,    -1,   163,    -1,   164,    -1,   163,
     164,    -1,    18,     4,    -1,    -1,   132,   165,   183,    -1,
      66,   191,    -1,    19,    -1,    -1,    81,     3,   167,   153,
     151,   168,   107,    -1,    -1,   169,    -1,   170,    -1,   169,
     170,    -1,    25,    -1,    24,    -1,    22,    -1,   171,   194,
      -1,    10,   194,    -1,   113,   194,    -1,    92,    -1,    93,
      -1,    95,    -1,    96,    -1,    94,    -1,    91,    -1,    -1,
      80,     3,   173,   153,   151,   174,   107,    -1,    -1,   175,
      -1,   176,    -1,   175,   106,   176,    -1,    25,    -1,    24,
      -1,    95,   194,    -1,    92,   194,    -1,    96,   194,    -1,
      93,   194,    -1,    10,   194,    -1,   114,     3,   194,    -1,
      -1,    86,     3,   178,   153,   151,   110,   179,   107,    -1,
     180,    -1,   179,   106,   180,    -1,   194,   132,   194,    -1,
      95,   194,    -1,    92,   194,    -1,    96,   194,    -1,    93,
     194,    -1,   182,     3,   151,   110,   194,   107,    -1,    83,
      -1,    84,    -1,   154,    -1,   194,    15,   194,    -1,   191,
      -1,    -1,    51,   184,   154,   191,    -1,   183,   185,   183,
      -1,   183,   186,   183,    -1,   100,    -1,   103,    -1,   101,
      -1,    99,    -1,   102,    -1,   191,    89,   191,    -1,   111,
     187,   112,    -1,   187,   189,   187,    -1,   190,   187,    -1,
     194,   188,   194,    -1,    91,    -1,    92,    -1,    93,    -1,
      95,    -1,    96,    -1,    94,    -1,    61,    -1,    62,    -1,
      65,    -1,     3,    -1,   121,   111,     4,   112,   128,   191,
      -1,   191,   108,   193,   109,    -1,    -1,   191,   128,   192,
     191,    -1,   194,    -1,   193,   106,   194,    -1,   198,    -1,
     191,    -1,   111,   194,   112,    -1,   133,   194,    -1,   194,
     134,   194,    -1,   194,   133,   194,    -1,   194,   136,   194,
      -1,   194,   135,   194,    -1,   194,    90,   194,    -1,    58,
     187,    59,   194,    -1,    58,   187,    59,   194,    60,   194,
      -1,   118,   111,   194,   112,    -1,   196,   111,   194,   112,
      -1,    -1,   197,   195,   154,   111,   194,   112,    -1,    78,
      -1,    70,    -1,    97,    -1,    98,    -1,    87,    -1,    57,
      -1,    56,    -1,    88,    -1,     4,    -1,     5,    -1,     6,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   127,   127,   128,   131,   132,   133,   134,   135,   136,
     139,   140,   141,   148,   151,   147,   176,   180,   175,   195,
     199,   194,   207,   208,   214,   213,   232,   233,   236,   246,
     247,   254,   258,   266,   270,   275,   279,   285,   291,   297,
     303,   318,   317,   337,   338,   344,   348,   356,   361,   360,
     368,   373,   383,   382,   399,   400,   407,   411,   419,   420,
     421,   422,   423,   424,   427,   428,   429,   430,   431,   432,
     439,   438,   453,   454,   458,   462,   470,   471,   472,   473,
     474,   475,   476,   477,   484,   483,   501,   505,   513,   519,
     520,   521,   522,   527,   540,   541,   547,   552,   556,   563,
     562,   570,   573,   579,   580,   581,   584,   585,   592,   597,
     600,   604,   608,   614,   615,   616,   617,   618,   619,   622,
     623,   625,   636,   660,   666,   673,   672,   687,   692,   701,
     706,   710,   711,   712,   713,   714,   715,   716,   718,   719,
     720,   724,   729,   728,   738,   739,   740,   741,   744,   745,
     746,   747,   750,   753,   756
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "INT_VAL", "FLOAT_VAL", "INFINITE",
  "COEFF", "COVER", "OBJ", "DEFAULT", "FROM", "TO", "TO_COME", "MODELTYPE",
  "DOTDOT", "NET_IN", "NET_OUT", "DIMEN", "ORDERED", "CIRCULAR",
  "REVERSED", "SYMBOLIC", "ARC", "INTEGER", "BINARY", "CHECK", "CLOSE",
  "DISPLAY", "DROP", "INCLUDE", "PRINT", "PRINTF", "QUIT", "RESET",
  "RESTORE", "SOLVE", "UPDATE", "WRITE", "SHELL", "MODEL", "DATA",
  "OPTION", "LET", "SOLUTION", "FIX", "UNFIX", "END", "FUNCTION", "PIPE",
  "FORMAT", "SETOF", "BY", "LESS", "MOD", "DIV", "MIN", "MAX", "IF",
  "THEN", "ELSE", "AND", "OR", "EXISTS", "FORALL", "NOT", "WITHIN",
  "WHILE", "REPEAT", "FOR", "CARD", "NEXT", "NEXTW", "PREV", "PREVW",
  "FIRST", "LAST", "MEMBER", "ORD", "ORD_ZERO", "VAR", "PARAM", "SET",
  "MAXIMIZE", "MINIMIZE", "OBJECTIVE", "SUBJECTTO", "SUM", "PROD", "IN",
  "POWER", "NE", "LE", "GE", "EQ", "LT", "GT", "SIN", "COS", "UNION",
  "DIFF", "CROSS", "INTER", "SYMDIFF", "LBRACE", "RBRACE", "COMMA",
  "SEMICOLON", "LSBRACKET", "RSBRACKET", "COLON", "LBRACKET", "RBRACKET",
  "DEFINED", "SUFFIX", "BLOCK", "USING", "DETERMINISTIC", "EXPECTATION",
  "STOCHASTIC", "STAGES", "ANCESTOR", "IDREF", "IDREFM", "VALUE", "LIST",
  "INDEX", "DVAR", "DOT", "STAGE", "NODE", "STRING", "ASSIGN", "MINUS",
  "PLUS", "DIVID", "TIMES", "'!'", "$accept", "statements", "statement",
  "block", "blockblock", "$@1", "$@2", "stochblock", "$@3", "$@4",
  "stageblock", "$@5", "$@6", "stochattr_opt", "$@7", "indexing_opt",
  "indexing_set", "indexing_condition_opt", "indexing_setexpr_list",
  "indexing_setexpr_item", "indexing_dummy_list", "indexing_dummy",
  "setdef", "$@8", "setattributes_opt", "setattributes", "setattribute",
  "$@9", "paramdef", "$@10", "paramattributes_opt", "paramattributes",
  "paramattribute", "relop", "vardef", "$@11", "varattributes_opt",
  "varattributes", "varattribute", "consdef", "$@12", "consattributes",
  "consattribute", "objdef", "objdef_type", "setexpression", "$@13",
  "bsetop", "ubsetop", "lexpr", "b_compare_op", "blogic_op", "ulogic_op",
  "identifier", "@14", "expr_list", "expr", "$@15", "func_op",
  "reduction_op", "value", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   138,   139,   139,   140,   140,   140,   140,   140,   140,
     141,   141,   141,   143,   144,   142,   146,   147,   145,   149,
     150,   148,   151,   151,   152,   151,   153,   153,   154,   155,
     155,   156,   156,   157,   157,   157,   157,   158,   158,   159,
     159,   161,   160,   162,   162,   163,   163,   164,   165,   164,
     164,   164,   167,   166,   168,   168,   169,   169,   170,   170,
     170,   170,   170,   170,   171,   171,   171,   171,   171,   171,
     173,   172,   174,   174,   175,   175,   176,   176,   176,   176,
     176,   176,   176,   176,   178,   177,   179,   179,   180,   180,
     180,   180,   180,   181,   182,   182,   183,   183,   183,   184,
     183,   183,   183,   185,   185,   185,   186,   186,   187,   187,
     187,   187,   187,   188,   188,   188,   188,   188,   188,   189,
     189,   190,   191,   191,   191,   192,   191,   193,   193,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   195,   194,   196,   196,   196,   196,   197,   197,
     197,   197,   198,   198,   198
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     0,     9,     0,     0,    14,     0,
       0,     8,     0,     1,     0,     3,     0,     1,     4,     0,
       2,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     0,     7,     0,     1,     1,     2,     2,     0,     3,
       2,     1,     0,     7,     0,     1,     1,     2,     1,     1,
       1,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       0,     7,     0,     1,     1,     3,     1,     1,     2,     2,
       2,     2,     2,     3,     0,     8,     1,     3,     3,     2,
       2,     2,     2,     6,     1,     1,     1,     3,     1,     0,
       4,     3,     3,     1,     1,     1,     1,     1,     3,     3,
       3,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     4,     0,     4,     1,     3,     1,
       1,     3,     2,     3,     3,     3,     3,     3,     4,     6,
       4,     4,     0,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,    94,    95,     0,     0,
      19,     3,     9,    11,    10,    12,     4,     5,     6,     7,
       8,     0,    70,    52,    41,    84,    13,     0,    22,    26,
      26,    26,    26,    26,    26,     0,     0,    23,    24,     0,
      22,    27,    22,    22,    22,     0,     0,   122,   152,   153,
     154,   150,   149,     0,   145,   144,   148,   151,   146,   147,
       0,     0,     0,     0,    29,    31,     0,   130,     0,     0,
     142,   129,    20,     0,     0,    72,    54,    43,     0,    14,
       0,   122,   152,   121,     0,     0,     0,   130,     0,   122,
       0,     0,   130,     0,     0,     0,   132,     0,     0,     0,
       0,     0,   125,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,     0,    77,    76,     0,     0,     0,
       0,     0,     0,    73,    74,     0,    60,    59,    58,    69,
      64,    65,    68,    66,    67,     0,     0,    55,    56,     0,
       0,    51,     0,    48,     0,    44,    45,     0,     0,     0,
       0,     0,     0,   119,   120,     0,   111,     0,   113,   114,
     115,   118,   116,   117,     0,     0,    38,   131,     0,     0,
      32,    30,    28,    34,     0,   127,     0,    36,   137,   134,
     133,   136,   135,     0,     0,     2,    93,    82,    79,    81,
      78,    80,     0,    71,     0,    62,    63,    53,    57,    61,
      47,    50,     0,    42,    46,     0,     0,     0,     0,     0,
      86,     0,     2,     0,   109,   138,   110,   108,   112,    40,
     140,     0,     0,   124,   126,   141,     0,     0,    83,    75,
      99,    96,    49,    98,     0,    90,    92,    89,    91,     0,
      85,     0,     0,     0,     0,     0,   128,     0,    21,     0,
     106,   103,   105,   107,   104,     0,     0,     0,    87,    88,
      15,     0,   139,   123,   143,     0,   101,   102,    97,    17,
     100,     0,     2,     0,    18
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    11,    12,    13,    33,   148,    14,    34,   271,
      15,    27,   111,    39,    73,    40,    41,    99,    64,    65,
      66,    91,    16,    31,   144,   145,   146,   202,    17,    30,
     136,   137,   138,   139,    18,    29,   122,   123,   124,    19,
      32,   209,   210,    20,    21,   232,   249,   255,   256,    85,
     164,   155,    86,    92,   176,   174,    88,   110,    69,    70,
      71
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -190
static const yytype_int16 yypact[] =
{
    -190,   213,  -190,    24,    50,    52,  -190,  -190,    73,    74,
    -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,
    -190,    85,  -190,  -190,  -190,  -190,   -44,   -13,   -95,   -13,
     -13,   -13,   -13,   -13,   -13,   273,   -18,  -190,  -190,   -15,
     -95,  -190,   -95,   -95,   -95,   -14,   -21,    17,   -70,  -190,
    -190,  -190,  -190,   267,  -190,  -190,  -190,  -190,  -190,  -190,
     311,     0,     6,   369,   -65,  -190,    20,   -82,    -9,     7,
    -190,  -190,  -190,   -13,   369,    54,   324,     2,    13,  -190,
      12,  -190,  -190,  -190,   267,    83,   267,   -76,   381,   -96,
     369,   -69,   -94,    72,   369,   125,   -75,   273,   267,    25,
      -2,   369,  -190,   369,   369,   369,   369,   369,   369,   369,
     -13,    32,  -190,   269,   369,  -190,  -190,   369,   369,   369,
     369,   130,    57,    55,  -190,   369,  -190,  -190,  -190,  -190,
    -190,  -190,  -190,  -190,  -190,   369,    58,   324,  -190,   369,
     159,  -190,    -2,  -190,    59,     2,  -190,   171,    65,    61,
     -54,   358,   369,  -190,  -190,   267,   -50,    -2,  -190,  -190,
    -190,  -190,  -190,  -190,   369,   167,  -190,  -190,   300,    68,
    -190,   -50,  -190,   -94,   -52,    81,    -2,    81,  -190,   -75,
     -75,    91,    91,   348,    62,  -190,  -190,    81,    81,    81,
      81,    81,   369,  -190,    54,    81,    81,  -190,  -190,    81,
    -190,   -94,   187,  -190,  -190,   369,   369,   369,   369,   -59,
    -190,   365,  -190,   273,  -190,   -51,   -50,   -94,    81,  -190,
    -190,    66,   369,  -190,   -94,  -190,   369,   438,    81,  -190,
    -190,  -190,    56,     4,    18,    81,    81,    81,    81,   171,
    -190,   369,   445,   -68,   369,    -2,    81,   373,  -190,   -13,
    -190,  -190,  -190,  -190,  -190,   187,   187,   369,  -190,    81,
    -190,    75,    81,   -94,  -190,    -2,    56,    56,    81,  -190,
     -94,    78,  -190,   452,  -190
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -190,  -183,  -190,  -190,  -190,  -190,  -190,  -190,  -190,  -190,
    -190,  -190,  -190,    29,  -190,   279,   -23,  -190,   -30,    89,
    -190,  -190,  -190,  -190,  -190,  -190,    42,  -190,  -190,  -190,
    -190,  -190,    60,  -190,  -190,  -190,  -190,  -190,    -5,  -190,
    -190,  -190,   -43,  -190,  -190,  -189,  -190,  -190,  -190,   -81,
    -190,  -190,  -190,   -35,  -190,  -190,    -4,  -190,  -190,  -190,
    -190
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -131
static const yytype_int16 yytable[] =
{
      67,    81,   227,   150,    36,   156,   103,   153,   154,   244,
     -39,   153,   154,   157,   101,   104,   -39,   171,    87,  -130,
     140,   141,    37,   -33,   -33,    38,   101,    22,   -33,   242,
     -33,    68,   101,   257,   102,   -35,   -35,   165,    97,   104,
     -35,    97,   -35,   166,   261,    98,   102,   239,   240,    87,
     112,    87,   102,    23,   222,    24,    93,   223,   214,    96,
     107,   108,    67,    87,   114,   173,   266,   267,   142,    75,
     113,    76,    77,    78,   216,   -16,    25,    26,   115,   116,
     151,   104,   105,   106,   107,   108,    93,   184,    28,   273,
     168,    35,    72,    68,  -130,    74,    79,   175,    80,   177,
     178,   179,   180,   181,   182,   183,   -37,   201,   104,   100,
     187,    94,   101,   188,   189,   190,   191,    95,   109,    62,
      87,   195,   217,   147,   105,   106,   107,   108,   149,   169,
     172,   196,   102,   192,   143,   199,   185,  -130,  -130,  -130,
    -130,   224,   152,   211,   153,   154,   117,   118,   215,   119,
     120,   105,   106,   107,   108,   250,   251,   252,   253,   254,
     218,   194,   104,   200,   193,   197,   203,   233,   121,   212,
     219,   104,   213,   226,    81,    82,    49,    50,    67,   231,
     221,   104,   272,   243,   167,   269,   170,   204,   228,   229,
      81,    82,    49,    50,   245,     0,   258,   198,   234,     0,
       0,   235,   236,   237,   238,   105,   106,   107,   108,    68,
     263,     0,     0,     2,   105,   106,   107,   108,   246,     0,
     233,   233,   247,     0,     0,     0,   265,    51,    52,    53,
     270,     0,   231,   231,     0,   211,     0,   259,   230,     0,
     262,    54,     0,    51,    52,    53,     0,     0,     0,    55,
       0,   234,   234,   268,     0,     0,     0,    54,    56,    57,
       0,     0,     0,   205,   206,    55,   207,   208,    58,    59,
      81,    82,    49,    50,    56,    57,    47,    48,    49,    50,
       0,     0,    90,     0,    58,    59,     0,     0,     0,    61,
       0,    35,    62,     3,     4,     5,     6,     7,    90,     8,
       0,     0,     0,     0,    63,    61,     0,     0,    62,    42,
      43,    44,    45,    46,    89,    82,    49,    50,     0,     0,
      63,     0,     0,    51,    52,    53,     0,     0,     9,    51,
      52,    53,    83,    10,   125,     0,     0,    54,     0,     0,
       0,     0,     0,    54,     0,    55,   126,     0,   127,   128,
       0,    55,     0,     0,    56,    57,     0,     0,     0,   104,
      56,    57,     0,     0,    58,    59,     0,    51,    52,    53,
      58,    59,    81,    82,    49,    50,   186,     0,    84,     0,
       0,    54,     0,     0,    60,    61,     0,     0,    62,    55,
     104,    61,     0,     0,    62,     0,     0,     0,    56,    57,
      63,     0,   105,   106,   107,   108,    63,     0,    58,    59,
       0,     0,   220,     0,     0,   129,   130,   131,   132,   133,
     134,     0,    90,     0,     0,    51,    52,    53,     0,    61,
       0,     0,    62,   105,   106,   107,   108,   135,   104,    54,
       0,     0,     0,     0,    63,     0,     0,    55,   104,   158,
     159,   160,   161,   162,   163,   104,    56,    57,     0,     0,
     225,     0,     0,   104,     0,     0,    58,    59,     0,     0,
     167,   104,   158,   159,   160,   161,   162,   163,     0,     0,
      90,   105,   106,   107,   108,   264,     0,    61,     0,     0,
      62,   105,   106,   107,   108,     0,     0,   241,   105,   106,
     107,   108,    63,     0,     0,     0,   105,   106,   107,   108,
       0,     0,     0,     0,   105,   106,   107,   108,     3,     4,
       5,     6,     7,     0,     8,     3,     4,     5,     6,     7,
       0,     8,     3,     4,     5,     6,     7,     0,     8,     0,
       0,     0,     0,   248,     0,     0,     0,     0,     0,     0,
     260,     0,     0,     9,     0,     0,     0,   274,    10,     0,
       9,     0,     0,     0,     0,    10,     0,     9,     0,     0,
       0,     0,    10
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-190))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      35,     3,   185,    84,    27,    86,    15,    61,    62,    60,
     106,    61,    62,    89,   108,    90,   112,    98,    53,    15,
      18,    19,   117,   105,   106,   120,   108,     3,   110,   212,
     112,    35,   108,    15,   128,   105,   106,   106,   106,    90,
     110,   106,   112,   112,   112,   110,   128,   106,   107,    84,
      73,    86,   128,     3,   106,     3,    60,   109,   112,    63,
     135,   136,    97,    98,    10,   100,   255,   256,    66,    40,
      74,    42,    43,    44,   155,   119,     3,     3,    24,    25,
      84,    90,   133,   134,   135,   136,    90,   110,     3,   272,
      94,   104,   110,    97,    90,   110,   110,   101,   119,   103,
     104,   105,   106,   107,   108,   109,    89,   142,    90,    89,
     114,   111,   108,   117,   118,   119,   120,   111,   111,   121,
     155,   125,   157,   110,   133,   134,   135,   136,   116,     4,
     105,   135,   128,     3,   132,   139,   104,   133,   134,   135,
     136,   176,    59,   147,    61,    62,    92,    93,   152,    95,
      96,   133,   134,   135,   136,    99,   100,   101,   102,   103,
     164,   106,    90,     4,   107,   107,   107,   202,   114,   104,
       3,    90,   111,   111,     3,     4,     5,     6,   213,   202,
     112,    90,   104,   213,   112,   110,    97,   145,   192,   194,
       3,     4,     5,     6,   128,    -1,   239,   137,   202,    -1,
      -1,   205,   206,   207,   208,   133,   134,   135,   136,   213,
     245,    -1,    -1,     0,   133,   134,   135,   136,   222,    -1,
     255,   256,   226,    -1,    -1,    -1,   249,    56,    57,    58,
     265,    -1,   255,   256,    -1,   239,    -1,   241,    51,    -1,
     244,    70,    -1,    56,    57,    58,    -1,    -1,    -1,    78,
      -1,   255,   256,   257,    -1,    -1,    -1,    70,    87,    88,
      -1,    -1,    -1,    92,    93,    78,    95,    96,    97,    98,
       3,     4,     5,     6,    87,    88,     3,     4,     5,     6,
      -1,    -1,   111,    -1,    97,    98,    -1,    -1,    -1,   118,
      -1,   104,   121,    80,    81,    82,    83,    84,   111,    86,
      -1,    -1,    -1,    -1,   133,   118,    -1,    -1,   121,    30,
      31,    32,    33,    34,     3,     4,     5,     6,    -1,    -1,
     133,    -1,    -1,    56,    57,    58,    -1,    -1,   115,    56,
      57,    58,    65,   120,    10,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    70,    -1,    78,    22,    -1,    24,    25,
      -1,    78,    -1,    -1,    87,    88,    -1,    -1,    -1,    90,
      87,    88,    -1,    -1,    97,    98,    -1,    56,    57,    58,
      97,    98,     3,     4,     5,     6,   107,    -1,   111,    -1,
      -1,    70,    -1,    -1,   111,   118,    -1,    -1,   121,    78,
      90,   118,    -1,    -1,   121,    -1,    -1,    -1,    87,    88,
     133,    -1,   133,   134,   135,   136,   133,    -1,    97,    98,
      -1,    -1,   112,    -1,    -1,    91,    92,    93,    94,    95,
      96,    -1,   111,    -1,    -1,    56,    57,    58,    -1,   118,
      -1,    -1,   121,   133,   134,   135,   136,   113,    90,    70,
      -1,    -1,    -1,    -1,   133,    -1,    -1,    78,    90,    91,
      92,    93,    94,    95,    96,    90,    87,    88,    -1,    -1,
     112,    -1,    -1,    90,    -1,    -1,    97,    98,    -1,    -1,
     112,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
     111,   133,   134,   135,   136,   112,    -1,   118,    -1,    -1,
     121,   133,   134,   135,   136,    -1,    -1,   132,   133,   134,
     135,   136,   133,    -1,    -1,    -1,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,   133,   134,   135,   136,    80,    81,
      82,    83,    84,    -1,    86,    80,    81,    82,    83,    84,
      -1,    86,    80,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
     105,    -1,    -1,   115,    -1,    -1,    -1,   105,   120,    -1,
     115,    -1,    -1,    -1,    -1,   120,    -1,   115,    -1,    -1,
      -1,    -1,   120
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   139,     0,    80,    81,    82,    83,    84,    86,   115,
     120,   140,   141,   142,   145,   148,   160,   166,   172,   177,
     181,   182,     3,     3,     3,     3,     3,   149,     3,   173,
     167,   161,   178,   143,   146,   104,   154,   117,   120,   151,
     153,   154,   153,   153,   153,   153,   153,     3,     4,     5,
       6,    56,    57,    58,    70,    78,    87,    88,    97,    98,
     111,   118,   121,   133,   156,   157,   158,   191,   194,   196,
     197,   198,   110,   152,   110,   151,   151,   151,   151,   110,
     119,     3,     4,    65,   111,   187,   190,   191,   194,     3,
     111,   159,   191,   194,   111,   111,   194,   106,   110,   155,
      89,   108,   128,    15,    90,   133,   134,   135,   136,   111,
     195,   150,   154,   194,    10,    24,    25,    92,    93,    95,
      96,   114,   174,   175,   176,    10,    22,    24,    25,    91,
      92,    93,    94,    95,    96,   113,   168,   169,   170,   171,
      18,    19,    66,   132,   162,   163,   164,   110,   144,   116,
     187,   194,    59,    61,    62,   189,   187,    89,    91,    92,
      93,    94,    95,    96,   188,   106,   112,   112,   194,     4,
     157,   187,   105,   191,   193,   194,   192,   194,   194,   194,
     194,   194,   194,   194,   154,   104,   107,   194,   194,   194,
     194,   194,     3,   107,   106,   194,   194,   107,   170,   194,
       4,   191,   165,   107,   164,    92,    93,    95,    96,   179,
     180,   194,   104,   111,   112,   194,   187,   191,   194,     3,
     112,   112,   106,   109,   191,   112,   111,   139,   194,   176,
      51,   154,   183,   191,   194,   194,   194,   194,   194,   106,
     107,   132,   139,   156,    60,   128,   194,   194,   105,   184,
      99,   100,   101,   102,   103,   185,   186,    15,   180,   194,
     105,   112,   194,   191,   112,   154,   183,   183,   194,   110,
     191,   147,   104,   139,   105
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 13:

/* Line 1806 of yacc.c  */
#line 148 "sml.tab.ypp"
    {  
				YAC_MODEL_LOG("blockblock: -- started -- "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 151 "sml.tab.ypp"
    {   
				begin_model((yyvsp[(2) - (5)].string),(yyvsp[(4) - (5)].opPtr));
			}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 153 "sml.tab.ypp"
    {
               	end_model((yyvsp[(2) - (9)].string));
               	restore_dummy_var();
            }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 176 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("stochblock: -- started - "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 180 "sml.tab.ypp"
    {
				begin_model((yyvsp[(2) - (10)].string), (yyvsp[(4) - (10)].opPtr), (yyvsp[(8) - (10)].opPtr));
    	    }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 183 "sml.tab.ypp"
    {
               end_model((yyvsp[(2) - (14)].string)); 
	           restore_dummy_var();
               YAC_MODEL_LOG("vardef: -- end -- "<<(yyvsp[(2) - (14)].string));
            }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 195 "sml.tab.ypp"
    {
				assert(isInStochModel() == true);
				record_dummy_var();		
			}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 199 "sml.tab.ypp"
    {    
				curr_stage_indexset = (yyvsp[(3) - (4)].opPtr);
	        }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 201 "sml.tab.ypp"
    {
                curr_stage_indexset = NULL;
				restore_dummy_var();            	
            }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 207 "sml.tab.ypp"
    {(yyval.opPtr) = NULL;}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 209 "sml.tab.ypp"
    {
                	assert(isInStochModel()==true);
                  	(yyval.opPtr) = new SyntaxNode(DETERMINISTIC);
             	}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 214 "sml.tab.ypp"
    {
                	assert(isInStochModel()==true);
             		record_dummy_var();
             	}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 218 "sml.tab.ypp"
    {
                	restore_dummy_var(); 
                  	(yyval.opPtr) = (yyvsp[(3) - (3)].opPtr);
               	}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 232 "sml.tab.ypp"
    {		(yyval.opPtr)=NULL;	}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 233 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 237 "sml.tab.ypp"
    {	
					YAC_MODEL_LOG("indexing -- rule matched!");
					SyntaxNode *tmp = NULL;
		      		tmp = new SyntaxNode(LBRACE, new SyntaxNode(COLON, (yyvsp[(2) - (4)].opPtr), (yyvsp[(3) - (4)].opPtr)));
		           	(yyval.opPtr) = tmp;
		           	YAC_MODEL_LOG("indexing -- rule end -- "<<(yyval.opPtr)->print());
				}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 246 "sml.tab.ypp"
    { (yyval.opPtr)=NULL; }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 248 "sml.tab.ypp"
    {
				  			(yyval.opPtr)=(yyvsp[(2) - (2)].opPtr);
				  		}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 255 "sml.tab.ypp"
    {  
				(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr)); 
			}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 259 "sml.tab.ypp"
    {
                  assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
                  (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
                  (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
			}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 267 "sml.tab.ypp"
    {  
							(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); 
						}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 271 "sml.tab.ypp"
    {
			              	assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
		              		(yyval.opPtr) = new SyntaxNode(IN, (yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
			           	}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 276 "sml.tab.ypp"
    {
			           		(yyval.opPtr) = new SyntaxNodeValue(*(yyvsp[(1) - (1)].ival));
			           	}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 280 "sml.tab.ypp"
    {
			           		(yyval.opPtr) = new SyntaxNode(DOTDOT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
			           	}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 286 "sml.tab.ypp"
    { 
						SyntaxNodeID* idnode = new SyntaxNodeID((yyvsp[(1) - (1)].string));
						index_dummy_vars.push_back(idnode->id);
						(yyval.opPtr) = new SyntaxNode(COMMA, idnode);
					}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 292 "sml.tab.ypp"
    { 
			  			(yyval.opPtr) = (yyvsp[(2) - (3)].opPtr);
			  		}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 298 "sml.tab.ypp"
    {
					SyntaxNodeID* idnode = new SyntaxNodeID((yyvsp[(1) - (1)].string));
					index_dummy_vars.push_back(idnode->id);
					(yyval.opPtr) = new SyntaxNode(COMMA, idnode);
				}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 304 "sml.tab.ypp"
    {
			   		assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
			   		SyntaxNodeID* idnode = new SyntaxNodeID((yyvsp[(3) - (3)].string));
			   		(yyvsp[(1) - (3)].opPtr)->push_back(idnode);
					index_dummy_vars.push_back(idnode->id);
			   		(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
			   	}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 318 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("setdef: start -- ID=["<<(yyvsp[(2) - (2)].string)<<"]");
			record_dummy_var();
		}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 322 "sml.tab.ypp"
    {
            YAC_MODEL_LOG("indexing_opt=["<<(yyvsp[(4) - (7)].opPtr)<<"]");
            YAC_MODEL_LOG("stochattr_opt=["<<(yyvsp[(5) - (7)].opPtr)<<"]");
            YAC_MODEL_LOG("setattributes_opt=["<<(yyvsp[(6) - (7)].opPtr)<<"]");
            ModelComp *newmc = new SetComp((yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].opPtr), (yyvsp[(6) - (7)].opPtr),current_model);
            if (isInStochModel()){
            	addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
            }
            current_model->addComp(newmc);
           	
           	restore_dummy_var();
           	YAC_MODEL_LOG("setdef: -- end -- "<<(yyvsp[(2) - (7)].string));
        }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 337 "sml.tab.ypp"
    { (yyval.opPtr)=NULL; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 339 "sml.tab.ypp"
    {
                 		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
                 	}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 345 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr));
				}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 349 "sml.tab.ypp"
    {
					assert((yyvsp[(1) - (2)].opPtr)->opCode == COMMA);
					(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
					(yyval.opPtr) = (yyvsp[(1) - (2)].opPtr);
				}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 357 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(DIMEN, new SyntaxNodeValue(*(yyvsp[(2) - (2)].ival)));
	           	}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 361 "sml.tab.ypp"
    {
            		record_dummy_var();
            	}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 364 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(ASSIGN, (yyvsp[(3) - (3)].opPtr));
	            	restore_dummy_var();
				}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 369 "sml.tab.ypp"
    {
					assert((yyvsp[(2) - (2)].opPtr)->opCode == IDREF);
					(yyval.opPtr) = new SyntaxNode(WITHIN, (yyvsp[(2) - (2)].opPtr));
				}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 374 "sml.tab.ypp"
    { 
            		(yyval.opPtr) = new SyntaxNode(ORDERED); 
            	}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 383 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("paramdef: -- start -- "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 386 "sml.tab.ypp"
    {
			   	ModelComp *newmc = new ParamComp((yyvsp[(2) - (7)].string),(yyvsp[(4) - (7)].opPtr),(yyvsp[(6) - (7)].opPtr),current_model);
			   	if (isInStochModel())
			   	{
			      	addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
			   	}
			   	current_model->addComp(newmc);
               	
               	restore_dummy_var();
               	YAC_MODEL_LOG("paramdef: -- end -- "<<(yyvsp[(2) - (7)].string));
            }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 399 "sml.tab.ypp"
    { (yyval.opPtr) = NULL; }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 401 "sml.tab.ypp"
    {
                   	 	(yyval.opPtr) =(yyvsp[(1) - (1)].opPtr);
                   	 }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 408 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr));
				}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 412 "sml.tab.ypp"
    {
					assert((yyvsp[(1) - (2)].opPtr)->opCode == COMMA);
					(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
					(yyval.opPtr) = (yyvsp[(1) - (2)].opPtr);
				}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 419 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 420 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 421 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 422 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNodeOP((yyvsp[(1) - (2)].optype), (yyvsp[(2) - (2)].opPtr));}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 423 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 424 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 427 "sml.tab.ypp"
    {(yyval.optype)=LE;}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 428 "sml.tab.ypp"
    {(yyval.optype)=GE;}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 429 "sml.tab.ypp"
    {(yyval.optype)=LT;}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 430 "sml.tab.ypp"
    {(yyval.optype)=GT;}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 431 "sml.tab.ypp"
    {(yyval.optype)=EQ;}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 432 "sml.tab.ypp"
    {(yyval.optype)=NE;}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 439 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("vardef: -- start -- "<<(yyvsp[(2) - (2)].string));
			record_dummy_var();
		}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 442 "sml.tab.ypp"
    {
            ModelComp *newmc = new VarComp((yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].opPtr), (yyvsp[(6) - (7)].opPtr),current_model);
            if (isInStochModel()){
            	addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
            }
            current_model->addComp(newmc);
          	restore_dummy_var();
          	YAC_MODEL_LOG("vardef: -- end -- "<<(yyvsp[(2) - (7)].string));
        }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 453 "sml.tab.ypp"
    {(yyval.opPtr)=NULL;}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 454 "sml.tab.ypp"
    {(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 459 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr)); 
               }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 463 "sml.tab.ypp"
    {
               		assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
             		(yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
             		(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
               }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 470 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 471 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 472 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LT, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 473 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LE, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 474 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(GT, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 475 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(GE, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 476 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 477 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SUFFIX, new SyntaxNodeID((yyvsp[(2) - (3)].string)), (yyvsp[(3) - (3)].opPtr));}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 484 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("consdef: -- start -- "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 488 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("consdef: -- indexing -- "<<(yyvsp[(4) - (8)].opPtr));
				ModelComp *newmc = new ConsComp((yyvsp[(2) - (8)].string), (yyvsp[(4) - (8)].opPtr), (yyvsp[(7) - (8)].opPtr),current_model);
	            if (isInStochModel()){
					addStochInfo(newmc, (yyvsp[(5) - (8)].opPtr));
	            }
	            current_model->addComp(newmc);
	            
	            restore_dummy_var();
	            YAC_MODEL_LOG("consdef: -- end -- "<<(yyvsp[(2) - (8)].string));
			}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 502 "sml.tab.ypp"
    {
				(yyval.opPtr) = new SyntaxNode(COMMA, (yyvsp[(1) - (1)].opPtr));
			}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 506 "sml.tab.ypp"
    {
				assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
         		(yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
         		(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
			}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 514 "sml.tab.ypp"
    { 
				assert((yyvsp[(1) - (3)].opPtr)!=NULL);
				assert((yyvsp[(3) - (3)].opPtr)!=NULL);
				(yyval.opPtr) = new SyntaxNode(ASSIGN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));  
			}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 519 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LT, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 520 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LE, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 521 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(GT, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 522 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(GE, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 528 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("objdef: -- "<<(yyvsp[(2) - (6)].string));
			ModelComp *newmc = new ObjComp((yyvsp[(2) - (6)].string), (yyvsp[(1) - (6)].otype), (yyvsp[(5) - (6)].opPtr),current_model);
            if (isInStochModel()){
	           addStochInfo(newmc, (yyvsp[(3) - (6)].opPtr));
            }
            
            current_model->addComp(newmc);
            YAC_MODEL_LOG("objdef: -- end -- "<<(yyvsp[(2) - (6)].string));
		}
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 540 "sml.tab.ypp"
    { (yyval.otype) = TMAX; }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 541 "sml.tab.ypp"
    { (yyval.otype) = TMIN; }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 548 "sml.tab.ypp"
    {
					YAC_MODEL_LOG("setexpression: indexing -- "<<(yyvsp[(1) - (1)].opPtr));
					(yyval.opPtr)= (yyvsp[(1) - (1)].opPtr);
				}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 553 "sml.tab.ypp"
    {
	           		(yyval.opPtr) = new SyntaxNode(DOTDOT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
	           	}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 557 "sml.tab.ypp"
    {
		     		YAC_MODEL_LOG("setexpression: identifier -- rule matched -- "<<(yyvsp[(1) - (1)].opPtr));
		     		assert((yyvsp[(1) - (1)].opPtr)->opCode == IDREF);
		     		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
             	}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 563 "sml.tab.ypp"
    { 
	         		YAC_MODEL_LOG("setexpression - setof -- start ");
	         		record_dummy_var();
	         	}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 566 "sml.tab.ypp"
    {
	         		restore_dummy_var();
	         		(yyval.opPtr) = new SyntaxNode(SETOF,(yyvsp[(3) - (4)].opPtr),(yyvsp[(4) - (4)].opPtr));
	            }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 570 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               	}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 573 "sml.tab.ypp"
    {
                  	(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               	}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 579 "sml.tab.ypp"
    { (yyval.optype) = DIFF; }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 580 "sml.tab.ypp"
    { (yyval.optype) = SYMDIFF; }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 581 "sml.tab.ypp"
    { (yyval.optype) = CROSS; }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 584 "sml.tab.ypp"
    { (yyval.optype) = UNION; }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 585 "sml.tab.ypp"
    { (yyval.optype) = INTER; }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 592 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("lexpr: identifier IN identifier - rule matched! "<<(yyvsp[(1) - (3)].opPtr)<<" "<<(yyvsp[(3) - (3)].opPtr));
			assert((yyvsp[(3) - (3)].opPtr)->opCode == IDREF);
			(yyval.opPtr) = new SyntaxNode(IN,(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
		}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 597 "sml.tab.ypp"
    {
            (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr); 
		}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 601 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype),(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 605 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (2)].optype),(yyvsp[(2) - (2)].opPtr));
     	}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 609 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 614 "sml.tab.ypp"
    { (yyval.optype) = NE; }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 615 "sml.tab.ypp"
    { (yyval.optype) = LE; }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 616 "sml.tab.ypp"
    { (yyval.optype) = GE; }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 617 "sml.tab.ypp"
    { (yyval.optype) = LT; }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 618 "sml.tab.ypp"
    { (yyval.optype) = GT; }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 619 "sml.tab.ypp"
    { (yyval.optype) = EQ; }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 622 "sml.tab.ypp"
    { (yyval.optype) = AND; }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 623 "sml.tab.ypp"
    {  (yyval.optype) = OR; }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 625 "sml.tab.ypp"
    {  (yyval.optype) = NOT; }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 637 "sml.tab.ypp"
    {
			   	YAC_MODEL_LOG("identifier: ID  -- "<<(yyvsp[(1) - (1)].string));
			   	string key = (yyvsp[(1) - (1)].string);
			   	if(isDummyVar(key))
			   	{
					(yyval.opPtr) = new SyntaxNodeID((yyvsp[(1) - (1)].string));
			   	}
			   	else
			   	{
			   		// a better implementation will be findModelComp below and above current model according to 
			   		// different cases: 1. a dot notation. ie. identifier.identifier[index] or 2. just a simple identifier.
			   		// for case 1: first identifier should reference to a model below current model
			   		// for case 2: identifier should be anything above or at current model.
			   		ModelComp* 	ref = current_model->findModelComp(key); 
			   		if(ref->type == TMODEL){
			   		   	(yyval.opPtr) = new SyntaxNodeIDREFM(new SyntaxNodeID((yyvsp[(1) - (1)].string)),static_cast<AmplModel*>(ref));
				   	}
				   	else
				   	{
				   	   	(yyval.opPtr) = new SyntaxNodeIDREF(new SyntaxNodeID((yyvsp[(1) - (1)].string)),ref);
				   	}
			   	}
            }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 661 "sml.tab.ypp"
    {
				assert(isInStochModel()==true);
	            assert((yyvsp[(6) - (6)].opPtr)->opCode == IDREF);
				(yyval.opPtr) = new SyntaxNode(ANCESTOR, new SyntaxNodeValue(*(yyvsp[(3) - (6)].ival)), (yyvsp[(6) - (6)].opPtr));
          	}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 667 "sml.tab.ypp"
    {
				assert((yyvsp[(1) - (4)].opPtr)->opCode == IDREF || (yyvsp[(1) - (4)].opPtr)->opCode == IDREFM);
          		(yyvsp[(1) - (4)].opPtr)->push_back((yyvsp[(3) - (4)].opPtr));
          		(yyval.opPtr) = (yyvsp[(1) - (4)].opPtr);
          	}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 673 "sml.tab.ypp"
    {
               	(yyval.model) = current_model;
          		assert((yyvsp[(1) - (2)].opPtr)->opCode == IDREFM);
          		current_model = static_cast<SyntaxNodeIDREFM*>((yyvsp[(1) - (2)].opPtr))->ref;
			}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 679 "sml.tab.ypp"
    { 
				(yyval.opPtr) = new SyntaxNode(DOT, (yyvsp[(1) - (4)].opPtr), (yyvsp[(4) - (4)].opPtr)); 
				current_model = (yyvsp[(3) - (4)].model);
			}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 688 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("expr_list: expr rule matched! "<<(yyvsp[(1) - (1)].opPtr));
               	(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr));
            }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 693 "sml.tab.ypp"
    { 
         		YAC_MODEL_LOG("expr_list: expr_list COMMA expr -- "<<(yyvsp[(1) - (3)].opPtr));
         		YAC_MODEL_LOG("expr_list: expr_list COMMA expr -- "<<(yyvsp[(3) - (3)].opPtr));
               	assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
               	(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
            }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 702 "sml.tab.ypp"
    { 
		YAC_MODEL_LOG("expr: value rule matched!" <<(yyvsp[(1) - (1)].opPtr));
		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
	}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 707 "sml.tab.ypp"
    {
		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
	}
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 710 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 711 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(MINUS, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 712 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(PLUS, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 713 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(MINUS, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 714 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(TIMES, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 715 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(DIVID, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 716 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(POWER, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 718 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (4)].opPtr), (yyvsp[(4) - (4)].opPtr)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 719 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (6)].opPtr), (yyvsp[(4) - (6)].opPtr), (yyvsp[(6) - (6)].opPtr)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 721 "sml.tab.ypp"
    { 
      	(yyval.opPtr) = new SyntaxNode(EXPECTATION, (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 725 "sml.tab.ypp"
    { 
      	(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (4)].optype), (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 729 "sml.tab.ypp"
    {	
	  	record_dummy_var();
	  }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 732 "sml.tab.ypp"
    {  
		restore_dummy_var();
      	(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (6)].optype), (yyvsp[(3) - (6)].opPtr), (yyvsp[(5) - (6)].opPtr));
      }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 738 "sml.tab.ypp"
    { (yyval.optype)=ORD;   }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 739 "sml.tab.ypp"
    { (yyval.optype)=CARD;  }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 740 "sml.tab.ypp"
    { (yyval.optype) = SIN; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 741 "sml.tab.ypp"
    { (yyval.optype) = COS; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 744 "sml.tab.ypp"
    { (yyval.optype)=SUM; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 745 "sml.tab.ypp"
    { (yyval.optype)=MAX; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 746 "sml.tab.ypp"
    { (yyval.optype)=MIN; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 747 "sml.tab.ypp"
    { (yyval.optype)=PROD; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 750 "sml.tab.ypp"
    {
            (yyval.opPtr)=new SyntaxNodeValue(*(yyvsp[(1) - (1)].ival));
         }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 753 "sml.tab.ypp"
    { 
            (yyval.opPtr)=new SyntaxNodeValue(*(yyvsp[(1) - (1)].fval));
         }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 756 "sml.tab.ypp"
    { 
            (yyval.opPtr) = new SyntaxNode(INFINITE);
         }
    break;



/* Line 1806 of yacc.c  */
#line 3297 "../src/parser/sml.tab.cpp"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 761 "sml.tab.ypp"


void yyerror(const char *s) {
   cerr << "MODEL: " << s << " on line " << yylineno << "\n";
}


/* ----------------------------------------------------------------------------
yywrap
---------------------------------------------------------------------------- */
/* not sure if this is correct, found this somewhere on the internet
   should open the data file and somehow tell the parser to carry on 
   reading in "data mode"
 */
int yywrap(void) {
   return 1;
}

/* ----------------------------------------------------------------------------
begin_model
---------------------------------------------------------------------------- */
void begin_model(const string& name, SyntaxNode *indexing, SyntaxNode* stochsets) 
{
	YAC_MODEL_LOG("begin_model  ["<< name <<"] indexing["<<indexing<<"] -- in current model["<<current_model->name<<"]");
	AmplModel* new_mod = NULL;
	if(stochsets!=NULL)
	{
		assert(indexing==NULL); // stochmodel has indexing=NULL
		assert(stochsets!=NULL && stochsets->nchild()==4);
		YAC_MODEL_LOG(" model ["<<name<<"] is stochastic block!" );
		new_mod = new StochModel(name, stochsets, current_model);
	}
	else
	{
		new_mod = new AmplModel(name,indexing,current_model);
	}
	current_model->addComp(new_mod);  
	current_model = new_mod;
	YAC_MODEL_LOG("     current model now "<<current_model->name);
}

/* ----------------------------------------------------------------------------
end_model
---------------------------------------------------------------------------- */
void end_model(const string& name) 
{
	YAC_MODEL_LOG("end_model -- ["<<current_model->name<<"] -- isStoch["<<isInStochModel()<<"]");
	// Check end block name matches block name
	if (name.compare(current_model->name)!=0) {
		cerr << "end model '" << name << "' encountered in model '" << current_model->name << "'" << endl;
		exit(1);
	}
	current_model = current_model->parent;
	assert(current_model!=NULL);
}


/* ---------------------------------------------------------------------------
Stochastic model helper functions
---------------------------------------------------------------------------- */
void addStochInfo(ModelComp *newmc, SyntaxNode *stochopt) {
	assert(isInStochModel() == true);
	if(curr_stage_indexset != NULL)
	{
		if(stochopt!=NULL && stochopt->opCode == DETERMINISTIC) {
			assert(newmc->type == TVAR);   //DETERMINISTIC only meaningful for var comp
			static_cast<VarComp*>(newmc)->isDet = true;
		}
		else if(stochopt!=NULL)
		{
			cerr<<"ambiguous stage set declaration in side a stageblock!"<<endl;
	 		assert(false);
	 		exit(1);
		}
		newmc->stage = curr_stage_indexset;
	}
	else {
		newmc->stage = stochopt;
	}
}
bool isInStochModel()
{
	if(typeid(StochModel) == typeid(*current_model))
	{
		return true;
	}
	return false;
}

/* ------------------------------------------------------------------------
add_indexing/rem_indexing
-------------------------------------------------------------------------- */
void print_index_dummy_vars()
{
	YAC_MODEL_LOG("print_index_dummy_vars -- ");
	for(int i=0;i<index_dummy_vars.size();i++)
	{
		YAC_MODEL_LOG("["<<i<<"] "<<index_dummy_vars[i]);
	}	
}

void record_dummy_var()
{
	YAC_MODEL_LOG("record_dummy_var -- size["<<index_dummy_vars.size()<<"]");
	dummy_var_size.push_back(index_dummy_vars.size());
}

void restore_dummy_var()
{
	YAC_MODEL_LOG("restore_dummy_var -- size["<<index_dummy_vars.size()<<"]");
	print_index_dummy_vars();
	int pre_size = dummy_var_size.back();
	assert(pre_size <= index_dummy_vars.size());
	for(int i=index_dummy_vars.size()-1;i>=pre_size;i--)
	{
		YAC_MODEL_LOG("remove - "<<index_dummy_vars[i]);
	}
	dummy_var_size.pop_back();
	index_dummy_vars.resize(pre_size);
	print_index_dummy_vars();
}

bool isDummyVar(string& v)
{
	for(vector<string>::iterator it=index_dummy_vars.begin();it!=index_dummy_vars.end();it++)
	{
		if(v.compare(*it)==0)
		{
			return true;
		}
	}
	return false;
}

//yyin opened,and readable
int parse_model() 
{
	int errcode = 0;
	yyin = fopen(GV(modelfilename).c_str(),"r");
	assert(errcode==0);
	string name = "root";
	//creating the root level - as always the case 
   	AmplModel::root = new AmplModel(name,NULL,NULL);
   	current_model = AmplModel::root;
   	current_model->level = 0;
   	
   	//stochastic model related initialization
   	curr_stage_indexset = NULL;

	YAC_MODEL_LOG("starting model parser.... ");
   	errcode = yyparse();
   	assert(errcode==0);
   	YAC_MODEL_LOG("model parser finished !");
   	
   	fclose(yyin);
   	assert(errcode==0);
	return errcode;
}

