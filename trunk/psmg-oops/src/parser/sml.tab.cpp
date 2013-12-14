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
   #include <iostream>
   #include "../model/SyntaxNode.h"
   #include "../model/SyntaxNodeIx.h"
   #include "../model/OpNode.h"
   #include "../model/IDNode.h"
   #include "../model/ValueNode.h"
   #include "../model/SyntaxNodeIDREF.h"
   #include "../model/AmplModel.h"
  
   #include "../st_model/StochModel.h"
   #include "../st_model/StochModelComp.h"
   
   #include "../sml/GlobalVariables.h"
   #include "../util/global_util_functions.h"

   using namespace std;

   void add_indexing(SyntaxNodeIx *indexing);
   void rem_indexing(SyntaxNodeIx *indexing);
   void begin_model(char *name, SyntaxNodeIx *indexing);
   void end_model(char *name);
   void begin_smodel(char *name, SyntaxNodeIx *indexing, SyntaxNode *stochsets);
   void end_smodel(char *name);

   extern int yylineno;
   int yylex(void);
   void yyerror(const char *s);

   static AmplModel *current_model;    /* this is the model currently active */
                                       /* this is the GLOBAL context */
   SyntaxNodeIx *list_of_indexing[20];    /* list of currently applicable 
                                                indexing expressions */
   int n_indexing;
   /* ---------------- stochastic global variables:------------------------ */
   static bool is_stoch_model;      /* true if inside stochastic model def */
   /* these are set by global stocastic modifier commands */
   static bool is_deterministic_glo;
   static SyntaxNode *stages_glo;
   extern FILE *yyin;

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
     DOT = 369,
     SUFFIX = 370,
     BLOCK = 371,
     IDREF = 372,
     IDREFM = 373,
     STAGE = 374,
     NODE = 375,
     USING = 376,
     DETERMINISTIC = 377,
     EXPECTATION = 378,
     STOCHASTIC = 379,
     STAGES = 380,
     ANCESTOR = 381,
     ASSIGN = 382
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
#define DOT 369
#define SUFFIX 370
#define BLOCK 371
#define IDREF 372
#define IDREFM 373
#define STAGE 374
#define NODE 375
#define USING 376
#define DETERMINISTIC 377
#define EXPECTATION 378
#define STOCHASTIC 379
#define STAGES 380
#define ANCESTOR 381
#define ASSIGN 382




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 68 "sml.tab.ypp"

  int optype;
  compType ctype;
  long *ival;
  double *fval;
  char *string;
  SyntaxNode *opPtr;
  SyntaxNodeIx *opPtrIx;



