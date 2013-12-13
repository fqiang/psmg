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
   #include "../model/ListNode.h"
   #include "../model/IDNode.h"
   #include "../model/ValueNode.h"
   #include "../model/SyntaxNodeIDREF.h"
   #include "../model/SetNode.h"
   #include "../model/SimpleSet.h"
   #include "../model/ListSet.h"
   //#include "../model/CompositeSet.h"
   #include "../model/IndexingSet.h"
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
   static AmplModel *local_context;    /* this is the LOCAL context */
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
#line 127 "../src/parser/sml.tab.cpp"

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
#line 75 "sml.tab.ypp"

  int optype;
  long *ival;
  double *fval;
  char *string;
  SyntaxNode *opPtr;
  SyntaxNodeIx *opPtrIx;



/* Line 293 of yacc.c  */
#line 428 "../src/parser/sml.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 440 "../src/parser/sml.tab.cpp"

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
#define YYLAST   623

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  159
/* YYNRULES -- Number of states.  */
#define YYNSTATES  302

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
      69,    70,    73,    76,    79,    82,    85,    87,    88,    96,
      97,   100,   102,   104,   106,   109,   112,   115,   118,   119,
     126,   127,   135,   136,   144,   145,   153,   155,   157,   159,
     161,   163,   165,   167,   171,   175,   181,   185,   191,   195,
     201,   205,   211,   215,   219,   223,   224,   226,   227,   233,
     234,   237,   239,   243,   245,   249,   251,   255,   257,   261,
     265,   267,   269,   273,   277,   281,   282,   287,   291,   295,
     299,   302,   306,   308,   310,   312,   314,   316,   318,   320,
     322,   324,   325,   326,   328,   331,   332,   334,   336,   340,
     342,   344,   346,   349,   352,   355,   358,   361,   364,   368,
     370,   372,   374,   376,   378,   380,   384,   386,   391,   398,
     400,   404,   406,   410,   414,   418,   421,   425,   429,   433,
     437,   438,   445,   450,   457,   462,   467,   472,   477,   479,
     481,   483,   485,   487,   489,   491,   493,   495,   497,   499
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     134,     0,    -1,    -1,   134,   142,    -1,   138,    -1,   136,
      -1,   140,    -1,    -1,   116,     3,   159,   110,   137,   104,
     134,   105,    -1,   139,   104,   134,   105,    -1,   116,     3,
     159,   124,   121,   111,   163,   112,   110,    -1,   141,   104,
     134,   105,    -1,   125,   167,   110,    -1,   143,   107,    -1,
     147,   107,    -1,   151,   107,    -1,   153,   107,    -1,   135,
      -1,    -1,    82,     3,   159,   144,   174,   145,    -1,    -1,
     146,   145,    -1,    18,     4,    -1,    66,   167,    -1,   131,
     167,    -1,    10,   167,    -1,    19,    -1,    -1,   173,    81,
       3,   159,   148,   174,   149,    -1,    -1,   150,   149,    -1,
      25,    -1,    24,    -1,    22,    -1,   157,   183,    -1,    89,
     167,    -1,    10,   183,    -1,   113,   183,    -1,    -1,    80,
       3,   159,   152,   174,   175,    -1,    -1,    83,     3,   159,
     154,   174,   110,   183,    -1,    -1,    84,     3,   159,   155,
     174,   110,   183,    -1,    -1,    86,     3,   159,   156,   174,
     110,   158,    -1,    92,    -1,    93,    -1,    95,    -1,    96,
      -1,   131,    -1,    94,    -1,    91,    -1,   183,   131,   183,
      -1,   183,    95,   183,    -1,   183,    95,   183,    95,   183,
      -1,   183,    92,   183,    -1,   183,    92,   183,    92,   183,
      -1,   183,    96,   183,    -1,   183,    96,   183,    96,   183,
      -1,   183,    93,   183,    -1,   183,    93,   183,    93,   183,
      -1,   183,    94,   183,    -1,   183,    91,   183,    -1,   169,
      61,   169,    -1,    -1,   160,    -1,    -1,   104,   163,   161,
     162,   105,    -1,    -1,   110,   169,    -1,   164,    -1,   163,
     106,   164,    -1,   167,    -1,   165,    89,   167,    -1,     3,
      -1,   111,   166,   112,    -1,     3,    -1,   166,   106,     3,
      -1,   104,   167,   105,    -1,   182,    -1,   180,    -1,   167,
     178,   167,    -1,   167,   179,   167,    -1,   183,    15,   183,
      -1,    -1,    51,   160,   168,   180,    -1,   183,    89,   167,
      -1,   111,   169,   112,    -1,   169,   171,   169,    -1,   172,
     169,    -1,   183,   170,   183,    -1,    91,    -1,    92,    -1,
      93,    -1,    95,    -1,    96,    -1,    94,    -1,    61,    -1,
      62,    -1,    65,    -1,    -1,    -1,   122,    -1,   125,   167,
      -1,    -1,   176,    -1,   177,    -1,   176,   106,   177,    -1,
      25,    -1,    24,    -1,    22,    -1,    92,   183,    -1,    93,
     183,    -1,   113,   183,    -1,   131,   183,    -1,    10,   183,
      -1,    89,   167,    -1,   115,     3,   183,    -1,   100,    -1,
     103,    -1,   101,    -1,    99,    -1,   102,    -1,   181,    -1,
     180,   114,   181,    -1,     3,    -1,     3,   108,   182,   109,
      -1,   126,   111,     4,   112,   114,   181,    -1,   183,    -1,
     182,   106,   183,    -1,   187,    -1,   111,   182,   112,    -1,
     183,   127,   183,    -1,   183,   128,   183,    -1,   128,   183,
      -1,   183,   129,   183,    -1,   183,   130,   183,    -1,   183,
      90,   183,    -1,   183,    15,   183,    -1,    -1,   186,   160,
     184,   111,   183,   112,    -1,    58,   169,    59,   183,    -1,
      58,   169,    59,   183,    60,   183,    -1,    75,   111,   167,
     112,    -1,    76,   111,   167,   112,    -1,   123,   111,   182,
     112,    -1,   185,   111,   182,   112,    -1,    78,    -1,    70,
      -1,    97,    -1,    98,    -1,    87,    -1,    57,    -1,    56,
      -1,    88,    -1,     4,    -1,     5,    -1,   180,    -1,     6,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   128,   128,   129,   132,   133,   134,   147,   147,   185,
     194,   205,   213,   228,   229,   230,   231,   232,   239,   238,
     259,   260,   271,   274,   277,   281,   284,   291,   291,   310,
     311,   321,   322,   323,   324,   325,   326,   327,   334,   334,
     354,   354,   369,   369,   384,   384,   405,   406,   407,   408,
     409,   410,   411,   414,   415,   416,   420,   421,   425,   426,
     430,   431,   435,   436,   437,   442,   443,   447,   446,   470,
     471,   478,   479,   488,   489,   495,   496,   499,   500,   516,
     520,   525,   530,   533,   536,   540,   539,   551,   555,   557,
     561,   565,   571,   572,   573,   574,   575,   576,   579,   580,
     582,   586,   614,   615,   624,   635,   636,   640,   642,   648,
     649,   650,   651,   652,   653,   654,   655,   659,   660,   665,
     666,   667,   670,   671,   680,   685,   735,   739,   744,   769,
     773,   786,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   795,   801,   802,   803,   804,   805,   809,   814,   815,
     816,   817,   820,   821,   822,   823,   826,   829,   832,   836
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
  "paramattributes_opt", "paramattribute", "vardef", "$@4", "cnstr", "$@5",
  "$@6", "$@7", "relop", "equation", "indexing_opt", "indexing", "@8",
  "indexing_condition", "setexpr_list", "setexpr_item", "mdim_dummy",
  "mdim_dummy_list", "setexpression", "@9", "lexpr", "b_compare_op",
  "blogic_op", "ulogic_op", "qualifier", "stochattr_opt",
  "varattributes_opt", "varattributes", "varattribute", "bsetop",
  "ubsetop", "identifier", "iditem", "expr_list", "expr", "$@10",
  "func_op", "reduction_op", "value", 0
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
     154,   153,   155,   153,   156,   153,   157,   157,   157,   157,
     157,   157,   157,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   159,   159,   161,   160,   162,
     162,   163,   163,   164,   164,   165,   165,   166,   166,   167,
     167,   167,   167,   167,   167,   168,   167,   169,   169,   169,
     169,   169,   170,   170,   170,   170,   170,   170,   171,   171,
     172,   173,   174,   174,   174,   175,   175,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   178,
     178,   178,   179,   179,   180,   180,   181,   181,   181,   182,
     182,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     184,   183,   183,   183,   183,   183,   183,   183,   185,   185,
     185,   185,   186,   186,   186,   186,   187,   187,   187,   187
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     0,     8,     4,
       9,     4,     3,     2,     2,     2,     2,     1,     0,     6,
       0,     2,     2,     2,     2,     2,     1,     0,     7,     0,
       2,     1,     1,     1,     2,     2,     2,     2,     0,     6,
       0,     7,     0,     7,     0,     7,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     5,     3,     5,     3,     5,
       3,     5,     3,     3,     3,     0,     1,     0,     5,     0,
       2,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     1,     3,     3,     3,     0,     4,     3,     3,     3,
       2,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     0,     1,     2,     0,     1,     1,     3,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     3,     1,     4,     6,     1,
       3,     1,     3,     3,     3,     2,     3,     3,     3,     3,
       0,     6,     4,     6,     4,     4,     4,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,   101,     1,     0,     0,     0,     0,     0,     0,     0,
      17,     5,     4,     0,     6,     0,     3,     0,     0,     0,
       0,     0,    65,    65,    65,    65,    65,    65,   126,   156,
     157,   159,     0,   154,   153,     0,   149,     0,     0,   148,
     152,   155,   150,   151,     0,     0,     0,     0,     0,     0,
      81,   124,    80,   129,     0,     0,   131,     2,     2,    13,
      14,    15,    16,     0,     0,    38,    66,    18,    40,    42,
      44,     0,     0,    85,   100,     0,     0,     0,   158,     0,
       0,     0,     0,     0,   129,     0,     0,   135,   122,   119,
     121,   123,   120,    12,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   140,   101,   101,    65,   126,
       0,    67,    71,     0,    73,   102,   102,   102,   102,   102,
       7,     0,     0,     0,     0,   129,     0,    98,    99,     0,
      90,     0,     0,    92,    93,    94,    97,    95,    96,     0,
       0,     0,    79,   132,     0,     0,    82,    83,   125,   130,
      84,   138,   133,   134,   136,   137,     0,     0,     9,    11,
      27,   126,     0,     0,    69,     0,   103,     0,   105,    20,
       0,     0,     0,     0,     0,   127,    86,    88,   142,    89,
     139,    87,    91,   144,   145,   146,     0,   147,     0,   102,
       0,    76,    72,     0,     0,    74,   104,     0,   111,   110,
     109,     0,     0,     0,     0,     0,     0,    39,   106,   107,
       0,     0,    26,     0,     0,    19,    20,     0,     0,     0,
       2,     0,     0,     0,     0,    29,    78,    70,    68,   116,
     117,   112,   113,   114,     0,   115,     0,    25,    22,    23,
      24,    21,    41,    43,    45,     0,     0,   101,     0,   143,
     128,   141,     0,    33,    32,    31,     0,    52,    46,    47,
      51,    48,    49,     0,    50,    28,    29,     0,   118,   108,
       0,     0,     0,     0,     0,     0,     0,     0,     8,     0,
      36,    35,    37,    30,    34,    64,    63,    56,    60,    62,
      54,    58,    53,    10,     0,     0,     0,     0,    57,    61,
      55,    59
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    10,    11,   173,    12,    13,    14,    15,    16,
      17,   116,   215,   216,    18,   189,   265,   266,    19,   115,
      20,   117,   118,   119,   267,   244,    65,    66,   164,   194,
     111,   112,   113,   162,   114,   123,    76,   139,   129,    77,
      21,   168,   207,   208,   209,    94,    95,    78,    51,    52,
      53,   157,    54,    55,    56
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -114
static const yytype_int16 yypact[] =
{
    -114,   128,  -114,    18,    38,    55,    57,    71,    80,   220,
    -114,  -114,  -114,   -93,  -114,    13,  -114,   -14,    11,    19,
      23,    32,    21,    21,    21,    21,    21,    21,    24,  -114,
    -114,  -114,    21,  -114,  -114,   300,  -114,    20,    30,  -114,
    -114,  -114,  -114,  -114,   220,    22,    31,    33,    22,    78,
       9,  -114,    28,   423,    43,    21,  -114,  -114,  -114,  -114,
    -114,  -114,  -114,   140,   263,  -114,  -114,  -114,  -114,  -114,
    -114,   -63,    22,  -114,  -114,   300,   -43,   300,    35,   414,
     220,   220,   518,   -58,   427,    22,   152,    56,  -114,  -114,
    -114,  -114,  -114,  -114,   220,   220,     1,    22,    22,    22,
      22,    22,    22,    22,    22,  -114,   481,   486,    21,   -69,
     349,    54,  -114,    72,   315,  -113,  -113,  -113,  -113,  -113,
    -114,    51,    -1,     1,   -27,   414,    22,  -114,  -114,   300,
      45,    22,   220,  -114,  -114,  -114,  -114,  -114,  -114,    22,
     180,   332,  -114,  -114,   -53,    52,   315,   315,  -114,   427,
     455,    -9,    56,    56,    -9,    -9,   -30,    63,  -114,  -114,
    -114,   -57,   -22,   263,    66,   220,  -114,   220,   141,     4,
      84,    85,    86,    93,    89,  -114,    35,  -114,    62,    45,
    -114,   315,   427,  -114,  -114,  -114,    87,  -114,    22,  -113,
     200,  -114,  -114,   300,    99,   315,   315,    22,  -114,  -114,
    -114,   220,    22,    22,    22,   204,    22,  -114,   109,  -114,
     220,   212,  -114,   220,   220,  -114,     4,    22,    22,   300,
    -114,   263,    22,     1,   272,   370,  -114,    45,  -114,   427,
     315,   427,   427,   427,    22,   427,   141,   315,  -114,   315,
     315,  -114,   427,   427,  -114,    50,   362,   491,   -11,   427,
    -114,  -114,    22,  -114,  -114,  -114,   220,  -114,  -114,  -114,
    -114,  -114,  -114,    22,  -114,  -114,   370,    22,   427,  -114,
     300,    22,    22,    22,    22,    22,    22,    22,  -114,   110,
     427,   315,   427,  -114,   427,    45,   427,   351,   394,   427,
     405,   398,   427,  -114,    22,    22,    22,    22,   427,   427,
     427,   427
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -114,   -50,  -114,  -114,  -114,  -114,  -114,  -114,  -114,  -114,
    -114,  -114,     3,  -114,  -114,  -114,   -39,  -114,  -114,  -114,
    -114,  -114,  -114,  -114,  -114,  -114,     6,   -17,  -114,  -114,
      10,    82,  -114,  -114,     8,  -114,   -72,  -114,  -114,  -114,
    -114,   -73,  -114,  -114,    -7,  -114,  -114,    -8,   -94,   225,
     -35,  -114,  -114,  -114,  -114
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -159
static const yytype_int16 yytable[] =
{
      79,    50,   148,   124,    28,   130,   131,   106,   107,   166,
      84,    57,   167,    87,   210,    73,   126,    49,   127,   128,
     -75,    22,   211,   212,  -158,    28,    29,    30,    31,    67,
      68,    69,    70,    71,   127,   128,    50,    84,   105,    72,
     125,    23,    79,   169,   170,   171,   172,   120,    97,   -77,
      84,    72,    82,    97,   143,   -77,    50,   179,    24,   185,
      25,   121,   149,   150,   151,   152,   153,   154,   155,    84,
     213,   131,    50,    50,    26,    84,    97,   131,    33,    34,
      35,    99,   187,    27,   190,   177,    50,    50,   140,   141,
     191,   178,    36,    59,    79,   163,   180,    37,    38,  -158,
      39,   279,   146,   147,   182,    97,   127,   128,   175,    40,
      41,   270,   128,    63,   160,   176,   225,    58,    60,    42,
      43,   227,   222,    96,    50,    64,    61,    47,     2,   250,
      62,    80,    72,    45,    97,   214,  -158,  -158,  -158,  -158,
     181,    81,    85,   108,    86,    46,    99,   245,    47,    96,
      48,   197,    99,   224,   104,    50,   145,    50,    79,    50,
     163,   165,   229,   198,   186,   199,   200,   231,   232,   233,
     247,   235,   174,   195,   188,   196,   193,    88,    89,    90,
      91,    92,   242,   243,   246,   102,   103,   249,    93,   100,
     101,   102,   103,    50,   217,   218,   219,   220,   285,   268,
     221,   223,    50,   226,   228,    50,    50,   234,     3,   230,
       4,     5,     6,    50,     7,   236,   238,   280,   237,   241,
     293,   239,   240,    28,    29,    30,    31,   283,   282,   269,
     201,   248,   284,   202,   203,    79,   286,   287,   288,   289,
     290,   291,   292,     0,     8,   192,     0,     0,    50,     0,
       0,     0,     0,     9,   204,     0,   205,     0,     0,   298,
     299,   300,   301,     0,   281,     0,   109,    29,    30,    31,
      83,    32,   206,     0,     0,     0,    33,    34,    35,    88,
      89,    90,    91,    92,     0,     0,     0,   131,     0,     0,
      36,     0,   183,     0,     0,    37,    38,   122,    39,     0,
      83,     0,     0,    28,    29,    30,    31,    40,    41,     0,
     144,     0,     0,     0,    32,     0,     0,    42,    43,    33,
      34,    35,     0,     0,    44,     0,     0,     0,     0,   156,
       0,    45,     0,    36,     0,    83,     0,     0,    37,    38,
       0,    39,     0,    46,     0,     0,    47,     0,    48,     0,
      40,    41,   161,    29,    30,    31,    33,    34,    35,     0,
      42,    43,    99,     0,     0,    74,   131,    44,     0,     0,
      36,     0,     0,     0,   110,    37,    38,   131,    39,     0,
     252,     0,     0,     0,   251,     0,    46,    40,    41,    47,
       0,    48,   253,     0,   254,   255,     0,    42,    43,   100,
     101,   102,   103,     0,     0,    33,    34,    35,     0,   131,
       0,    75,     0,   131,    88,    89,    90,    91,    92,    36,
     131,     0,     0,    46,    37,    38,    47,    39,    48,   131,
       0,    88,    89,    90,    91,    92,    40,    41,    98,     0,
       0,    99,   131,   294,   184,     0,    42,    43,     0,     0,
       0,   132,    99,   271,   272,   273,   274,   275,   276,   256,
      45,   257,   258,   259,   260,   261,   262,     0,     0,     0,
    -139,     0,    46,     0,     0,    47,     0,    48,   100,   101,
     102,   103,     0,   263,    99,     0,     0,   295,    99,   100,
     101,   102,   103,   277,   297,    99,     0,     0,     0,     0,
     296,   264,     0,   132,    99,   133,   134,   135,   136,   137,
     138,     0,     0,    99,     0,     0,     0,    99,     0,     0,
       0,   100,   101,   102,   103,   100,   101,   102,   103,     0,
       0,     0,   100,   101,   102,   103,     0,     0,     0,     0,
       0,   100,   101,   102,   103,  -139,     0,     0,     0,     0,
     100,   101,   102,   103,   100,   101,   102,   103,     0,     0,
       0,     3,     0,     4,     5,     6,     3,     7,     4,     5,
       6,     3,     7,     4,     5,     6,     0,     7,     0,     0,
       0,     0,  -139,  -139,  -139,  -139,   158,     0,     0,     0,
       0,   159,     0,     0,     0,     0,   278,     8,     0,     0,
       0,     0,     8,     0,     0,     0,     9,     8,     0,     0,
       0,     9,     0,     0,     0,     0,     9,    88,    89,    90,
      91,    92,     0,   142
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-114))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      35,     9,    96,    75,     3,    77,    15,    57,    58,   122,
      45,   104,   125,    48,    10,    32,    59,     9,    61,    62,
      89,     3,    18,    19,    15,     3,     4,     5,     6,    23,
      24,    25,    26,    27,    61,    62,    44,    72,    55,   108,
      75,     3,    77,   116,   117,   118,   119,   110,   106,   106,
      85,   108,    44,   106,   112,   112,    64,   129,     3,   112,
       3,   124,    97,    98,    99,   100,   101,   102,   103,   104,
      66,    15,    80,    81,     3,   110,   106,    15,    56,    57,
      58,    90,   112,     3,   106,   112,    94,    95,    80,    81,
     112,   126,    70,   107,   129,   106,   131,    75,    76,    90,
      78,   112,    94,    95,   139,   106,    61,    62,   109,    87,
      88,    61,    62,    81,   108,   123,   189,   104,   107,    97,
      98,   193,    60,   114,   132,   104,   107,   126,     0,   223,
     107,   111,   108,   111,   106,   131,   127,   128,   129,   130,
     132,   111,   111,     3,   111,   123,    90,   219,   126,   114,
     128,    10,    90,   188,   111,   163,     4,   165,   193,   167,
     106,    89,   197,    22,   112,    24,    25,   202,   203,   204,
     220,   206,   121,   165,   111,   167,   110,    99,   100,   101,
     102,   103,   217,   218,   219,   129,   130,   222,   110,   127,
     128,   129,   130,   201,   110,   110,   110,   104,   270,   234,
     111,   114,   210,     3,   105,   213,   214,     3,    80,   201,
      82,    83,    84,   221,    86,   106,     4,   252,   210,   216,
     110,   213,   214,     3,     4,     5,     6,   266,   263,   236,
      89,   221,   267,    92,    93,   270,   271,   272,   273,   274,
     275,   276,   277,    -1,   116,   163,    -1,    -1,   256,    -1,
      -1,    -1,    -1,   125,   113,    -1,   115,    -1,    -1,   294,
     295,   296,   297,    -1,   256,    -1,     3,     4,     5,     6,
      45,    51,   131,    -1,    -1,    -1,    56,    57,    58,    99,
     100,   101,   102,   103,    -1,    -1,    -1,    15,    -1,    -1,
      70,    -1,   112,    -1,    -1,    75,    76,    72,    78,    -1,
      75,    -1,    -1,     3,     4,     5,     6,    87,    88,    -1,
      85,    -1,    -1,    -1,    51,    -1,    -1,    97,    98,    56,
      57,    58,    -1,    -1,   104,    -1,    -1,    -1,    -1,   104,
      -1,   111,    -1,    70,    -1,   110,    -1,    -1,    75,    76,
      -1,    78,    -1,   123,    -1,    -1,   126,    -1,   128,    -1,
      87,    88,     3,     4,     5,     6,    56,    57,    58,    -1,
      97,    98,    90,    -1,    -1,    65,    15,   104,    -1,    -1,
      70,    -1,    -1,    -1,   111,    75,    76,    15,    78,    -1,
      10,    -1,    -1,    -1,   112,    -1,   123,    87,    88,   126,
      -1,   128,    22,    -1,    24,    25,    -1,    97,    98,   127,
     128,   129,   130,    -1,    -1,    56,    57,    58,    -1,    15,
      -1,   111,    -1,    15,    99,   100,   101,   102,   103,    70,
      15,    -1,    -1,   123,    75,    76,   126,    78,   128,    15,
      -1,    99,   100,   101,   102,   103,    87,    88,    15,    -1,
      -1,    90,    15,    92,   112,    -1,    97,    98,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,    89,
     111,    91,    92,    93,    94,    95,    96,    -1,    -1,    -1,
      15,    -1,   123,    -1,    -1,   126,    -1,   128,   127,   128,
     129,   130,    -1,   113,    90,    -1,    -1,    93,    90,   127,
     128,   129,   130,   131,    96,    90,    -1,    -1,    -1,    -1,
      95,   131,    -1,    89,    90,    91,    92,    93,    94,    95,
      96,    -1,    -1,    90,    -1,    -1,    -1,    90,    -1,    -1,
      -1,   127,   128,   129,   130,   127,   128,   129,   130,    -1,
      -1,    -1,   127,   128,   129,   130,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,    90,    -1,    -1,    -1,    -1,
     127,   128,   129,   130,   127,   128,   129,   130,    -1,    -1,
      -1,    80,    -1,    82,    83,    84,    80,    86,    82,    83,
      84,    80,    86,    82,    83,    84,    -1,    86,    -1,    -1,
      -1,    -1,   127,   128,   129,   130,   105,    -1,    -1,    -1,
      -1,   105,    -1,    -1,    -1,    -1,   105,   116,    -1,    -1,
      -1,    -1,   116,    -1,    -1,    -1,   125,   116,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,   125,    99,   100,   101,
     102,   103,    -1,   105
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   134,     0,    80,    82,    83,    84,    86,   116,   125,
     135,   136,   138,   139,   140,   141,   142,   143,   147,   151,
     153,   173,     3,     3,     3,     3,     3,     3,     3,     4,
       5,     6,    51,    56,    57,    58,    70,    75,    76,    78,
      87,    88,    97,    98,   104,   111,   123,   126,   128,   167,
     180,   181,   182,   183,   185,   186,   187,   104,   104,   107,
     107,   107,   107,    81,   104,   159,   160,   159,   159,   159,
     159,   159,   108,   160,    65,   111,   169,   172,   180,   183,
     111,   111,   167,   182,   183,   111,   111,   183,    99,   100,
     101,   102,   103,   110,   178,   179,   114,   106,    15,    90,
     127,   128,   129,   130,   111,   160,   134,   134,     3,     3,
     111,   163,   164,   165,   167,   152,   144,   154,   155,   156,
     110,   124,   182,   168,   169,   183,    59,    61,    62,   171,
     169,    15,    89,    91,    92,    93,    94,    95,    96,   170,
     167,   167,   105,   112,   182,     4,   167,   167,   181,   183,
     183,   183,   183,   183,   183,   183,   182,   184,   105,   105,
     159,     3,   166,   106,   161,    89,   122,   125,   174,   174,
     174,   174,   174,   137,   121,   109,   180,   112,   183,   169,
     183,   167,   183,   112,   112,   112,   112,   112,   111,   148,
     106,   112,   164,   110,   162,   167,   167,    10,    22,    24,
      25,    89,    92,    93,   113,   115,   131,   175,   176,   177,
      10,    18,    19,    66,   131,   145,   146,   110,   110,   110,
     104,   111,    60,   114,   183,   174,     3,   169,   105,   183,
     167,   183,   183,   183,     3,   183,   106,   167,     4,   167,
     167,   145,   183,   183,   158,   169,   183,   134,   163,   183,
     181,   112,    10,    22,    24,    25,    89,    91,    92,    93,
      94,    95,    96,   113,   131,   149,   150,   157,   183,   177,
      61,    91,    92,    93,    94,    95,    96,   131,   105,   112,
     183,   167,   183,   149,   183,   169,   183,   183,   183,   183,
     183,   183,   183,   110,    92,    93,    95,    96,   183,   183,
     183,   183
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
#line 147 "sml.tab.ypp"
    {   
				YAC_MODEL_LOG("blockblock: BLOCK ID indexing_opt COLON LBRACE statements RBRACE --- rule matched!");
				add_indexing((yyvsp[(3) - (4)].opPtrIx));
				begin_model((char*)(yyvsp[(2) - (4)].string),(yyvsp[(3) - (4)].opPtrIx));
			}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 153 "sml.tab.ypp"
    {
               	end_model((char*)(yyvsp[(2) - (8)].string));
               	rem_indexing((yyvsp[(3) - (8)].opPtrIx));
            }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 185 "sml.tab.ypp"
    {
               end_smodel(NULL);
               rem_indexing(NULL);
            }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 195 "sml.tab.ypp"
    {
                     add_indexing((yyvsp[(3) - (9)].opPtrIx));
                     begin_smodel((char*)(yyvsp[(2) - (9)].string), (yyvsp[(3) - (9)].opPtrIx), (yyvsp[(7) - (9)].opPtr));
                  }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 205 "sml.tab.ypp"
    {
               stages_glo = NULL;
            }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 213 "sml.tab.ypp"
    {              
                     if (!is_stoch_model) { 
                        cerr << "Syntax Error: keyword 'stages' can only be "
                        "used in stochastic blocks" << endl;
                        exit(1);
                     }
                     stages_glo = (yyvsp[(2) - (3)].opPtr);
                  }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 239 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("setdef:[SET ID indexing_opt] - ID=["<<(yyvsp[(2) - (3)].string)<<"] indexing_opt=["<<(yyvsp[(3) - (3)].opPtrIx)<<"]");
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
        }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 243 "sml.tab.ypp"
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
            //$$=$2; 
        }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 259 "sml.tab.ypp"
    {(yyval.opPtr)=NULL;}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 261 "sml.tab.ypp"
    {
                 		if((yyvsp[(2) - (2)].opPtr)!=NULL)
                 		{
                 			(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
                 		}
                 		(yyval.opPtr)=(yyvsp[(1) - (2)].opPtr);
                 	}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 271 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new SyntaxNode(DIMEN, new ValueNode(*(yyvsp[(2) - (2)].ival)));
               }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 274 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new SyntaxNode(WITHIN, (yyvsp[(2) - (2)].opPtr));
               }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 277 "sml.tab.ypp"
    {
            		YAC_MODEL_LOG("match setexpression define rule");
                  (yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(2) - (2)].opPtr));
               }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 281 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));
               }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 284 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(ORDERED); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 291 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("paramdef: -- indexing_opt -- "<<(yyvsp[(4) - (4)].opPtrIx));
               if ((yyvsp[(4) - (4)].opPtrIx)) add_indexing((yyvsp[(4) - (4)].opPtrIx));
            }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 294 "sml.tab.ypp"
    {
               ModelComp *newmc;
               if (is_stoch_model){
               	  YAC_MODEL_LOG("paramdef: -- stochInfo --"<<(yyvsp[(6) - (7)].opPtr));
                  StochModelComp *newmcs = new StochModelComp((yyvsp[(3) - (7)].string),TPARAM,(yyvsp[(4) - (7)].opPtrIx),(yyvsp[(7) - (7)].opPtr));
                  addStochInfo(newmcs, (yyvsp[(6) - (7)].opPtr));
                  newmc = newmcs;
               }else{
               	  YAC_MODEL_LOG("create param comp - id["<<(yyvsp[(3) - (7)].string)<<"]");
                  newmc = new ModelComp((yyvsp[(3) - (7)].string), TPARAM, (yyvsp[(4) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
               }
               current_model->addComp(newmc);
               if ((yyvsp[(4) - (7)].opPtrIx)) rem_indexing((yyvsp[(4) - (7)].opPtrIx));
            }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 310 "sml.tab.ypp"
    { (yyval.opPtr) = NULL; }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 312 "sml.tab.ypp"
    {
                   	 	if((yyvsp[(2) - (2)].opPtr)!=NULL) {
                   	 		(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
                   	 	}
                   	 	(yyval.opPtr)=(yyvsp[(1) - (2)].opPtr);
                   	 }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 321 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 322 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 323 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 324 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode((yyvsp[(1) - (2)].optype), (yyvsp[(2) - (2)].opPtr));}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 325 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(IN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 326 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 327 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 334 "sml.tab.ypp"
    {
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 336 "sml.tab.ypp"
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
            //$$=$2; 
         }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 354 "sml.tab.ypp"
    {
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 356 "sml.tab.ypp"
    {
            ModelComp *newmc;
            if (is_stoch_model){
               newmc = new StochModelComp((yyvsp[(2) - (7)].string), TMAX, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
               addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
            }else{
               newmc = new ModelComp((yyvsp[(2) - (7)].string), TMAX, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
            }

            current_model->addComp(newmc);
            if ((yyvsp[(3) - (7)].opPtrIx)) rem_indexing((yyvsp[(3) - (7)].opPtrIx));
            (yyval.string)=(yyvsp[(2) - (7)].string);
         }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 369 "sml.tab.ypp"
    {
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 371 "sml.tab.ypp"
    {
            ModelComp *newmc;
            if (is_stoch_model){
               newmc = new StochModelComp((yyvsp[(2) - (7)].string), TMIN, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
               addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
            }else{
               newmc = new ModelComp((yyvsp[(2) - (7)].string), TMIN, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
            }

            current_model->addComp(newmc);
            if ((yyvsp[(3) - (7)].opPtrIx)) rem_indexing((yyvsp[(3) - (7)].opPtrIx));
            (yyval.string)=(yyvsp[(2) - (7)].string);
         }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 384 "sml.tab.ypp"
    { 
     		YAC_MODEL_LOG("subject to rule matched - ID["<<(yyvsp[(2) - (3)].string)<<"]");
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 387 "sml.tab.ypp"
    {
         	YAC_MODEL_LOG("subject to rule part2");
         	ModelComp *newmc;
            if (is_stoch_model){
            	YAC_MODEL_LOG("stoch model subject to");
               	newmc = new StochModelComp((yyvsp[(2) - (7)].string), TCON, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
               	addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
            }
            else{
            	YAC_MODEL_LOG("non-stoch model subject to - ");
               	newmc = new ModelComp((yyvsp[(2) - (7)].string), TCON, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
            }
            current_model->addComp(newmc);
            if ((yyvsp[(3) - (7)].opPtrIx)) rem_indexing((yyvsp[(3) - (7)].opPtrIx));
            (yyval.string)=(yyvsp[(2) - (7)].string);
         }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 405 "sml.tab.ypp"
    {(yyval.optype)=LE;}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 406 "sml.tab.ypp"
    {(yyval.optype)=GE;}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 407 "sml.tab.ypp"
    {(yyval.optype)=LT;}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 408 "sml.tab.ypp"
    {(yyval.optype)=GT;}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 409 "sml.tab.ypp"
    {(yyval.optype)=ASSIGN;}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 410 "sml.tab.ypp"
    {(yyval.optype)=EQ;}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 411 "sml.tab.ypp"
    {(yyval.optype)=NE;}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 414 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 415 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(LT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 416 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(LT, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(LT, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 420 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(LE, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 421 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(LE, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(LE, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 425 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(GT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 426 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(GT, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(GT, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 430 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(GE, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 431 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(GE, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(GE, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 435 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(EQ, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 436 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(NE,(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 437 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(AND, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 442 "sml.tab.ypp"
    {		(yyval.opPtrIx)=NULL;	}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 443 "sml.tab.ypp"
    { (yyval.opPtrIx) = (yyvsp[(1) - (1)].opPtrIx); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 447 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("indexing -- rule matched!");
			SyntaxNodeIx* indx = new SyntaxNodeIx(new SyntaxNode(LBRACE, (yyvsp[(2) - (2)].opPtr)));
			add_indexing(indx);
			(yyval.opPtrIx) = indx;
		}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 454 "sml.tab.ypp"
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

  case 69:

/* Line 1806 of yacc.c  */
#line 470 "sml.tab.ypp"
    { (yyval.opPtr)=NULL; }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 472 "sml.tab.ypp"
    {
				  		(yyval.opPtr)=(yyvsp[(2) - (2)].opPtr);
				  	}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 480 "sml.tab.ypp"
    {
                  if ((yyvsp[(1) - (3)].opPtr)->getOpCode() == COMMA)
                     (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
                  else
                     (yyval.opPtr) = new ListNode(COMMA, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
			}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 490 "sml.tab.ypp"
    {
              	(yyval.opPtr) = new OpNode(IN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
           	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 495 "sml.tab.ypp"
    { (yyval.opPtr) = new IDNode((yyvsp[(1) - (1)].string)); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 496 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr);}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 499 "sml.tab.ypp"
    { (yyval.opPtr) = new IDNode((yyvsp[(1) - (1)].string)); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 501 "sml.tab.ypp"
    {
			   		if((yyvsp[(1) - (3)].opPtr)->getOpCode()==COMMA)
			   		{
			   			(yyvsp[(1) - (3)].opPtr)->push_back(new IDNode((yyvsp[(3) - (3)].string)));
			   			(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
			   		}
			   		else
			   		{
			   			(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (3)].opPtr),new IDNode((yyvsp[(3) - (3)].string)));
			   		}
			   }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 516 "sml.tab.ypp"
    {
					YAC_MODEL_LOG("setexpression: LBRACE setexpression RBRACE -- rule matched");
					(yyval.opPtr)= new ListSet((yyvsp[(2) - (3)].opPtr));
				}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 521 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("create ListSet");
				(yyval.opPtr) = new ListSet((yyvsp[(1) - (1)].opPtr));
			 }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 525 "sml.tab.ypp"
    {
	     		YAC_MODEL_LOG("setexpression: identifier -- rule matched");
	     		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
             }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 530 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 533 "sml.tab.ypp"
    {
                  	(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 536 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SimpleSet((yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 540 "sml.tab.ypp"
    { 
	         		YAC_MODEL_LOG("setexpression - setof --  "<<(yyvsp[(2) - (2)].opPtrIx));
	         		add_indexing((yyvsp[(2) - (2)].opPtrIx));
	         		(yyval.opPtrIx) = (yyvsp[(2) - (2)].opPtrIx);
	         	}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 544 "sml.tab.ypp"
    {
	         		(yyval.opPtr) = new SyntaxNode(SETOF,(yyvsp[(2) - (4)].opPtrIx),(yyvsp[(4) - (4)].opPtr));
	         		rem_indexing((yyvsp[(3) - (4)].opPtrIx));
	            }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 551 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("lexpr: expr IN setexpression - rule matched! "<<(yyvsp[(1) - (3)].opPtr)<<" "<<(yyvsp[(3) - (3)].opPtr));
			(yyval.opPtr) = new OpNode(IN,(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
		}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 555 "sml.tab.ypp"
    {
            (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 558 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype),(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 562 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (2)].optype),(yyvsp[(2) - (2)].opPtr));
     	}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 566 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 571 "sml.tab.ypp"
    { (yyval.optype) = NE; }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 572 "sml.tab.ypp"
    { (yyval.optype) = LE; }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 573 "sml.tab.ypp"
    { (yyval.optype) = GE; }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 574 "sml.tab.ypp"
    { (yyval.optype) = LT; }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 575 "sml.tab.ypp"
    { (yyval.optype) = GT; }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 576 "sml.tab.ypp"
    { (yyval.optype) = EQ; }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 579 "sml.tab.ypp"
    { (yyval.optype) = AND; }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 580 "sml.tab.ypp"
    {  (yyval.optype) = OR; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 582 "sml.tab.ypp"
    {  (yyval.optype) = NOT; }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 614 "sml.tab.ypp"
    {(yyval.opPtr) = NULL;}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 615 "sml.tab.ypp"
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

  case 104:

/* Line 1806 of yacc.c  */
#line 624 "sml.tab.ypp"
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

  case 105:

/* Line 1806 of yacc.c  */
#line 635 "sml.tab.ypp"
    {(yyval.opPtr)=NULL;}
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 636 "sml.tab.ypp"
    {(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 640 "sml.tab.ypp"
    {
                  (yyval.opPtr) = addItemToListOrCreate(COMMA, NULL, (yyvsp[(1) - (1)].opPtr));}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 642 "sml.tab.ypp"
    {
                  (yyval.opPtr) = addItemToListOrCreate(COMMA, (ListNode*)(yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 648 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 649 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 650 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 651 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(LE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 652 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(GE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 653 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 654 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 655 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 659 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(IN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 660 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SUFFIX, (yyvsp[(3) - (3)].opPtr));}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 665 "sml.tab.ypp"
    { (yyval.optype) = DIFF; }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 666 "sml.tab.ypp"
    { (yyval.optype) = SYMDIFF; }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 667 "sml.tab.ypp"
    { (yyval.optype) = CROSS; }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 670 "sml.tab.ypp"
    { (yyval.optype) = UNION; }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 671 "sml.tab.ypp"
    { (yyval.optype) = INTER; }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 680 "sml.tab.ypp"
    {
               /* this is a simple identifier in global context */
               YAC_MODEL_LOG("identifier: iditem  -- "<<(yyvsp[(1) - (1)].opPtr));
               (yyval.opPtr) = SyntaxNode::find_var_ref_in_context(current_model, (yyvsp[(1) - (1)].opPtr));
            }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 685 "sml.tab.ypp"
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

               if ((yyvsp[(1) - (3)].opPtr)->getOpCode() != IDREFM) {
                  cerr << "Attempting to use dot specifier for something "
                     "not an object:\n " << *((yyvsp[(1) - (3)].opPtr)) << "\n";
                  exit(1);
               }
               local_context = (((SyntaxNodeIDREF*)(yyvsp[(1) - (3)].opPtr))->ref)->other;

               (yyval.opPtr) = SyntaxNode::find_var_ref_in_context(local_context, (yyvsp[(3) - (3)].opPtr));

               YAC_MODEL_LOG("Merge identifier:"<< *((yyvsp[(1) - (3)].opPtr)) <<"-----> iditem:"<<*((yyvsp[(3) - (3)].opPtr)));
               /* merge argument lists */
               (yyval.opPtr)->merge((yyvsp[(1) - (3)].opPtr));
            }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 735 "sml.tab.ypp"
    {                               /* simple identifier */
			YAC_MODEL_LOG("iditem : ID -- "<<(yyvsp[(1) - (1)].string));
            (yyval.opPtr)=new IDNode((yyvsp[(1) - (1)].string));
         }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 739 "sml.tab.ypp"
    { /* subscripted id'fier */
            YAC_MODEL_LOG("ID "<<(yyvsp[(1) - (4)].string)<<" ["<<(yyvsp[(3) - (4)].opPtr)<<"]");
            (yyval.opPtr) = new SyntaxNode(LSBRACKET, new IDNode((yyvsp[(1) - (4)].string)), (yyvsp[(3) - (4)].opPtr));
            //printf("%s\n", print_SyntaxNodesymb($$));
         }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 744 "sml.tab.ypp"
    {
            // the same as above, just different syntax "ancestor(1).xh[i]"
            // => need to change the ID node in iditem into a binary node
            //    with INT_VAL ($3) as the second argument
   
            // iditem is either an ID or a LSBRACKET node
            SyntaxNode *node = (yyvsp[(6) - (6)].opPtr);
            IDNode *idnode;
            if (node->getOpCode() == LSBRACKET)
               idnode = (IDNode*) node->front();
            else
               idnode = (IDNode *) node;
            assert(idnode->getOpCode() == ID);
            assert(idnode->getStochParent() == 0);
            idnode->setStochParent(*(yyvsp[(3) - (6)].ival));
            (yyval.opPtr) = (yyvsp[(6) - (6)].opPtr);
         }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 769 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("expr_list: expr rule matched! "<<(yyvsp[(1) - (1)].opPtr));
               (yyval.opPtr) = new ListNode(COMMA, (yyvsp[(1) - (1)].opPtr));
            }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 773 "sml.tab.ypp"
    {      /* add item to list */
               /* epxr_list could be a simple node or a comma separated
                  list (CSL) already 
                  - if it is a simple node, need to start a comma separated list
                  - if it is a CSL need to add an item to it
               */
               //printf("join nodes >%s< >%s<\n",
               //print_SyntaxNode($1),print_SyntaxNode($3));
               assert((yyvsp[(1) - (3)].opPtr)->getOpCode() == COMMA);
               (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
            }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 786 "sml.tab.ypp"
    { YAC_MODEL_LOG("expr: value rule matched!" <<(yyvsp[(1) - (1)].opPtr)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 787 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LBRACKET, (yyvsp[(2) - (3)].opPtr)); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 788 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('+', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 789 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('-', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 790 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('-', (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 791 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('*', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 792 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('/', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 793 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(POWER, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 794 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(DOTDOT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 795 "sml.tab.ypp"
    {add_indexing((yyvsp[(2) - (2)].opPtrIx));}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 795 "sml.tab.ypp"
    {   /*feng - need a bracket to handle current indexing? eg. sum{}(expr) - sum{}(expr) vs sum{}(expr - sum{}(expr))*/
         /* reduction operator: do we need to keep track of the ID of the
            dummy variable(s)? */
         (yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (6)].optype), (yyvsp[(2) - (6)].opPtrIx), (yyvsp[(5) - (6)].opPtr));
         rem_indexing((yyvsp[(2) - (6)].opPtrIx));
      }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 801 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (4)].opPtr), (yyvsp[(4) - (4)].opPtr)); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 802 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (6)].opPtr), (yyvsp[(4) - (6)].opPtr), (yyvsp[(6) - (6)].opPtr)); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 803 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(FIRST, (yyvsp[(3) - (4)].opPtr)); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 804 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LAST, (yyvsp[(3) - (4)].opPtr)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 805 "sml.tab.ypp"
    { 
         //$$ = new SyntaxNode(EXPECTATION, new SyntaxNode(LBRACKET, $3));}
         (yyval.opPtr) = new SyntaxNode(EXPECTATION, (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 809 "sml.tab.ypp"
    { /* function definition */
         (yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (4)].optype), new SyntaxNode(LBRACKET, (yyvsp[(3) - (4)].opPtr)));
      }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 814 "sml.tab.ypp"
    { (yyval.optype)=ORD;   }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 815 "sml.tab.ypp"
    { (yyval.optype)=CARD;  }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 816 "sml.tab.ypp"
    { (yyval.optype) = SIN; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 817 "sml.tab.ypp"
    { (yyval.optype) = COS; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 820 "sml.tab.ypp"
    { (yyval.optype)=SUM; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 821 "sml.tab.ypp"
    { (yyval.optype)=MAX; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 822 "sml.tab.ypp"
    { (yyval.optype)=MIN; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 823 "sml.tab.ypp"
    { (yyval.optype)=PROD; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 826 "sml.tab.ypp"
    {
            (yyval.opPtr)=new ValueNode(*(yyvsp[(1) - (1)].ival));
         }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 829 "sml.tab.ypp"
    { 
            (yyval.opPtr)=new ValueNode(*(yyvsp[(1) - (1)].fval));
         }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 832 "sml.tab.ypp"
    { 
     		YAC_MODEL_LOG("value: identifier rule matched! "<<(yyvsp[(1) - (1)].opPtr));
            (yyval.opPtr)=new SyntaxNode(0, (yyvsp[(1) - (1)].opPtr));
         }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 836 "sml.tab.ypp"
    { 
            (yyval.opPtr) = new SyntaxNode(INFINITE);
         }
    break;



/* Line 1806 of yacc.c  */
#line 3388 "../src/parser/sml.tab.cpp"
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
#line 841 "sml.tab.ypp"


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
	AmplModel *new_mod = new AmplModel(name,current_model);
	ModelComp *newmc;
	
	/* FIXME: include attrib in definition */
	newmc = new ModelComp(name, TMODEL, indexing, NULL);
	newmc->other = new_mod;
	
	new_mod->node = newmc;            /* add pointer-to-node to the model */
	current_model->addComp(newmc);
	  
	/* and change current model */
	current_model = new_mod;
	YAC_MODEL_LOG("begin_model  ["<< name <<"] indexing["<<indexing<<"]");
}

/* ----------------------------------------------------------------------------
begin_smodel
---------------------------------------------------------------------------- */
void begin_smodel(char *name, SyntaxNodeIx *indexing, SyntaxNode *stochsets) {
   StochModel *new_mod;
   ModelComp *newmc;
  
   ListNode *stochsetsl = static_cast<ListNode*>(stochsets);

   if (!stochsetsl || stochsetsl->nchild()!=4){
      cerr << "Syntax error in Stochastic Block definition: \n";
      cerr << " 'USING' needs 4 parameters \n";
      exit(1);
   }

   LOG_SM("Start Stochastic Model: " << name);

   ListNode::iterator i = stochsetsl->begin();
   SyntaxNode *nodes = *i;
   SyntaxNode *anc = *(++i);
   SyntaxNode *prob = *(++i);
   SyntaxNode *stages = *(++i);
   /*cout << "BEG SMODEL " << name << endl << "   nodes = " << nodes << endl;
   cout << "   anc = " << anc << endl << "   prob = " << prob << endl;
   cout << "   stages = " << stages << endl;*/
   new_mod = new StochModel(stages, nodes, anc, prob, current_model);
   new_mod->name = name;
  
   /* Fixme: include attrib in definition */
   newmc = new ModelComp(name, TMODEL, indexing, NULL);
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
     bool isDet = stochopt->getOpCode() == DETERMINISTIC;
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