/* Line 293 of yacc.c  */
#line 422 "../src/parser/sml.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 434 "../src/parser/sml.tab.cpp"

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
#define YYLAST   540

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  145
/* YYNRULES -- Number of states.  */
#define YYNSTATES  262

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   382

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   132,     2,     2,     2,     2,     2,     2,
       2,     2,   129,   127,     2,   128,     2,   130,     2,     2,
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
     125,   126,   131
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    14,    23,
      28,    38,    43,    47,    50,    53,    56,    59,    61,    62,
      69,    70,    73,    76,    79,    82,    85,    87,    88,    95,
      96,    99,   101,   103,   105,   108,   111,   114,   117,   118,
     125,   126,   128,   130,   134,   136,   138,   140,   143,   146,
     149,   152,   155,   158,   162,   163,   171,   173,   175,   177,
     179,   181,   183,   185,   187,   189,   191,   192,   194,   195,
     201,   202,   205,   207,   211,   213,   217,   219,   223,   225,
     229,   231,   233,   237,   241,   245,   246,   251,   255,   259,
     263,   266,   270,   272,   274,   276,   278,   280,   282,   284,
     286,   288,   289,   291,   294,   296,   298,   300,   302,   304,
     306,   310,   312,   317,   324,   326,   330,   332,   336,   340,
     344,   347,   351,   355,   359,   363,   367,   368,   375,   380,
     387,   392,   397,   402,   407,   409,   411,   413,   415,   417,
     419,   421,   423,   425,   427,   429
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     134,     0,    -1,    -1,   134,   142,    -1,   138,    -1,   136,
      -1,   140,    -1,    -1,   116,     3,   160,   110,   137,   104,
     134,   105,    -1,   139,   104,   134,   105,    -1,   116,     3,
     160,   124,   121,   111,   164,   112,   110,    -1,   141,   104,
     134,   105,    -1,   125,   168,   110,    -1,   143,   107,    -1,
     147,   107,    -1,   151,   107,    -1,   156,   107,    -1,   135,
      -1,    -1,    82,     3,   160,   144,   174,   145,    -1,    -1,
     145,   146,    -1,    18,     4,    -1,    66,   168,    -1,   131,
     168,    -1,    10,   168,    -1,    19,    -1,    -1,    81,     3,
     160,   148,   174,   149,    -1,    -1,   150,   149,    -1,    25,
      -1,    24,    -1,    22,    -1,   159,   180,    -1,    89,   168,
      -1,    10,   180,    -1,   113,   180,    -1,    -1,    80,     3,
     160,   152,   174,   153,    -1,    -1,   154,    -1,   155,    -1,
     154,   106,   155,    -1,    25,    -1,    24,    -1,    22,    -1,
      92,   180,    -1,    93,   180,    -1,   113,   180,    -1,   131,
     180,    -1,    10,   180,    -1,    89,   168,    -1,   115,     3,
     180,    -1,    -1,   158,     3,   160,   157,   174,   110,   180,
      -1,    83,    -1,    84,    -1,    86,    -1,    92,    -1,    93,
      -1,    95,    -1,    96,    -1,   131,    -1,    94,    -1,    91,
      -1,    -1,   161,    -1,    -1,   104,   164,   162,   163,   105,
      -1,    -1,   110,   170,    -1,   165,    -1,   164,   106,   165,
      -1,   168,    -1,   166,    89,   168,    -1,     3,    -1,   111,
     167,   112,    -1,     3,    -1,   167,   106,     3,    -1,   161,
      -1,   177,    -1,   168,   175,   168,    -1,   168,   176,   168,
      -1,   180,    15,   180,    -1,    -1,    51,   161,   169,   177,
      -1,   180,    89,   168,    -1,   111,   170,   112,    -1,   170,
     172,   170,    -1,   173,   170,    -1,   180,   171,   180,    -1,
      91,    -1,    92,    -1,    93,    -1,    95,    -1,    96,    -1,
      94,    -1,    61,    -1,    62,    -1,    65,    -1,    -1,   122,
      -1,   125,   168,    -1,   100,    -1,   103,    -1,   101,    -1,
      99,    -1,   102,    -1,   178,    -1,   177,   114,   178,    -1,
       3,    -1,   178,   108,   179,   109,    -1,   126,   111,     4,
     112,   114,   178,    -1,   180,    -1,   179,   106,   180,    -1,
     184,    -1,   111,   179,   112,    -1,   180,   127,   180,    -1,
     180,   128,   180,    -1,   128,   180,    -1,   180,   129,   180,
      -1,   180,   130,   180,    -1,   180,    90,   180,    -1,   180,
      15,   180,    -1,   180,   131,   180,    -1,    -1,   183,   161,
     181,   111,   180,   112,    -1,    58,   170,    59,   180,    -1,
      58,   170,    59,   180,    60,   180,    -1,    75,   111,   168,
     112,    -1,    76,   111,   168,   112,    -1,   123,   111,   179,
     112,    -1,   182,   111,   179,   112,    -1,    78,    -1,    70,
      -1,    97,    -1,    98,    -1,    87,    -1,    57,    -1,    56,
      -1,    88,    -1,     4,    -1,     5,    -1,   177,    -1,     6,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   123,   123,   124,   127,   128,   129,   142,   142,   180,
     189,   200,   208,   223,   227,   231,   235,   239,   246,   245,
     267,   268,   284,   288,   292,   297,   301,   310,   310,   330,
     331,   346,   347,   348,   349,   350,   351,   352,   359,   359,
     376,   377,   381,   385,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   412,   411,   430,   431,   432,   435,
     436,   437,   438,   439,   440,   441,   444,   445,   449,   448,
     472,   473,   480,   481,   492,   493,   499,   500,   503,   504,
     517,   521,   526,   529,   532,   536,   535,   547,   551,   553,
     557,   561,   567,   568,   569,   570,   571,   572,   575,   576,
     578,   581,   582,   591,   602,   603,   604,   607,   608,   617,
     622,   671,   675,   679,   704,   709,   718,   719,   720,   721,
     722,   723,   724,   725,   726,   727,   729,   728,   737,   738,
     739,   740,   741,   745,   751,   752,   753,   754,   757,   758,
     759,   760,   763,   766,   769,   773
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
  "DEFINED", "DOT", "SUFFIX", "BLOCK", "IDREF", "IDREFM", "STAGE", "NODE",
  "USING", "DETERMINISTIC", "EXPECTATION", "STOCHASTIC", "STAGES",
  "ANCESTOR", "'+'", "'-'", "'*'", "'/'", "ASSIGN", "'!'", "$accept",
  "statements", "block", "blockblock", "$@1", "stochblock",
  "stochblockbegin", "stageblock", "stageblock_start", "statement",
  "setdef", "$@2", "setattributes_opt", "setattribute", "paramdef", "$@3",
  "paramattributes_opt", "paramattribute", "vardef", "$@4",
  "varattributes_opt", "varattributes", "varattribute", "cnstr", "$@5",
  "cnstr_type", "relop", "indexing_opt", "indexing", "@6",
  "indexing_condition", "setexpr_list", "setexpr_item", "mdim_dummy",
  "mdim_dummy_list", "setexpression", "@7", "lexpr", "b_compare_op",
  "blogic_op", "ulogic_op", "stochattr_opt", "bsetop", "ubsetop",
  "identifier", "iditem", "expr_list", "expr", "$@8", "func_op",
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
     375,   376,   377,   378,   379,   380,   381,    43,    45,    42,
      47,   382,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   133,   134,   134,   135,   135,   135,   137,   136,   138,
     139,   140,   141,   142,   142,   142,   142,   142,   144,   143,
     145,   145,   146,   146,   146,   146,   146,   148,   147,   149,
     149,   150,   150,   150,   150,   150,   150,   150,   152,   151,
     153,   153,   154,   154,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   157,   156,   158,   158,   158,   159,
     159,   159,   159,   159,   159,   159,   160,   160,   162,   161,
     163,   163,   164,   164,   165,   165,   166,   166,   167,   167,
     168,   168,   168,   168,   168,   169,   168,   170,   170,   170,
     170,   170,   171,   171,   171,   171,   171,   171,   172,   172,
     173,   174,   174,   174,   175,   175,   175,   176,   176,   177,
     177,   178,   178,   178,   179,   179,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   181,   180,   180,   180,
     180,   180,   180,   180,   182,   182,   182,   182,   183,   183,
     183,   183,   184,   184,   184,   184
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     0,     8,     4,
       9,     4,     3,     2,     2,     2,     2,     1,     0,     6,
       0,     2,     2,     2,     2,     2,     1,     0,     6,     0,
       2,     1,     1,     1,     2,     2,     2,     2,     0,     6,
       0,     1,     1,     3,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     3,     0,     7,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     0,     5,
       0,     2,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     1,     3,     3,     3,     0,     4,     3,     3,     3,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     2,     1,     1,     1,     1,     1,     1,
       3,     1,     4,     6,     1,     3,     1,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     0,     6,     4,     6,
       4,     4,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,    56,    57,    58,     0,
       0,    17,     5,     4,     0,     6,     0,     3,     0,     0,
       0,     0,     0,    66,    66,    66,    66,   111,   142,   143,
     145,     0,   140,   139,     0,   135,     0,     0,   134,   138,
     141,   136,   137,     0,     0,     0,     0,     0,    80,     0,
      81,   109,     0,     0,     0,   116,     2,     2,    13,    14,
      15,    16,    66,    38,    67,    27,    18,     0,    85,   100,
       0,     0,     0,   144,     0,     0,     0,   111,     0,    68,
      72,     0,    74,     0,   114,     0,     0,   120,   107,   104,
     106,   108,   105,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   126,     0,     0,    54,
     101,   101,   101,     7,     0,     0,     0,   114,     0,    98,
      99,     0,    90,     0,     0,    92,    93,    94,    97,    95,
      96,     0,     0,     0,   111,     0,     0,    70,     0,     0,
     117,     0,     0,    82,    83,   110,     0,    84,   123,   118,
     119,   121,   122,   125,     0,     0,     9,    11,   101,   102,
       0,    40,    29,    20,     0,     0,    86,    88,   128,    89,
     124,    87,    91,   130,   131,     0,    77,    73,     0,     0,
      75,   115,   132,     0,   112,   133,     0,     0,   103,     0,
      46,    45,    44,     0,     0,     0,     0,     0,     0,    39,
      41,    42,     0,    33,    32,    31,     0,    65,    59,    60,
      64,    61,    62,     0,    63,    28,    29,     0,    19,     2,
       0,     0,    79,    71,    69,     0,     0,     0,    51,    52,
      47,    48,    49,     0,    50,     0,    36,    35,    37,    30,
      34,     0,     0,    26,     0,     0,    21,     0,     0,   129,
     113,   127,    55,    53,    43,    25,    22,    23,    24,     8,
       0,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    11,    12,   164,    13,    14,    15,    16,    17,
      18,   112,   218,   246,    19,   111,   215,   216,    20,   110,
     199,   200,   201,    21,   158,    22,   217,    63,    48,   137,
     179,    79,    80,    81,   135,    82,   115,    71,   131,   121,
      72,   161,    94,    95,    73,    51,    83,    52,   155,    53,
      54,    55
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -95
static const yytype_int16 yypact[] =
{
     -95,    56,   -95,    29,    50,    57,   -95,   -95,   -95,    66,
     151,   -95,   -95,   -95,   -93,   -95,   -25,   -95,   -17,   -12,
      -6,    -5,    77,     6,     6,     6,     6,   -95,   -95,   -95,
     -95,     6,   -95,   -95,   265,   -95,     7,    14,   -95,   -95,
     -95,   -95,   -95,   208,   311,    22,    30,   311,   -95,   413,
      -7,     3,    16,    42,     6,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,     6,   -95,   -95,   -95,   -95,   -94,   -95,   -95,
     265,    38,   265,     5,   368,   151,   151,    60,   345,    55,
     -95,    75,   350,   -65,   114,   311,   166,    94,   -95,   -95,
     -95,   -95,   -95,   -95,   151,   151,    -2,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   -95,   330,   395,   -95,
      10,    10,    10,   -95,    52,    -2,   -49,   368,   311,   -95,
     -95,   265,   -43,   311,   151,   -95,   -95,   -95,   -95,   -95,
     -95,   311,   -63,   225,   -62,   -60,   208,    64,   151,   311,
     -95,   -58,    65,   350,   350,     3,    54,   173,     8,    94,
      94,    27,    27,    27,   -51,    68,   -95,   -95,    10,   -95,
     151,     0,   335,   -95,    76,    71,     5,   -95,    88,   -43,
     -95,   350,   114,   -95,   -95,   184,   -95,   -95,   265,    84,
     350,   114,   -95,    78,   -95,   -95,   311,    81,   350,   311,
     -95,   -95,   -95,   151,   311,   311,   311,   190,   311,   -95,
      89,   -95,   311,   -95,   -95,   -95,   151,   -95,   -95,   -95,
     -95,   -95,   -95,   311,   -95,   -95,   335,   311,    -4,   -95,
     208,   311,   -95,   -43,   -95,    -2,   161,   311,   114,   350,
     114,   114,   114,   311,   114,     0,   114,   350,   243,   -95,
     114,   151,   193,   -95,   151,   151,   -95,   402,   -24,   114,
       3,   -95,   114,   114,   -95,   350,   -95,   350,   350,   -95,
      91,   -95
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -95,   -53,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,   -11,   -95,   -95,   -95,
     -95,   -95,   -36,   -95,   -95,   -95,   -95,     2,   478,   -95,
     -95,    12,    67,   -95,   -95,    -8,   -95,   -13,   -95,   -95,
     -95,   -77,   -95,   -95,   -10,   -91,   -76,   -27,   -95,   -95,
     -95,   -95
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -145
static const yytype_int16 yytable[] =
{
      50,    27,    49,   107,   108,   145,   241,    74,  -144,   141,
     189,    56,   119,   120,   242,   243,   113,    84,   119,   120,
      87,   146,   190,   123,   191,   192,    65,    66,    67,   154,
     114,    98,    23,    50,   162,   163,    88,    89,    90,    91,
      92,   139,   123,   117,   -78,    74,   175,   140,   139,   173,
     -78,    84,   176,    24,   182,   139,     2,   116,    84,   122,
      25,   185,   244,   167,   109,    50,    50,   132,   133,    26,
      84,   147,   148,   149,   150,   151,   152,   153,    84,    57,
      62,   187,   136,  -144,    50,    50,   143,   144,   260,   193,
      58,   168,   194,   195,    74,    59,   170,   118,    99,   119,
     120,    60,    61,   123,   172,   166,    99,    96,   169,   123,
      43,    97,   181,   196,    50,   197,   171,    99,    75,    96,
    -144,  -144,  -144,  -144,    46,    76,    50,   245,    50,   123,
     180,   198,   159,    85,   250,   160,     3,     4,     5,     6,
       7,    86,     8,   100,   101,   102,   103,   104,   221,   -76,
      50,    74,   188,   105,    27,    28,    29,    30,   104,   226,
     139,   136,   228,   184,   138,   223,   247,   230,   231,   232,
     142,   234,     9,   165,   178,   236,   123,   183,    99,   186,
     219,    10,   220,    50,    99,   229,   238,   222,  -124,   224,
     240,   227,   225,   233,   249,   235,    50,   256,   237,   254,
     252,   261,    31,   177,    99,   239,   253,    32,    33,    34,
      50,    77,    28,    29,    30,   100,   101,   102,   103,   104,
       0,    35,     0,   102,   103,   104,    36,    37,     0,    38,
       0,    50,   248,   255,    50,    50,   257,   258,    39,    40,
       0,   100,   101,   102,   103,   104,     0,     0,    41,    42,
       0,    99,     0,     0,     0,    43,     0,     0,   123,    31,
       0,     0,    44,  -124,    32,    33,    34,     0,    27,    28,
      29,    30,     0,   251,    45,     0,     0,    46,    35,    47,
       0,     0,     0,    36,    37,     0,    38,     0,   100,   101,
     102,   103,   104,     0,     0,    39,    40,     0,     0,     0,
    -124,  -124,  -124,  -124,     0,    41,    42,     0,     0,     0,
       0,     0,    43,     0,    27,    28,    29,    30,     0,    78,
       0,    32,    33,    34,    88,    89,    90,    91,    92,     0,
      69,    45,     0,    99,    46,    35,    47,   174,     0,     0,
      36,    37,     0,    38,     0,   202,     0,     0,   134,    28,
      29,    30,    39,    40,     0,     0,     0,   203,     0,   204,
     205,     0,    41,    42,     0,     0,     0,    32,    33,    34,
     100,   101,   102,   103,     0,     0,    70,     0,     0,     0,
       0,    35,     0,   123,     0,     0,    36,    37,    45,    38,
       0,    46,     0,    47,     0,     0,     0,     0,    39,    40,
       0,    32,    33,    34,     0,     0,     0,     0,    41,    42,
       3,     4,     5,     6,     7,    35,     8,     0,     0,     0,
      36,    37,    44,    38,   206,     0,   207,   208,   209,   210,
     211,   212,    39,    40,    45,   156,     0,    46,     0,    47,
       0,     0,    41,    42,     0,     0,     9,     0,   213,    88,
      89,    90,    91,    92,     0,    10,    44,   124,    99,   125,
     126,   127,   128,   129,   130,     0,   214,     0,    45,     0,
       0,    46,     0,    47,     0,     3,     4,     5,     6,     7,
       0,     8,     3,     4,     5,     6,     7,     0,     8,     0,
       0,     0,     0,     0,     0,   100,   101,   102,   103,   104,
     157,    64,    64,    64,    64,     0,     0,   259,     0,    68,
       0,     9,    88,    89,    90,    91,    92,     0,     9,     0,
      10,     0,     0,    93,     0,     0,     0,    10,     0,     0,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
      64
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-95))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      10,     3,    10,    56,    57,    96,    10,    34,    15,    85,
      10,   104,    61,    62,    18,    19,   110,    44,    61,    62,
      47,    97,    22,    15,    24,    25,    24,    25,    26,   105,
     124,    15,     3,    43,   111,   112,    99,   100,   101,   102,
     103,   106,    15,    70,   106,    72,   106,   112,   106,   112,
     112,    78,   112,     3,   112,   106,     0,    70,    85,    72,
       3,   112,    66,   112,    62,    75,    76,    75,    76,     3,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   104,
       3,   158,   106,    90,    94,    95,    94,    95,   112,    89,
     107,   118,    92,    93,   121,   107,   123,    59,    90,    61,
      62,   107,   107,    15,   131,   115,    90,   114,   121,    15,
     104,   108,   139,   113,   124,   115,   124,    90,   111,   114,
     127,   128,   129,   130,   126,   111,   136,   131,   138,    15,
     138,   131,   122,   111,   225,   125,    80,    81,    82,    83,
      84,   111,    86,   127,   128,   129,   130,   131,    60,    89,
     160,   178,   160,   111,     3,     4,     5,     6,   131,   186,
     106,   106,   189,   109,    89,   178,   219,   194,   195,   196,
       4,   198,   116,   121,   110,   202,    15,   112,    90,   111,
     104,   125,   111,   193,    90,   193,   213,     3,    15,   105,
     217,   110,   114,     3,   221,   106,   206,     4,   206,   235,
     227,   110,    51,   136,    90,   216,   233,    56,    57,    58,
     220,     3,     4,     5,     6,   127,   128,   129,   130,   131,
      -1,    70,    -1,   129,   130,   131,    75,    76,    -1,    78,
      -1,   241,   220,   241,   244,   245,   244,   245,    87,    88,
      -1,   127,   128,   129,   130,   131,    -1,    -1,    97,    98,
      -1,    90,    -1,    -1,    -1,   104,    -1,    -1,    15,    51,
      -1,    -1,   111,    90,    56,    57,    58,    -1,     3,     4,
       5,     6,    -1,   112,   123,    -1,    -1,   126,    70,   128,
      -1,    -1,    -1,    75,    76,    -1,    78,    -1,   127,   128,
     129,   130,   131,    -1,    -1,    87,    88,    -1,    -1,    -1,
     127,   128,   129,   130,    -1,    97,    98,    -1,    -1,    -1,
      -1,    -1,   104,    -1,     3,     4,     5,     6,    -1,   111,
      -1,    56,    57,    58,    99,   100,   101,   102,   103,    -1,
      65,   123,    -1,    90,   126,    70,   128,   112,    -1,    -1,
      75,    76,    -1,    78,    -1,    10,    -1,    -1,     3,     4,
       5,     6,    87,    88,    -1,    -1,    -1,    22,    -1,    24,
      25,    -1,    97,    98,    -1,    -1,    -1,    56,    57,    58,
     127,   128,   129,   130,    -1,    -1,   111,    -1,    -1,    -1,
      -1,    70,    -1,    15,    -1,    -1,    75,    76,   123,    78,
      -1,   126,    -1,   128,    -1,    -1,    -1,    -1,    87,    88,
      -1,    56,    57,    58,    -1,    -1,    -1,    -1,    97,    98,
      80,    81,    82,    83,    84,    70,    86,    -1,    -1,    -1,
      75,    76,   111,    78,    89,    -1,    91,    92,    93,    94,
      95,    96,    87,    88,   123,   105,    -1,   126,    -1,   128,
      -1,    -1,    97,    98,    -1,    -1,   116,    -1,   113,    99,
     100,   101,   102,   103,    -1,   125,   111,    89,    90,    91,
      92,    93,    94,    95,    96,    -1,   131,    -1,   123,    -1,
      -1,   126,    -1,   128,    -1,    80,    81,    82,    83,    84,
      -1,    86,    80,    81,    82,    83,    84,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     105,    23,    24,    25,    26,    -1,    -1,   105,    -1,    31,
      -1,   116,    99,   100,   101,   102,   103,    -1,   116,    -1,
     125,    -1,    -1,   110,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      62
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   134,     0,    80,    81,    82,    83,    84,    86,   116,
     125,   135,   136,   138,   139,   140,   141,   142,   143,   147,
     151,   156,   158,     3,     3,     3,     3,     3,     4,     5,
       6,    51,    56,    57,    58,    70,    75,    76,    78,    87,
      88,    97,    98,   104,   111,   123,   126,   128,   161,   168,
     177,   178,   180,   182,   183,   184,   104,   104,   107,   107,
     107,   107,     3,   160,   161,   160,   160,   160,   161,    65,
     111,   170,   173,   177,   180,   111,   111,     3,   111,   164,
     165,   166,   168,   179,   180,   111,   111,   180,    99,   100,
     101,   102,   103,   110,   175,   176,   114,   108,    15,    90,
     127,   128,   129,   130,   131,   111,   161,   134,   134,   160,
     152,   148,   144,   110,   124,   169,   170,   180,    59,    61,
      62,   172,   170,    15,    89,    91,    92,    93,    94,    95,
      96,   171,   168,   168,     3,   167,   106,   162,    89,   106,
     112,   179,     4,   168,   168,   178,   179,   180,   180,   180,
     180,   180,   180,   180,   179,   181,   105,   105,   157,   122,
     125,   174,   174,   174,   137,   121,   177,   112,   180,   170,
     180,   168,   180,   112,   112,   106,   112,   165,   110,   163,
     168,   180,   112,   112,   109,   112,   111,   174,   168,    10,
      22,    24,    25,    89,    92,    93,   113,   115,   131,   153,
     154,   155,    10,    22,    24,    25,    89,    91,    92,    93,
      94,    95,    96,   113,   131,   149,   150,   159,   145,   104,
     111,    60,     3,   170,   105,   114,   180,   110,   180,   168,
     180,   180,   180,     3,   180,   106,   180,   168,   180,   149,
     180,    10,    18,    19,    66,   131,   146,   134,   164,   180,
     178,   112,   180,   180,   155,   168,     4,   168,   168,   105,
     112,   110
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
        case 7:

/* Line 1806 of yacc.c  */
#line 142 "sml.tab.ypp"
    {   
				YAC_MODEL_LOG("blockblock: BLOCK ID indexing_opt COLON LBRACE statements RBRACE --- rule matched!");
				add_indexing((yyvsp[(3) - (4)].opPtrIx));
				begin_model((char*)(yyvsp[(2) - (4)].string),(yyvsp[(3) - (4)].opPtrIx));
			}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 148 "sml.tab.ypp"
    {
               	end_model((char*)(yyvsp[(2) - (8)].string));
               	rem_indexing((yyvsp[(3) - (8)].opPtrIx));
            }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 180 "sml.tab.ypp"
    {
               end_smodel(NULL);
               rem_indexing(NULL);
            }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 190 "sml.tab.ypp"
    {
                     add_indexing((yyvsp[(3) - (9)].opPtrIx));
                     begin_smodel((char*)(yyvsp[(2) - (9)].string), (yyvsp[(3) - (9)].opPtrIx), (yyvsp[(7) - (9)].opPtr));
                  }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 200 "sml.tab.ypp"
    {
               stages_glo = NULL;
            }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 208 "sml.tab.ypp"
    {              
                     if (!is_stoch_model) { 
                        cerr << "Syntax Error: keyword 'stages' can only be "
                        "used in stochastic blocks" << endl;
                        exit(1);
                     }
                     stages_glo = (yyvsp[(2) - (3)].opPtr);
                  }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 224 "sml.tab.ypp"
    {                  
				YAC_MODEL_LOG("parsed setdef:"<<(yyvsp[(1) - (2)].string));
			}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 228 "sml.tab.ypp"
    {                  
				YAC_MODEL_LOG("parsed paramdef:"<<(yyvsp[(1) - (2)].string));
			}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 232 "sml.tab.ypp"
    {                  
				YAC_MODEL_LOG("parsed vardef:"<<(yyvsp[(1) - (2)].string));
			}
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 236 "sml.tab.ypp"
    {                  
				YAC_MODEL_LOG("parsed cnstr:"<<(yyvsp[(1) - (2)].string));
			}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 246 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("setdef:[SET ID indexing_opt] - ID=["<<(yyvsp[(2) - (3)].string)<<"] indexing_opt=["<<(yyvsp[(3) - (3)].opPtrIx)<<"]");
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
        }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 251 "sml.tab.ypp"
    {
            YAC_MODEL_LOG("stochattr_opt=[" << (yyvsp[(5) - (6)].opPtr)<<"]");
            YAC_MODEL_LOG("setattributes_opt=[" <<(yyvsp[(6) - (6)].opPtr)<<"]");
            ModelComp *newmc;
            if (is_stoch_model){
               newmc = new StochModelComp((yyvsp[(2) - (6)].string), TSET, (yyvsp[(3) - (6)].opPtrIx), (yyvsp[(6) - (6)].opPtr));
               addStochInfo(newmc, (yyvsp[(5) - (6)].opPtr));
            }else{
               newmc = new ModelComp((yyvsp[(2) - (6)].string), TSET, (yyvsp[(3) - (6)].opPtrIx), (yyvsp[(6) - (6)].opPtr));
            }
            current_model->addComp(newmc);
            if ((yyvsp[(3) - (6)].opPtrIx)) rem_indexing((yyvsp[(3) - (6)].opPtrIx));
            (yyval.string)=(yyvsp[(2) - (6)].string); 
        }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 267 "sml.tab.ypp"
    { (yyval.opPtr)=NULL; }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 269 "sml.tab.ypp"
    {
                 		if((yyvsp[(1) - (2)].opPtr)!=NULL)
                 		{
                 			if((yyvsp[(1) - (2)].opPtr)->opCode != COMMA)
                 			{
                 				(yyvsp[(1) - (2)].opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (2)].opPtr));
                 			}
                 			assert((yyvsp[(2) - (2)].opPtr)!=NULL);
                 			(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
                 		}
                 		(yyval.opPtr) = (yyvsp[(1) - (2)].opPtr);
                 	}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 285 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(DIMEN, new ValueNode(*(yyvsp[(2) - (2)].ival)));
	           	}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 289 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(WITHIN, (yyvsp[(2) - (2)].opPtr));
				}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 293 "sml.tab.ypp"
    {
					YAC_MODEL_LOG("setattribute: ASSIGN setexpression  -- "<<(yyvsp[(2) - (2)].opPtr));
					(yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(2) - (2)].opPtr));
	           	}
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 298 "sml.tab.ypp"
    {
                	(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));
               	}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 302 "sml.tab.ypp"
    { 
            		(yyval.opPtr) = new SyntaxNode(ORDERED); 
            	}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 310 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("paramdef: -- indexing_opt -- "<<(yyvsp[(3) - (3)].opPtrIx));
               if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
            }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 313 "sml.tab.ypp"
    {
               ModelComp *newmc;
               if (is_stoch_model){
               	  YAC_MODEL_LOG("paramdef: -- stochInfo --"<<(yyvsp[(5) - (6)].opPtr));
                  StochModelComp *newmcs = new StochModelComp((yyvsp[(2) - (6)].string),TPARAM,(yyvsp[(3) - (6)].opPtrIx),(yyvsp[(6) - (6)].opPtr));
                  addStochInfo(newmcs, (yyvsp[(5) - (6)].opPtr));
                  newmc = newmcs;
               }else{
               	  YAC_MODEL_LOG("create param comp - id["<<(yyvsp[(3) - (6)].opPtrIx)<<"]");
                  newmc = new ModelComp((yyvsp[(2) - (6)].string), TPARAM, (yyvsp[(3) - (6)].opPtrIx), (yyvsp[(6) - (6)].opPtr));
               }
               current_model->addComp(newmc);
               if ((yyvsp[(3) - (6)].opPtrIx)) rem_indexing((yyvsp[(3) - (6)].opPtrIx));
               (yyval.string) = (yyvsp[(2) - (6)].string);
            }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 330 "sml.tab.ypp"
    { (yyval.opPtr) = NULL; }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 332 "sml.tab.ypp"
    {
                   	 	if((yyvsp[(2) - (2)].opPtr)!=NULL)
                 		{
                 			if((yyvsp[(1) - (2)].opPtr)->opCode != COMMA)
                 			{
                 				(yyvsp[(1) - (2)].opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (2)].opPtr));
                 			}
                 			(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
                 		}
                 		(yyval.opPtr) = (yyvsp[(1) - (2)].opPtr);
                   	 }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 346 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 347 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 348 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 349 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode((yyvsp[(1) - (2)].optype), (yyvsp[(2) - (2)].opPtr));}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 350 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(IN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 351 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 352 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 359 "sml.tab.ypp"
    {
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 361 "sml.tab.ypp"
    {
            ModelComp *newmc;
            if (is_stoch_model){
               newmc = new StochModelComp((yyvsp[(2) - (6)].string), TVAR, (yyvsp[(3) - (6)].opPtrIx), (yyvsp[(6) - (6)].opPtr));
               addStochInfo(newmc, (yyvsp[(5) - (6)].opPtr));
            }else{
               newmc = new ModelComp((yyvsp[(2) - (6)].string), TVAR, (yyvsp[(3) - (6)].opPtrIx), (yyvsp[(6) - (6)].opPtr));
            }

            current_model->addComp(newmc);
            if ((yyvsp[(3) - (6)].opPtrIx)) rem_indexing((yyvsp[(3) - (6)].opPtrIx));
            (yyval.string)=(yyvsp[(2) - (6)].string); 
         }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 376 "sml.tab.ypp"
    {(yyval.opPtr)=NULL;}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 377 "sml.tab.ypp"
    {(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 382 "sml.tab.ypp"
    {
					(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); 
               }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 386 "sml.tab.ypp"
    {
               		if((yyvsp[(1) - (3)].opPtr)->opCode != COMMA)
               		{
               			(yyvsp[(1) - (3)].opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (3)].opPtr));
               		}
             		(yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
             		(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
               }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 396 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 397 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 398 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 399 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(LE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 400 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(GE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 401 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 402 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 403 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 404 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(IN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 405 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SUFFIX, new IDNode((yyvsp[(2) - (3)].string)), (yyvsp[(3) - (3)].opPtr));}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 412 "sml.tab.ypp"
    {
        	if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
        }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 416 "sml.tab.ypp"
    {
			ModelComp *newmc;
            if (is_stoch_model){
               newmc = new StochModelComp((yyvsp[(2) - (7)].string), (yyvsp[(1) - (7)].ctype), (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
               addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
            }else{
               newmc = new ModelComp((yyvsp[(2) - (7)].string), (yyvsp[(1) - (7)].ctype), (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
            }
            current_model->addComp(newmc);
            if ((yyvsp[(3) - (7)].opPtrIx)) rem_indexing((yyvsp[(3) - (7)].opPtrIx));
			(yyval.string) =(yyvsp[(2) - (7)].string);
		}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 430 "sml.tab.ypp"
    { (yyval.ctype) = TMAX;  }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 431 "sml.tab.ypp"
    { (yyval.ctype) = TMIN;  }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 432 "sml.tab.ypp"
    { (yyval.ctype) = TCON;  }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 435 "sml.tab.ypp"
    {(yyval.optype)=LE;}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 436 "sml.tab.ypp"
    {(yyval.optype)=GE;}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 437 "sml.tab.ypp"
    {(yyval.optype)=LT;}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 438 "sml.tab.ypp"
    {(yyval.optype)=GT;}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 439 "sml.tab.ypp"
    {(yyval.optype)=ASSIGN;}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 440 "sml.tab.ypp"
    {(yyval.optype)=EQ;}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 441 "sml.tab.ypp"
    {(yyval.optype)=NE;}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 444 "sml.tab.ypp"
    {		(yyval.opPtrIx)=NULL;	}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 445 "sml.tab.ypp"
    { (yyval.opPtrIx) = (yyvsp[(1) - (1)].opPtrIx); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 449 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("indexing -- rule matched!");
			SyntaxNodeIx* indx = new SyntaxNodeIx(new SyntaxNode(LBRACE, (yyvsp[(2) - (2)].opPtr)));
			add_indexing(indx);
			(yyval.opPtrIx) = indx;
		}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 456 "sml.tab.ypp"
    {	
        	SyntaxNode *tmp = NULL;
      		if((yyvsp[(4) - (5)].opPtr)!=NULL)
           	{
           		tmp = new SyntaxNode(LBRACE, new SyntaxNode(COLON, (yyvsp[(2) - (5)].opPtr), (yyvsp[(4) - (5)].opPtr)));
           	}
           	else
           	{
           		tmp = new SyntaxNode(LBRACE, (yyvsp[(2) - (5)].opPtr));
           	}
           	rem_indexing((yyvsp[(3) - (5)].opPtrIx));
           	(yyval.opPtrIx) = new SyntaxNodeIx(tmp);
           	YAC_MODEL_LOG("indexing -- rule end");
		}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 472 "sml.tab.ypp"
    { (yyval.opPtr)=NULL; }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 474 "sml.tab.ypp"
    {
				  		(yyval.opPtr)=(yyvsp[(2) - (2)].opPtr);
				  	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 480 "sml.tab.ypp"
    {  (yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 482 "sml.tab.ypp"
    {
                  if ((yyvsp[(1) - (3)].opPtr)->opCode != COMMA)
                  {
                     (yyvsp[(1) - (3)].opPtr) = new SyntaxNode(COMMA, (yyvsp[(1) - (3)].opPtr));
                  }
                  (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
                  (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
			}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 492 "sml.tab.ypp"
    {  (yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 494 "sml.tab.ypp"
    {
              	(yyval.opPtr) = new OpNode(IN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
           	}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 499 "sml.tab.ypp"
    { (yyval.opPtr) = new IDNode((yyvsp[(1) - (1)].string)); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 500 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr);}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 503 "sml.tab.ypp"
    { (yyval.opPtr) = new IDNode((yyvsp[(1) - (1)].string)); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 505 "sml.tab.ypp"
    {
			   		if((yyvsp[(1) - (3)].opPtr)->opCode != COMMA)
			   		{
			   			(yyvsp[(1) - (3)].opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (3)].opPtr));
			   		}
			   		(yyvsp[(1) - (3)].opPtr)->push_back(new IDNode((yyvsp[(3) - (3)].string)));
			   		(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
			   }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 517 "sml.tab.ypp"
    {
					YAC_MODEL_LOG("setexpression: indexing -- "<<(yyvsp[(1) - (1)].opPtrIx));
					(yyval.opPtr)= (yyvsp[(1) - (1)].opPtrIx);
				}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 521 "sml.tab.ypp"
    {
	     		YAC_MODEL_LOG("setexpression: identifier -- rule matched");
	     		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
             }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 526 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 529 "sml.tab.ypp"
    {
                  	(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 532 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(DOTDOT,(yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 536 "sml.tab.ypp"
    { 
	         		YAC_MODEL_LOG("setexpression - setof --  "<<(yyvsp[(2) - (2)].opPtrIx));
	         		add_indexing((yyvsp[(2) - (2)].opPtrIx));
	         		(yyval.opPtrIx) = (yyvsp[(2) - (2)].opPtrIx);
	         	}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 540 "sml.tab.ypp"
    {
	         		(yyval.opPtr) = new SyntaxNode(SETOF,(yyvsp[(2) - (4)].opPtrIx),(yyvsp[(4) - (4)].opPtr));
	         		rem_indexing((yyvsp[(3) - (4)].opPtrIx));
	            }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 547 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("lexpr: expr IN setexpression - rule matched! "<<(yyvsp[(1) - (3)].opPtr)<<" "<<(yyvsp[(3) - (3)].opPtr));
			(yyval.opPtr) = new OpNode(IN,(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
		}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 551 "sml.tab.ypp"
    {
            (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 554 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype),(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 558 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (2)].optype),(yyvsp[(2) - (2)].opPtr));
     	}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 562 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 567 "sml.tab.ypp"
    { (yyval.optype) = NE; }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 568 "sml.tab.ypp"
    { (yyval.optype) = LE; }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 569 "sml.tab.ypp"
    { (yyval.optype) = GE; }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 570 "sml.tab.ypp"
    { (yyval.optype) = LT; }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 571 "sml.tab.ypp"
    { (yyval.optype) = GT; }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 572 "sml.tab.ypp"
    { (yyval.optype) = EQ; }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 575 "sml.tab.ypp"
    { (yyval.optype) = AND; }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 576 "sml.tab.ypp"
    {  (yyval.optype) = OR; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 578 "sml.tab.ypp"
    {  (yyval.optype) = NOT; }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 581 "sml.tab.ypp"
    {(yyval.opPtr) = NULL;}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 582 "sml.tab.ypp"
    {
                  // check that this is in a stochastic model
                  if (!is_stoch_model){ 
                     cerr << "Syntax Error: keyword 'DETERMINISTIC' can only"
                        "be used in stochastic blocks\n";
                     exit(1);
                  }
                  (yyval.opPtr) = new SyntaxNode(DETERMINISTIC);
               }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 591 "sml.tab.ypp"
    {
                  // check that this is in a stochastic model
                  if (!is_stoch_model){ 
                     cerr << "Syntax Error: keyword 'STAGES' can only be used"
                        "in stochastic blocks\n";
                     exit(1);
                  }
                  (yyval.opPtr) = (yyvsp[(2) - (2)].opPtr);
               }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 602 "sml.tab.ypp"
    { (yyval.optype) = DIFF; }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 603 "sml.tab.ypp"
    { (yyval.optype) = SYMDIFF; }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 604 "sml.tab.ypp"
    { (yyval.optype) = CROSS; }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 607 "sml.tab.ypp"
    { (yyval.optype) = UNION; }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 608 "sml.tab.ypp"
    { (yyval.optype) = INTER; }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 617 "sml.tab.ypp"
    {
               /* this is a simple identifier in global context */
               YAC_MODEL_LOG("identifier: iditem  -- "<<(yyvsp[(1) - (1)].opPtr));
               (yyval.opPtr) = SyntaxNode::find_var_ref_in_context(current_model, (yyvsp[(1) - (1)].opPtr));
            }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 622 "sml.tab.ypp"
    {
               /* identifier sets the context for the iditem:
                  The result of this is either a context setting or a
                  complete description of a variable.

                  Can implement this by simply adding context and argument lists
                
                  A variable reference should be represented internally as a
                  pointer to the referenced object in the model list
                  and a pointer to the list of arguments

                  This can be represented as a SyntaxNode with
                  ->opCode = ID;
                  ->nval = # of arguments
                  ->values[0] = pointer to entity in model list
                  ->values[1 - n] = arguments 

                  CONTEXT: iditems can be interpreted in two different contexts
                  1) is the global context (i.e. referring from the model
                     part that is currently defined)
                  2) is a local context that can be set by preceeding bits
                     of a dot'd expression. If a dot'd expression is 
                     parsed the flag 'local_context' should be set and 
                     'local_context' should be set to the current local
                     context. 
                  a context is expressed as a pointer to a model entity
               */
       
               /* identifier sets the context for the idem */
               /* this only works if the identifier is actually a reference 
                  to a submodel */

               if ((yyvsp[(1) - (3)].opPtr)->opCode != IDREFM) {
                  cerr << "Attempting to use dot specifier for something not an object:\n " << *((yyvsp[(1) - (3)].opPtr)) << "\n";
                  exit(1);
               }
               AmplModel* local_context = (((SyntaxNodeIDREF*)(yyvsp[(1) - (3)].opPtr))->ref)->other;

               (yyval.opPtr) = SyntaxNode::find_var_ref_in_context(local_context, (yyvsp[(3) - (3)].opPtr));

               YAC_MODEL_LOG("Merge identifier:"<< *((yyvsp[(1) - (3)].opPtr)) <<"-----> iditem:"<<*((yyvsp[(3) - (3)].opPtr)));
               /* merge argument lists */
               (yyval.opPtr)->merge((yyvsp[(1) - (3)].opPtr));
            }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 671 "sml.tab.ypp"
    {                               /* simple identifier */
			YAC_MODEL_LOG("iditem : ID -- "<<(yyvsp[(1) - (1)].string));
            (yyval.opPtr)=new IDNode((yyvsp[(1) - (1)].string));
         }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 675 "sml.tab.ypp"
    { /* subscripted id'fier */
            YAC_MODEL_LOG("iditem: iditem LSBRACKET expr_list RSBRACKET --  "<<(yyvsp[(1) - (4)].opPtr)<<" ["<<(yyvsp[(3) - (4)].opPtr)<<"]");
            (yyval.opPtr) = new SyntaxNode(LSBRACKET, (yyvsp[(1) - (4)].opPtr), (yyvsp[(3) - (4)].opPtr));
         }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 679 "sml.tab.ypp"
    {
            // the same as above, just different syntax "ancestor(1).xh[i]"
            // => need to change the ID node in iditem into a binary node
            //    with INT_VAL ($3) as the second argument
   			assert(is_stoch_model==true);
            // iditem is either an ID or a LSBRACKET node
            SyntaxNode *node = (yyvsp[(6) - (6)].opPtr);
            IDNode *idnode;
            if (node->opCode == LSBRACKET)
               idnode = (IDNode*) node->front();
            else
               idnode = (IDNode *) node;
            assert(idnode->opCode == ID);
            assert(idnode->getStochParent() == 0);
            idnode->setStochParent(*(yyvsp[(3) - (6)].ival));
            (yyval.opPtr) = (yyvsp[(6) - (6)].opPtr);
         }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 705 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("expr_list: expr rule matched! "<<(yyvsp[(1) - (1)].opPtr));
               	(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
            }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 709 "sml.tab.ypp"
    { 
               if((yyvsp[(1) - (3)].opPtr)->opCode != COMMA);
               {
               		(yyvsp[(1) - (3)].opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (3)].opPtr));
               }
               (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
            }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 718 "sml.tab.ypp"
    { YAC_MODEL_LOG("expr: value rule matched!" <<(yyvsp[(1) - (1)].opPtr)); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 719 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 720 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('+', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 721 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('-', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 722 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('-', (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 723 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('*', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 724 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('/', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 725 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(POWER, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 726 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(DOTDOT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 727 "sml.tab.ypp"
    {	(yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 729 "sml.tab.ypp"
    {	add_indexing((yyvsp[(2) - (2)].opPtrIx));}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 731 "sml.tab.ypp"
    {   
      	/*feng - need a bracket to handle current indexing? eg. sum{}(expr) - sum{}(expr) vs sum{}(expr - sum{}(expr))*/
        /* reduction operator: do we need to keep track of the ID of the dummy variable(s)? */
      	(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (6)].optype), (yyvsp[(2) - (6)].opPtrIx), (yyvsp[(5) - (6)].opPtr));
      	rem_indexing((yyvsp[(2) - (6)].opPtrIx));
      }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 737 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (4)].opPtr), (yyvsp[(4) - (4)].opPtr)); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 738 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (6)].opPtr), (yyvsp[(4) - (6)].opPtr), (yyvsp[(6) - (6)].opPtr)); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 739 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(FIRST, (yyvsp[(3) - (4)].opPtr)); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 740 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LAST, (yyvsp[(3) - (4)].opPtr)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 742 "sml.tab.ypp"
    { 
      	(yyval.opPtr) = new SyntaxNode(EXPECTATION, (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 746 "sml.tab.ypp"
    { 
      	(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (4)].optype), (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 751 "sml.tab.ypp"
    { (yyval.optype)=ORD;   }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 752 "sml.tab.ypp"
    { (yyval.optype)=CARD;  }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 753 "sml.tab.ypp"
    { (yyval.optype) = SIN; }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 754 "sml.tab.ypp"
    { (yyval.optype) = COS; }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 757 "sml.tab.ypp"
    { (yyval.optype)=SUM; }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 758 "sml.tab.ypp"
    { (yyval.optype)=MAX; }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 759 "sml.tab.ypp"
    { (yyval.optype)=MIN; }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 760 "sml.tab.ypp"
    { (yyval.optype)=PROD; }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 763 "sml.tab.ypp"
    {
            (yyval.opPtr)=new ValueNode(*(yyvsp[(1) - (1)].ival));
         }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 766 "sml.tab.ypp"
    { 
            (yyval.opPtr)=new ValueNode(*(yyvsp[(1) - (1)].fval));
         }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 769 "sml.tab.ypp"
    { 
     		YAC_MODEL_LOG("value: identifier rule matched! "<<(yyvsp[(1) - (1)].opPtr)->print());
            (yyval.opPtr)=(yyvsp[(1) - (1)].opPtr);
         }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 773 "sml.tab.ypp"
    { 
            (yyval.opPtr) = new SyntaxNode(INFINITE);
         }
    break;



/* Line 1806 of yacc.c  */
#line 3265 "../src/parser/sml.tab.cpp"
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
#line 778 "sml.tab.ypp"


void yyerror(const char *s) {
   cerr << "MODEL: " << s << " on line " << yylineno << "\n";
   //fprintf(stderr, "%s\n", s);
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
void begin_model(char *name, SyntaxNodeIx *indexing) 
{
	YAC_MODEL_LOG("begin_model  ["<< name <<"] indexing["<<indexing->print()<<"]");
	AmplModel *new_mod = new AmplModel(name,current_model);
	
	/* FIXME: include attrib in definition */
	ModelComp* newmc = new ModelComp(name, TMODEL, indexing, NULL);
	newmc->other = new_mod;
	
	new_mod->node = newmc;            /* add pointer-to-node to the model */
	current_model->addComp(newmc);
	  
	/* and change current model */
	current_model = new_mod;
}

/* ----------------------------------------------------------------------------
begin_smodel
---------------------------------------------------------------------------- */
void begin_smodel(char *name, SyntaxNodeIx *indexing, SyntaxNode *stochsets) 
{
   LOG_SM("Start Stochastic Model: " << name <<"indexing ["<<indexing->print()<<"]");
   if (!stochsets || stochsets->nchild()!=4){
      cerr << "Syntax error in Stochastic Block definition: \n";
      cerr << " 'USING' needs 4 parameters \n";
      exit(1);
   }

   SyntaxNode::iterator i = stochsets->begin();
   SyntaxNode *nodes = *i;
   SyntaxNode *anc = *(++i);
   SyntaxNode *prob = *(++i);
   SyntaxNode *stages = *(++i);
   LOG("   anc = [" << anc << endl << "]   prob = [" << prob<<"]"<< "   stages = [" << stages<<"]");
   StochModel* new_mod = new StochModel(name, stages, nodes, anc, prob, current_model);
  
   /* Fixme: include attrib in definition */
   ModelComp* newmc = new ModelComp(name, TMODEL, indexing, NULL);
   newmc->other = new_mod;

   new_mod->node = newmc;            /* add pointer-to-node to the model */
   current_model->addComp(newmc);
 
   /* and change current model */
   current_model = new_mod;
   is_stoch_model = true;
}

/* ----------------------------------------------------------------------------
end_model
---------------------------------------------------------------------------- */
void end_model(char *name) 
{
	YAC_MODEL_LOG("end_model -- ["<<current_model->name<<"]");
	// Check end block name matches block name
	if (name && string(name) != current_model->name) {
		cerr << "end block '" << name << "' encountered in block '" << 
		current_model->name << "'" << endl;
		exit(1);
	}
	current_model = current_model->parent;
	assert(current_model!=NULL);
}

/* ----------------------------------------------------------------------------
end_smodel
---------------------------------------------------------------------------- */
void end_smodel(char *name){
  // current_model is a StochModel -> convert this into a tree a FlatModels

  // Check end block name matches block name
  if (name && name != current_model->name) {
    cerr << "end stochastic block '" << name << "' encountered in stochastic "
      "block '" << current_model->name << "'" << endl;
    exit(1);
  }

  // this is the ModelComp pointing to the StochModel
  ModelComp *mc = current_model->node; 
  
  // point that to the expanded flat model tree
  mc->other = current_model->expandToFlatModel();

  // and change the name of the ModelComp of this model to the name of the 
  // new (AmplModel) model. 
  // (this is a concatenation of the StochModel name and the name of the 
  // first stage)
  mc->id = mc->other->name;

  // and go back to the parent 
  current_model = current_model->parent;
  is_stoch_model = false;
  
  assert(AmplModel::root == current_model);
  //AmplModel::root->isStochastic = true;
}

/* ------------------------------------------------------------------------
add_indexing/rem_indexing
-------------------------------------------------------------------------- */
void  add_indexing(SyntaxNodeIx *indexing){
   YAC_MODEL_LOG("add_indexing -- indexing:["<< *indexing<<"] "<<SyntaxNode::print_SyntaxNodesymb(indexing));
   list_of_indexing[n_indexing] = indexing;
   YAC_MODEL_LOG("[add_index] - length of indexing now: " << n_indexing+1);
   n_indexing++;
}

void rem_indexing(SyntaxNodeIx *indexing){
	YAC_MODEL_LOG("rem_indexing -- " << *indexing);
	if (indexing){
    	assert(indexing==list_of_indexing[n_indexing-1]);
   	}
	n_indexing--;
   	YAC_MODEL_LOG("rem_indexing --  length now[" << n_indexing<<"]" );
}

/* ---------------------------------------------------------------------------
Stochastic model helper functions
---------------------------------------------------------------------------- */
void addStochInfo(ModelComp *newmcs, SyntaxNode *stochopt) {
   if(stochopt) {
     bool isDet = stochopt->opCode == DETERMINISTIC;
     YAC_MODEL_LOG("isDet["<<isDet<<"] is_deterministic_glo["<<is_deterministic_glo<<"]");
     newmcs->setDeterministic(isDet || is_deterministic_glo);
     newmcs->setStageSetNode(isDet ? stages_glo : stochopt);
   } else {
      newmcs->setDeterministic(is_deterministic_glo);
      newmcs->setStageSetNode(stages_glo);
   }
}

//yyin opened,and readable
int parse_model() 
{
	int errcode = 0;
	yyin = fopen(GlobalVariables::modelfilename.c_str(),"r");
	assert(errcode==0);
   	AmplModel::root = new AmplModel("root",NULL);
   	current_model = AmplModel::root;
   	is_stoch_model = false;
   	is_deterministic_glo = false;
   	stages_glo = NULL;

	YAC_MODEL_LOG("starting model parser.... ");
   	errcode = yyparse();
   	assert(errcode==0);
   	YAC_MODEL_LOG("model parser finished !");
   	
   	fclose(yyin);
   	assert(errcode==0);
	return errcode;
}

