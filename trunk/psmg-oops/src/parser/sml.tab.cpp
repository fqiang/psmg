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
//   #include "../model/SymbolTable.h"
   #include "../model/SetNode.h"
   #include "../model/SimpleSet.h"
   #include "../model/ListSet.h"
   #include "../model/CompositeSet.h"
   #include "../model/IndexingSet.h"
   #include "../model/AmplModel.h"
  
   #include "../st_model/StochModel.h"
   #include "../st_model/StochModelComp.h"
   
   #include "../sml/GlobalVariables.h"
   #include "../util/global_util_functions.h"

   #ifdef HAVE_DIRECT_H
   #include <direct.h> // for chdir() under MinGW
   #endif

   using namespace std;

   void add_indexing(SyntaxNodeIx *indexing);
   void rem_indexing(SyntaxNodeIx *indexing);
   void begin_model(char *name, SyntaxNode *indexing);
   void end_model(char *name);
   void begin_smodel(char *name, SyntaxNode *indexing, SyntaxNode *stochsets);
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
#line 132 "../src/parser/sml.tab.cpp"

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
     INFINITY = 261,
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
     UNION = 352,
     DIFF = 353,
     CROSS = 354,
     INTER = 355,
     SYMDIFF = 356,
     LBRACE = 357,
     RBRACE = 358,
     COMMA = 359,
     SEMICOLON = 360,
     LSBRACKET = 361,
     RSBRACKET = 362,
     COLON = 363,
     LBRACKET = 364,
     RBRACKET = 365,
     DEFINED = 366,
     LOGICAL_OR = 367,
     LOGICAL_AND = 368,
     ELLIPSE = 369,
     DOT = 370,
     SUFFIX = 371,
     BLOCK = 372,
     IDREF = 373,
     IDREFM = 374,
     STAGE = 375,
     NODE = 376,
     USING = 377,
     DETERMINISTIC = 378,
     EXPECTATION = 379,
     STOCHASTIC = 380,
     STAGES = 381,
     ANCESTOR = 382,
     ASSIGN = 383
   };
#endif
/* Tokens.  */
#define ID 258
#define INT_VAL 259
#define FLOAT_VAL 260
#define INFINITY 261
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
#define UNION 352
#define DIFF 353
#define CROSS 354
#define INTER 355
#define SYMDIFF 356
#define LBRACE 357
#define RBRACE 358
#define COMMA 359
#define SEMICOLON 360
#define LSBRACKET 361
#define RSBRACKET 362
#define COLON 363
#define LBRACKET 364
#define RBRACKET 365
#define DEFINED 366
#define LOGICAL_OR 367
#define LOGICAL_AND 368
#define ELLIPSE 369
#define DOT 370
#define SUFFIX 371
#define BLOCK 372
#define IDREF 373
#define IDREFM 374
#define STAGE 375
#define NODE 376
#define USING 377
#define DETERMINISTIC 378
#define EXPECTATION 379
#define STOCHASTIC 380
#define STAGES 381
#define ANCESTOR 382
#define ASSIGN 383




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 80 "sml.tab.ypp"

  int optype;
  long *ival;
  double *fval;
  char *string;
  SyntaxNode *opPtr;
  SyntaxNodeIx *opPtrIx;



/* Line 293 of yacc.c  */
#line 435 "../src/parser/sml.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 447 "../src/parser/sml.tab.cpp"

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
#define YYLAST   787

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  134
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  163
/* YYNRULES -- Number of states.  */
#define YYNSTATES  323

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   383

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   133,     2,     2,     2,     2,     2,     2,
       2,     2,   131,   129,     2,   130,     2,   132,     2,     2,
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
     125,   126,   127,   128
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    17,    22,
      27,    31,    36,    40,    45,    55,    59,    64,    67,    72,
      76,    79,    82,    85,    88,    90,    91,    98,    99,   107,
     108,   115,   116,   124,   125,   133,   134,   142,   144,   146,
     148,   150,   152,   154,   156,   160,   164,   170,   174,   180,
     184,   190,   194,   200,   204,   208,   209,   211,   214,   219,
     222,   224,   228,   230,   234,   238,   241,   244,   248,   250,
     254,   256,   257,   258,   260,   262,   265,   268,   271,   274,
     277,   279,   283,   285,   287,   289,   293,   297,   301,   305,
     309,   313,   314,   316,   318,   322,   324,   326,   328,   331,
     334,   337,   340,   343,   344,   346,   349,   350,   352,   354,
     358,   360,   362,   364,   367,   370,   373,   376,   379,   382,
     386,   388,   390,   392,   394,   396,   398,   402,   404,   409,
     416,   423,   425,   429,   431,   435,   439,   443,   446,   450,
     454,   458,   462,   466,   470,   474,   477,   478,   485,   490,
     497,   502,   507,   512,   517,   519,   521,   523,   525,   527,
     529,   531,   533,   535
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     135,     0,    -1,    -1,   135,   146,    -1,   140,    -1,   137,
      -1,   144,    -1,   138,   135,   139,    -1,   138,   102,   135,
     103,    -1,   117,     3,   159,   108,    -1,    47,   117,   105,
      -1,    47,   117,     3,   105,    -1,   141,   135,   142,    -1,
     141,   102,   135,   103,    -1,   117,     3,   159,   125,   122,
     109,   162,   110,   108,    -1,    47,   143,   105,    -1,    47,
     143,     3,   105,    -1,   125,   117,    -1,   145,   102,   135,
     103,    -1,   126,   171,   108,    -1,   147,   105,    -1,   149,
     105,    -1,   151,   105,    -1,   153,   105,    -1,   136,    -1,
      -1,    82,     3,   159,   148,   175,   168,    -1,    -1,   167,
      81,     3,   159,   150,   175,   172,    -1,    -1,    80,     3,
     159,   152,   175,   176,    -1,    -1,    83,     3,   159,   154,
     175,   108,   184,    -1,    -1,    84,     3,   159,   155,   175,
     108,   184,    -1,    -1,    86,     3,   159,   156,   175,   108,
     158,    -1,    92,    -1,    93,    -1,    95,    -1,    96,    -1,
     128,    -1,    94,    -1,    91,    -1,   184,   128,   184,    -1,
     184,    95,   184,    -1,   184,    95,   184,    95,   184,    -1,
     184,    92,   184,    -1,   184,    92,   184,    92,   184,    -1,
     184,    96,   184,    -1,   184,    96,   184,    96,   184,    -1,
     184,    93,   184,    -1,   184,    93,   184,    93,   184,    -1,
     184,    94,   184,    -1,   184,    91,   184,    -1,    -1,   161,
      -1,   102,   162,    -1,   160,   108,   166,   103,    -1,   160,
     103,    -1,   163,    -1,   162,   104,   163,    -1,   171,    -1,
       3,    89,   171,    -1,   164,    89,   171,    -1,   165,   110,
      -1,   109,     3,    -1,   165,   104,     3,    -1,   184,    -1,
     109,   166,   110,    -1,   158,    -1,    -1,    -1,   169,    -1,
     170,    -1,   169,   170,    -1,    18,     4,    -1,    66,   171,
      -1,   111,   171,    -1,    10,   171,    -1,    19,    -1,   102,
     171,   103,    -1,   161,    -1,   183,    -1,   181,    -1,   171,
     179,   171,    -1,   171,   180,   171,    -1,   180,   161,   171,
      -1,   184,    15,   184,    -1,    51,   161,   181,    -1,   109,
     171,   110,    -1,    -1,   173,    -1,   174,    -1,   173,   104,
     174,    -1,    25,    -1,    24,    -1,    22,    -1,   157,   184,
      -1,    89,   171,    -1,   128,   184,    -1,    10,   184,    -1,
     111,   184,    -1,    -1,   123,    -1,   126,   171,    -1,    -1,
     177,    -1,   178,    -1,   177,   104,   178,    -1,    25,    -1,
      24,    -1,    22,    -1,    92,   184,    -1,    93,   184,    -1,
     111,   184,    -1,   128,   184,    -1,    10,   184,    -1,    89,
     171,    -1,   116,     3,   184,    -1,    98,    -1,   101,    -1,
      99,    -1,    97,    -1,   100,    -1,   182,    -1,   181,   115,
     182,    -1,     3,    -1,     3,   106,   183,   107,    -1,     3,
     109,     4,   105,   183,   110,    -1,   127,   109,     4,   110,
     115,   182,    -1,   184,    -1,   183,   104,   184,    -1,   188,
      -1,   109,   183,   110,    -1,   184,   129,   184,    -1,   184,
     130,   184,    -1,   130,   184,    -1,   184,   131,   184,    -1,
     184,   132,   184,    -1,   184,    90,   184,    -1,   184,   114,
     184,    -1,   184,   112,   184,    -1,   184,   113,   184,    -1,
     184,    91,   184,    -1,   133,   184,    -1,    -1,   187,   161,
     185,   109,   184,   110,    -1,    58,   166,    59,   184,    -1,
      58,   166,    59,   184,    60,   184,    -1,    75,   109,   171,
     110,    -1,    76,   109,   171,   110,    -1,   124,   109,   183,
     110,    -1,   186,   109,   183,   110,    -1,    78,    -1,    70,
      -1,    87,    -1,    57,    -1,    56,    -1,    88,    -1,     4,
      -1,     5,    -1,   181,    -1,     6,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   132,   132,   133,   136,   137,   138,   154,   155,   161,
     167,   171,   206,   207,   216,   227,   231,   237,   245,   253,
     268,   269,   270,   271,   272,   278,   278,   301,   301,   321,
     321,   341,   341,   356,   356,   371,   371,   392,   393,   394,
     395,   396,   397,   398,   401,   402,   403,   407,   408,   412,
     413,   417,   418,   422,   423,   428,   429,   434,   443,   449,
     460,   461,   469,   470,   474,   479,   482,   486,   491,   494,
     496,   502,   509,   510,   513,   514,   531,   534,   537,   541,
     544,   548,   552,   556,   577,   582,   585,   588,   592,   595,
     600,   608,   609,   612,   616,   629,   630,   631,   632,   633,
     634,   635,   636,   639,   640,   649,   660,   661,   665,   667,
     673,   674,   675,   676,   677,   678,   679,   680,   684,   685,
     690,   691,   692,   695,   696,   705,   710,   760,   764,   769,
     778,   803,   807,   820,   821,   822,   823,   824,   825,   826,
     827,   828,   829,   830,   833,   837,   838,   838,   844,   845,
     846,   847,   848,   852,   857,   858,   861,   862,   863,   864,
     867,   870,   873,   877
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "INT_VAL", "FLOAT_VAL", "INFINITY",
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
  "POWER", "NE", "LE", "GE", "EQ", "LT", "GT", "UNION", "DIFF", "CROSS",
  "INTER", "SYMDIFF", "LBRACE", "RBRACE", "COMMA", "SEMICOLON",
  "LSBRACKET", "RSBRACKET", "COLON", "LBRACKET", "RBRACKET", "DEFINED",
  "LOGICAL_OR", "LOGICAL_AND", "ELLIPSE", "DOT", "SUFFIX", "BLOCK",
  "IDREF", "IDREFM", "STAGE", "NODE", "USING", "DETERMINISTIC",
  "EXPECTATION", "STOCHASTIC", "STAGES", "ANCESTOR", "ASSIGN", "'+'",
  "'-'", "'*'", "'/'", "'!'", "$accept", "statements", "block",
  "blockblock", "blockblockbegin", "blockblockend", "stochblock",
  "stochblockbegin", "stochblockend", "sblock_alias", "stageblock",
  "stageblock_start", "statement", "setdef", "$@1", "paramdef", "$@2",
  "vardef", "$@3", "cnstr", "$@4", "$@5", "$@6", "relop", "equation",
  "indexing_opt", "start_indexing", "indexing", "setexpr_list",
  "setexpr_item", "mdim_dummy", "mdim_dummy_start", "lexpr", "qualifier",
  "setattributes_opt", "setattributes", "setattribute", "setexpression",
  "paramattributes_opt", "paramattributes", "paramattribute",
  "stochattr_opt", "varattributes_opt", "varattributes", "varattribute",
  "bsetop", "ubsetop", "identifier", "iditem", "expr_list", "expr", "$@7",
  "func", "reduction_op", "value", 0
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
     375,   376,   377,   378,   379,   380,   381,   382,   383,    43,
      45,    42,    47,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   134,   135,   135,   136,   136,   136,   137,   137,   138,
     139,   139,   140,   140,   141,   142,   142,   143,   144,   145,
     146,   146,   146,   146,   146,   148,   147,   150,   149,   152,
     151,   154,   153,   155,   153,   156,   153,   157,   157,   157,
     157,   157,   157,   157,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   159,   159,   160,   161,   161,
     162,   162,   163,   163,   163,   164,   165,   165,   166,   166,
     166,   167,   168,   168,   169,   169,   170,   170,   170,   170,
     170,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   172,   172,   173,   173,   174,   174,   174,   174,   174,
     174,   174,   174,   175,   175,   175,   176,   176,   177,   177,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     179,   179,   179,   180,   180,   181,   181,   182,   182,   182,
     182,   183,   183,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   185,   184,   184,   184,
     184,   184,   184,   184,   186,   186,   187,   187,   187,   187,
     188,   188,   188,   188
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     3,     4,     4,
       3,     4,     3,     4,     9,     3,     4,     2,     4,     3,
       2,     2,     2,     2,     1,     0,     6,     0,     7,     0,
       6,     0,     7,     0,     7,     0,     7,     1,     1,     1,
       1,     1,     1,     1,     3,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     3,     0,     1,     2,     4,     2,
       1,     3,     1,     3,     3,     2,     2,     3,     1,     3,
       1,     0,     0,     1,     1,     2,     2,     2,     2,     2,
       1,     3,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     0,     1,     1,     3,     1,     1,     1,     2,     2,
       2,     2,     2,     0,     1,     2,     0,     1,     1,     3,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     3,
       1,     1,     1,     1,     1,     1,     3,     1,     4,     6,
       6,     1,     3,     1,     3,     3,     3,     2,     3,     3,
       3,     3,     3,     3,     3,     2,     0,     6,     4,     6,
       4,     4,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,    71,     1,     0,     0,     0,     0,     0,     0,     0,
      24,     5,     2,     4,     2,     6,     0,     3,     0,     0,
       0,     0,     0,    55,    55,    55,    55,    55,    55,   127,
     160,   161,   163,     0,   158,   157,     0,   155,     0,     0,
     154,   156,   159,   123,   124,     0,     0,     0,     0,     0,
       0,     0,    82,     0,     0,    84,   125,    83,   131,     0,
       0,   133,     2,    71,     2,    71,     2,    20,    21,    22,
      23,     0,     0,    29,    56,    25,    31,    33,    35,     0,
       0,     0,     0,     0,    70,     0,   162,    68,     0,     0,
     127,     0,    57,    60,     0,     0,    62,     0,    83,     0,
       0,     0,   137,   145,    59,     0,   120,   122,   121,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   146,    71,     0,     7,
      71,     0,    12,    71,    55,    62,   103,   103,   103,   103,
     103,     9,     0,     0,   131,     0,    89,     0,     0,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   127,     0,     0,     0,    65,    81,    90,   134,     0,
       0,     0,    85,    86,    87,   126,   132,    88,   140,   144,
     142,   143,   141,   135,   136,   138,   139,     0,     0,     8,
       0,    13,     0,     0,    18,    27,   104,     0,   106,    72,
       0,     0,     0,     0,   128,     0,    69,   148,   144,    47,
      51,    53,    45,    49,    44,   150,   151,    63,    61,    64,
      67,   152,     0,    58,   153,     0,     0,    10,    17,     0,
      15,   103,   105,     0,   112,   111,   110,     0,     0,     0,
       0,     0,     0,    30,   107,   108,     0,     0,    80,     0,
       0,    26,    73,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,    16,    91,   117,
     118,   113,   114,   115,     0,   116,     0,    79,    76,    77,
      78,    75,    32,    34,    36,     0,     0,   129,   149,    48,
      52,    46,    50,   130,   147,     0,    97,    96,    95,     0,
      43,    37,    38,    42,    39,    40,     0,     0,     0,    28,
      92,    93,   119,   109,     0,   101,    99,   102,   100,    98,
       0,    14,    94
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    10,    11,    12,   129,    13,    14,   132,   193,
      15,    16,    17,    18,   137,    19,   231,    20,   136,    21,
     138,   139,   140,   308,    84,    73,    51,    52,    92,    93,
      94,    95,    85,    22,   251,   252,   253,   135,   309,   310,
     311,   198,   243,   244,   245,   110,    54,    86,    56,    57,
      58,   188,    59,    60,    61
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -132
static const yytype_int16 yypact[] =
{
    -132,    31,  -132,     9,    16,    23,    27,    50,    52,   251,
    -132,  -132,   -64,  -132,   -63,  -132,   -33,  -132,    -8,    18,
      41,    42,    14,    -6,    -6,    -6,    -6,    -6,    -6,   -52,
    -132,  -132,  -132,    -6,  -132,  -132,   386,  -132,    29,    40,
    -132,  -132,  -132,  -132,  -132,   307,   251,    49,    51,   424,
     424,   -15,  -132,   378,    -6,    60,  -132,    67,    94,    68,
      -6,  -132,  -132,   438,  -132,   459,  -132,  -132,  -132,  -132,
    -132,   166,   307,  -132,  -132,  -132,  -132,  -132,  -132,   -75,
     424,   179,    10,   386,  -132,   127,    90,   550,   251,   251,
     -60,   363,    83,  -132,   120,   -70,   660,   101,   -42,   424,
     206,   424,   -55,    98,  -132,   386,  -132,  -132,  -132,  -132,
     251,   251,   251,    10,   424,   424,   424,   424,   424,   424,
     424,   424,   424,   424,   424,   424,  -132,   -59,    96,  -132,
     443,    89,  -132,   510,    -6,    21,   -45,   -45,   -45,   -45,
     -45,  -132,    99,    36,   636,   115,    90,   112,   -42,   525,
     424,   424,   424,   424,   424,   424,   424,   424,   181,   300,
     251,    72,   307,   251,   228,  -132,  -132,  -132,  -132,   -39,
     122,   131,    21,    21,    21,  -132,   636,   636,   -86,    65,
     624,   655,  -132,   -55,   -55,   -86,   -86,   -21,   126,  -132,
      -1,  -132,   121,     0,  -132,  -132,  -132,   251,   445,    33,
     135,   139,   144,   130,  -132,   424,  -132,   475,   469,   518,
     575,   636,   579,   604,   636,  -132,  -132,    21,  -132,    21,
    -132,  -132,   143,  -132,  -132,   424,   154,  -132,  -132,   155,
    -132,   -45,    21,   424,  -132,  -132,  -132,   251,   424,   424,
     424,   258,   424,  -132,   159,  -132,   251,   264,  -132,   251,
     251,  -132,    33,  -132,   424,   424,   424,   307,   -18,   424,
     424,   424,   424,   424,    10,   611,  -132,  -132,   205,   636,
      21,   636,   636,   636,   424,   636,   445,    21,  -132,    21,
      21,  -132,   636,   636,  -132,   550,    35,  -132,   636,   636,
     636,   636,   636,  -132,  -132,   424,  -132,  -132,  -132,   251,
    -132,  -132,  -132,  -132,  -132,  -132,   424,   424,   424,  -132,
     165,  -132,   636,  -132,   167,   636,    21,   636,   636,   636,
     205,  -132,  -132
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -132,     8,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,  -132,    20,   -10,  -132,   319,    26,   124,
    -132,  -132,   -73,  -132,  -132,  -132,    32,    -4,  -132,  -132,
     -32,  -131,  -132,  -132,    13,  -132,    45,    -9,  -112,   -35,
      11,  -132,  -132,  -132,  -132
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -163
static const yytype_int16 yytable[] =
{
      55,   175,   226,   229,   116,    53,   199,   200,   201,   202,
     147,    98,    23,    29,    75,    76,    77,    78,    79,    24,
      63,     3,    65,     4,     5,     6,    25,     7,   120,   160,
      26,     2,   171,   141,   164,   116,    55,    55,    62,    64,
     165,    96,    97,   246,   189,   143,    80,    87,   148,    81,
     142,   247,   248,    27,    80,    28,    98,    81,     8,   120,
     102,   103,   114,    55,   169,   114,   148,     9,   168,    66,
     127,   221,   130,   146,   133,  -162,   123,   124,   196,    55,
      55,   197,    55,   114,   158,   159,   114,    97,   104,   224,
     187,   144,   287,   105,   149,    71,    72,    67,   111,   249,
     268,    55,    55,    55,   227,   230,   172,   173,   174,   115,
     144,     3,   144,     4,     5,     6,    87,     7,    43,   106,
     107,    44,   108,    68,   195,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   144,    48,    88,   162,
     114,   111,   111,   204,   250,   314,    69,    70,     8,    89,
    -162,    55,   293,    55,    55,   116,   217,     9,    99,   219,
     100,   207,   208,   209,   210,   211,   212,   213,   214,   134,
     258,   114,  -162,  -162,  -162,   113,   -66,   125,    80,   120,
     111,    81,   -66,   145,   116,   117,   150,   162,    55,  -162,
    -162,  -162,  -162,   232,   121,   122,   123,   124,    43,   106,
     107,    44,   108,   111,   111,   113,   118,   119,   120,   163,
     170,   167,   120,   190,   192,   295,   144,   111,   111,   111,
     205,   203,   206,   121,   122,   123,   124,   296,    55,   297,
     298,   220,   222,   270,   223,   225,   265,    55,   228,   257,
      55,    55,   277,   254,   269,   279,   280,   255,    55,   271,
     272,   273,   256,   275,    29,    30,    31,    32,   264,   266,
     267,   274,   111,   276,   111,   282,   283,   285,   278,   320,
     288,   289,   290,   291,   292,   321,   284,   111,    43,   106,
     107,    44,   108,   286,   281,   312,   218,     0,   322,   313,
      55,   215,     0,     0,   299,   316,   300,   301,   302,   303,
     304,   305,    33,     0,     0,     0,   315,    34,    35,    36,
      90,    30,    31,    32,     0,   111,   306,   317,   318,   319,
       0,    37,   111,     0,   111,   111,    38,    39,     0,    40,
       0,     0,     0,   307,     0,     0,     0,     0,    41,    42,
       0,     0,    74,    74,    74,    74,    74,    74,    43,     0,
       0,    44,    82,    45,     0,     0,     0,     0,    33,     0,
      46,   111,     0,    34,    35,    36,   161,    30,    31,    32,
       0,     0,     0,   112,     0,    47,     0,    37,    48,   126,
       0,    49,    38,    39,    50,    40,     0,     0,     0,    29,
      30,    31,    32,     0,    41,    42,     0,    43,   106,   107,
      44,   108,     0,     0,    43,     0,     0,    44,     0,    45,
     216,     0,     0,     0,    33,     0,    91,     0,     0,    34,
      35,    36,     0,     0,     0,     0,     0,    29,    30,    31,
      32,    47,     0,    37,    48,     0,     0,    49,    38,    39,
      50,    40,    34,    35,    36,     0,     0,     0,     0,     0,
      41,    42,     0,    74,     0,   233,    37,     0,     0,     0,
      43,    38,    39,    44,    40,    45,     0,   234,     0,   235,
     236,     0,    46,    41,    42,    43,   106,   107,    44,   108,
      34,    35,    36,     0,     0,   128,   109,    47,     0,     0,
      48,     0,     0,    49,    37,    83,    50,     0,     0,    38,
      39,     0,    40,     0,     0,     0,   131,     0,     0,     0,
      47,    41,    42,    48,     0,     0,    49,     0,     3,    50,
       4,     5,     6,     3,     7,     4,     5,     6,   -54,     7,
       0,     0,     0,   101,   237,   259,     0,   238,   239,     3,
       0,     4,     5,     6,     0,     7,   191,     0,    47,     0,
       0,    48,     0,     0,    49,     8,   240,    50,     0,   116,
       8,   241,     0,     0,     9,   116,   117,     0,     0,     9,
       0,     0,   -54,   242,   -54,     0,     8,     0,     0,   -54,
       0,     0,     0,   120,     0,     9,     0,   118,   119,   120,
       3,     0,     4,     5,     6,     0,     7,     0,   121,   122,
     123,   124,     0,     0,   121,   122,   123,   124,   116,   117,
     260,     0,     0,   194,     0,   116,   151,   152,   153,   154,
     155,   156,     0,     0,     0,     0,     0,     8,     0,  -131,
     118,   119,   120,     0,     0,     0,     9,   118,   119,   120,
     116,   151,   152,   153,   154,   155,   156,   121,   122,   123,
     124,     0,     0,   157,   121,   122,   123,   124,     0,     0,
       0,     0,   118,   119,   120,   116,   117,     0,   261,   116,
     117,     0,     0,     0,   262,     0,     0,     0,   157,   121,
     122,   123,   124,     0,     0,     0,     0,   118,   119,   120,
       0,   118,   119,   120,   116,   117,     0,     0,     0,     0,
     263,   116,   117,     0,   121,   122,   123,   124,   121,   122,
     123,   124,     0,     0,   116,   117,   118,   119,   120,     0,
       0,   294,     0,   118,   119,   120,   116,   117,     0,     0,
       0,     0,     0,   121,   122,   123,   124,   119,   120,     0,
     121,   122,   123,   124,     0,   116,   117,     0,   118,   119,
     120,     0,     0,   121,   122,   123,   124,    43,   106,   107,
      44,   108,     0,   166,     0,   121,   122,   123,   124,   120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   121,   122,   123,   124
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-132))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       9,   113,     3,     3,    90,     9,   137,   138,   139,   140,
      83,    46,     3,     3,    24,    25,    26,    27,    28,     3,
      12,    80,    14,    82,    83,    84,     3,    86,   114,    89,
       3,     0,   105,   108,   104,    90,    45,    46,   102,   102,
     110,    45,    46,    10,   103,    80,   106,    36,    83,   109,
     125,    18,    19,     3,   106,     3,    91,   109,   117,   114,
      49,    50,   104,    72,    99,   104,   101,   126,   110,   102,
      62,   110,    64,    82,    66,    15,   131,   132,   123,    88,
      89,   126,    91,   104,    88,    89,   104,    91,   103,   110,
     125,    80,   110,   108,    83,    81,   102,   105,    53,    66,
     231,   110,   111,   112,   105,   105,   110,   111,   112,    15,
      99,    80,   101,    82,    83,    84,   105,    86,    97,    98,
      99,   100,   101,   105,   134,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   127,   109,   104,
     104,    96,    97,   107,   111,   110,   105,   105,   117,   109,
      90,   160,   264,   162,   163,    90,   160,   126,   109,   163,
     109,   150,   151,   152,   153,   154,   155,   156,   157,     3,
     205,   104,   112,   113,   114,   115,   104,   109,   106,   114,
     135,   109,   110,     4,    90,    91,    59,   104,   197,   129,
     130,   131,   132,   197,   129,   130,   131,   132,    97,    98,
      99,   100,   101,   158,   159,   115,   112,   113,   114,    89,
       4,   110,   114,   117,   125,    10,   205,   172,   173,   174,
     105,   122,   110,   129,   130,   131,   132,    22,   237,    24,
      25,     3,   110,   237,   103,   109,   225,   246,   117,   109,
     249,   250,   246,   108,   233,   249,   250,   108,   257,   238,
     239,   240,   108,   242,     3,     4,     5,     6,   115,   105,
     105,     3,   217,   104,   219,   254,   255,   256,     4,   104,
     259,   260,   261,   262,   263,   108,   256,   232,    97,    98,
      99,   100,   101,   257,   252,   274,   162,    -1,   320,   276,
     299,   110,    -1,    -1,    89,   299,    91,    92,    93,    94,
      95,    96,    51,    -1,    -1,    -1,   295,    56,    57,    58,
       3,     4,     5,     6,    -1,   270,   111,   306,   307,   308,
      -1,    70,   277,    -1,   279,   280,    75,    76,    -1,    78,
      -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    23,    24,    25,    26,    27,    28,    97,    -1,
      -1,   100,    33,   102,    -1,    -1,    -1,    -1,    51,    -1,
     109,   316,    -1,    56,    57,    58,     3,     4,     5,     6,
      -1,    -1,    -1,    54,    -1,   124,    -1,    70,   127,    60,
      -1,   130,    75,    76,   133,    78,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    87,    88,    -1,    97,    98,    99,
     100,   101,    -1,    -1,    97,    -1,    -1,   100,    -1,   102,
     110,    -1,    -1,    -1,    51,    -1,   109,    -1,    -1,    56,
      57,    58,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,   124,    -1,    70,   127,    -1,    -1,   130,    75,    76,
     133,    78,    56,    57,    58,    -1,    -1,    -1,    -1,    -1,
      87,    88,    -1,   134,    -1,    10,    70,    -1,    -1,    -1,
      97,    75,    76,   100,    78,   102,    -1,    22,    -1,    24,
      25,    -1,   109,    87,    88,    97,    98,    99,   100,   101,
      56,    57,    58,    -1,    -1,    47,   108,   124,    -1,    -1,
     127,    -1,    -1,   130,    70,   109,   133,    -1,    -1,    75,
      76,    -1,    78,    -1,    -1,    -1,    47,    -1,    -1,    -1,
     124,    87,    88,   127,    -1,    -1,   130,    -1,    80,   133,
      82,    83,    84,    80,    86,    82,    83,    84,    59,    86,
      -1,    -1,    -1,   109,    89,    60,    -1,    92,    93,    80,
      -1,    82,    83,    84,    -1,    86,   103,    -1,   124,    -1,
      -1,   127,    -1,    -1,   130,   117,   111,   133,    -1,    90,
     117,   116,    -1,    -1,   126,    90,    91,    -1,    -1,   126,
      -1,    -1,   103,   128,   105,    -1,   117,    -1,    -1,   110,
      -1,    -1,    -1,   114,    -1,   126,    -1,   112,   113,   114,
      80,    -1,    82,    83,    84,    -1,    86,    -1,   129,   130,
     131,   132,    -1,    -1,   129,   130,   131,   132,    90,    91,
      92,    -1,    -1,   103,    -1,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,    -1,   117,    -1,   104,
     112,   113,   114,    -1,    -1,    -1,   126,   112,   113,   114,
      90,    91,    92,    93,    94,    95,    96,   129,   130,   131,
     132,    -1,    -1,   128,   129,   130,   131,   132,    -1,    -1,
      -1,    -1,   112,   113,   114,    90,    91,    -1,    93,    90,
      91,    -1,    -1,    -1,    95,    -1,    -1,    -1,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,   112,   113,   114,
      -1,   112,   113,   114,    90,    91,    -1,    -1,    -1,    -1,
      96,    90,    91,    -1,   129,   130,   131,   132,   129,   130,
     131,   132,    -1,    -1,    90,    91,   112,   113,   114,    -1,
      -1,   110,    -1,   112,   113,   114,    90,    91,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,   132,   113,   114,    -1,
     129,   130,   131,   132,    -1,    90,    91,    -1,   112,   113,
     114,    -1,    -1,   129,   130,   131,   132,    97,    98,    99,
     100,   101,    -1,   103,    -1,   129,   130,   131,   132,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   129,   130,   131,   132
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   135,     0,    80,    82,    83,    84,    86,   117,   126,
     136,   137,   138,   140,   141,   144,   145,   146,   147,   149,
     151,   153,   167,     3,     3,     3,     3,     3,     3,     3,
       4,     5,     6,    51,    56,    57,    58,    70,    75,    76,
      78,    87,    88,    97,   100,   102,   109,   124,   127,   130,
     133,   160,   161,   171,   180,   181,   182,   183,   184,   186,
     187,   188,   102,   135,   102,   135,   102,   105,   105,   105,
     105,    81,   102,   159,   161,   159,   159,   159,   159,   159,
     106,   109,   161,   109,   158,   166,   181,   184,   109,   109,
       3,   109,   162,   163,   164,   165,   171,   171,   183,   109,
     109,   109,   184,   184,   103,   108,    98,    99,   101,   108,
     179,   180,   161,   115,   104,    15,    90,    91,   112,   113,
     114,   129,   130,   131,   132,   109,   161,   135,    47,   139,
     135,    47,   142,   135,     3,   171,   152,   148,   154,   155,
     156,   108,   125,   183,   184,     4,   181,   166,   183,   184,
      59,    91,    92,    93,    94,    95,    96,   128,   171,   171,
      89,     3,   104,    89,   104,   110,   103,   110,   110,   183,
       4,   166,   171,   171,   171,   182,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   183,   185,   103,
     117,   103,   125,   143,   103,   159,   123,   126,   175,   175,
     175,   175,   175,   122,   107,   105,   110,   184,   184,   184,
     184,   184,   184,   184,   184,   110,   110,   171,   163,   171,
       3,   110,   110,   103,   110,   109,     3,   105,   117,     3,
     105,   150,   171,    10,    22,    24,    25,    89,    92,    93,
     111,   116,   128,   176,   177,   178,    10,    18,    19,    66,
     111,   168,   169,   170,   108,   108,   108,   109,   183,    60,
      92,    93,    95,    96,   115,   184,   105,   105,   175,   184,
     171,   184,   184,   184,     3,   184,   104,   171,     4,   171,
     171,   170,   184,   184,   158,   184,   162,   110,   184,   184,
     184,   184,   184,   182,   110,    10,    22,    24,    25,    89,
      91,    92,    93,    94,    95,    96,   111,   128,   157,   172,
     173,   174,   184,   178,   110,   184,   171,   184,   184,   184,
     104,   108,   174
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
        case 8:

/* Line 1806 of yacc.c  */
#line 155 "sml.tab.ypp"
    {
               end_model(NULL);
               rem_indexing(NULL);
            }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 161 "sml.tab.ypp"
    {
                     add_indexing((yyvsp[(3) - (4)].opPtrIx));
                     begin_model((char*)(yyvsp[(2) - (4)].string), (yyvsp[(3) - (4)].opPtrIx));
                  }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 167 "sml.tab.ypp"
    {
                  end_model(NULL);
                  rem_indexing(NULL);
               }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 171 "sml.tab.ypp"
    {
                  end_model((char*)(yyvsp[(3) - (4)].string));
                  rem_indexing(NULL);
               }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 207 "sml.tab.ypp"
    {
               end_smodel(NULL);
               rem_indexing(NULL);
            }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 217 "sml.tab.ypp"
    {
                     add_indexing((yyvsp[(3) - (9)].opPtrIx));
                     begin_smodel((char*)(yyvsp[(2) - (9)].string), (yyvsp[(3) - (9)].opPtrIx), (yyvsp[(7) - (9)].opPtr));
                  }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 227 "sml.tab.ypp"
    {
                  end_smodel(NULL);
                  rem_indexing(NULL);
               }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 231 "sml.tab.ypp"
    {
                  end_smodel((char*)(yyvsp[(3) - (4)].string));
                  rem_indexing(NULL);
               }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 245 "sml.tab.ypp"
    {
               stages_glo = NULL;
            }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 253 "sml.tab.ypp"
    {              
                     if (!is_stoch_model) { 
                        cerr << "Syntax Error: keyword 'stages' can only be "
                        "used in stochastic blocks" << endl;
                        exit(1);
                     }
                     stages_glo = (yyvsp[(2) - (3)].opPtr);
                  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 278 "sml.tab.ypp"
    {
		YAC_LOG("setdef:[SET ID indexing_opt] - ID=["<<(yyvsp[(2) - (3)].string)<<"] indexing_opt=["<<(yyvsp[(3) - (3)].opPtrIx)<<"]");
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 281 "sml.tab.ypp"
    {
            YAC_LOG("stochattr_opt=[" << (yyvsp[(5) - (6)].opPtr)<<"]");
            YAC_LOG("setattributes_opt=[" <<(yyvsp[(6) - (6)].opPtr)<<"]");
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

  case 27:

/* Line 1806 of yacc.c  */
#line 301 "sml.tab.ypp"
    {
               if ((yyvsp[(4) - (4)].opPtrIx)) add_indexing((yyvsp[(4) - (4)].opPtrIx));
            }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 303 "sml.tab.ypp"
    {
               ModelComp *newmc;
               if (is_stoch_model){
                  StochModelComp *newmcs = new StochModelComp((yyvsp[(3) - (7)].string),TPARAM,(yyvsp[(4) - (7)].opPtrIx),(yyvsp[(7) - (7)].opPtr));
                  addStochInfo(newmcs, (yyvsp[(6) - (7)].opPtr));
                  newmc = newmcs;
               }else{
                  newmc = new ModelComp((yyvsp[(3) - (7)].string), TPARAM, (yyvsp[(4) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
               }
               current_model->addComp(newmc);
               if ((yyvsp[(4) - (7)].opPtrIx)) rem_indexing((yyvsp[(4) - (7)].opPtrIx));
               //$$=$2; 
            }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 321 "sml.tab.ypp"
    {
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 323 "sml.tab.ypp"
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

  case 31:

/* Line 1806 of yacc.c  */
#line 341 "sml.tab.ypp"
    {
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 343 "sml.tab.ypp"
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

  case 33:

/* Line 1806 of yacc.c  */
#line 356 "sml.tab.ypp"
    {
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 358 "sml.tab.ypp"
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

  case 35:

/* Line 1806 of yacc.c  */
#line 371 "sml.tab.ypp"
    { 
     		YAC_LOG("subject to rule matched - ID["<<(yyvsp[(2) - (3)].string)<<"]");
            if ((yyvsp[(3) - (3)].opPtrIx)) add_indexing((yyvsp[(3) - (3)].opPtrIx));
         }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 374 "sml.tab.ypp"
    {
         	YAC_LOG("subject to rule part2");
         	ModelComp *newmc;
            if (is_stoch_model){
            	YAC_LOG("stoch model subject to");
               	newmc = new StochModelComp((yyvsp[(2) - (7)].string), TCON, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
               	addStochInfo(newmc, (yyvsp[(5) - (7)].opPtr));
            }
            else{
            	YAC_LOG("non-stoch model subject to - ");
               	newmc = new ModelComp((yyvsp[(2) - (7)].string), TCON, (yyvsp[(3) - (7)].opPtrIx), (yyvsp[(7) - (7)].opPtr));
            }
            current_model->addComp(newmc);
            if ((yyvsp[(3) - (7)].opPtrIx)) rem_indexing((yyvsp[(3) - (7)].opPtrIx));
            (yyval.string)=(yyvsp[(2) - (7)].string);
         }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 392 "sml.tab.ypp"
    {(yyval.optype)=LE;}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 393 "sml.tab.ypp"
    {(yyval.optype)=GE;}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 394 "sml.tab.ypp"
    {(yyval.optype)=LT;}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 395 "sml.tab.ypp"
    {(yyval.optype)=GT;}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 396 "sml.tab.ypp"
    {(yyval.optype)=ASSIGN;}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 397 "sml.tab.ypp"
    {(yyval.optype)=EQ;}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 398 "sml.tab.ypp"
    {(yyval.optype)=NE;}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 401 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 402 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(LT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 403 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(LT, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(LT, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 407 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(LE, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 408 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(LE, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(LE, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 412 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(GT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 413 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(GT, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(GT, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 417 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(GE, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 418 "sml.tab.ypp"
    { 
               OpNode *temp = new OpNode(GE, (yyvsp[(1) - (5)].opPtr), (yyvsp[(3) - (5)].opPtr));
               (yyval.opPtr) = new OpNode(GE, temp, (yyvsp[(5) - (5)].opPtr)); 
            }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 422 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(EQ, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 423 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(NE,(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr)); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 428 "sml.tab.ypp"
    {(yyval.opPtrIx)=NULL;}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 429 "sml.tab.ypp"
    {
               YAC_LOG("indexing_opt : indexing rule matched!");
               (yyval.opPtrIx)=(yyvsp[(1) - (1)].opPtrIx);}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 434 "sml.tab.ypp"
    {
					YAC_LOG("start_indexing : LBRACE setexpr_list rule matched!");
                     SyntaxNodeIx *tmp = 
                        new SyntaxNodeIx(new SyntaxNode(LBRACE, (yyvsp[(2) - (2)].opPtr)));
                     add_indexing(tmp);
                     (yyval.opPtr) = (yyvsp[(2) - (2)].opPtr);
                  }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 443 "sml.tab.ypp"
    {
               rem_indexing(NULL);
               SyntaxNode *tmp = 
                  new SyntaxNode(LBRACE, new SyntaxNode(COLON, (yyvsp[(1) - (4)].opPtr), (yyvsp[(3) - (4)].opPtr)));
               (yyval.opPtrIx) = new SyntaxNodeIx(tmp);
            }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 449 "sml.tab.ypp"
    {
               rem_indexing(NULL);
               SyntaxNodeIx *tmp = new SyntaxNodeIx(new SyntaxNode(LBRACE, (yyvsp[(1) - (2)].opPtr)));
               (yyval.opPtrIx)=tmp;
            }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 461 "sml.tab.ypp"
    {
                  if ((yyvsp[(1) - (3)].opPtr)->getOpCode() == COMMA)
                     (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
                  else
                     (yyval.opPtr) = new ListNode(COMMA, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 470 "sml.tab.ypp"
    {
             	YAC_LOG("ID IN setexpression rule matched!");
                  (yyval.opPtr) = new OpNode(IN, new IDNode((yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 474 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new OpNode(IN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 479 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LBRACKET, (yyvsp[(1) - (2)].opPtr)); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 482 "sml.tab.ypp"
    { 
					YAC_LOG("mdim_dummy_start: LBRACKET ID rule matched!");
                     (yyval.opPtr) = new ListNode(COMMA, new IDNode((yyvsp[(2) - (2)].string)));
                  }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 486 "sml.tab.ypp"
    {
                     (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back(new IDNode((yyvsp[(3) - (3)].string)));
                  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 491 "sml.tab.ypp"
    {
		YAC_LOG("lexpr: expr rule matched! "<<(yyvsp[(1) - (1)].opPtr));
		}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 494 "sml.tab.ypp"
    {
            (yyval.opPtr) = new SyntaxNode(LBRACKET, (yyvsp[(2) - (3)].opPtr)); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 509 "sml.tab.ypp"
    {(yyval.opPtr)=NULL;}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 510 "sml.tab.ypp"
    {(yyval.opPtr)=(yyvsp[(1) - (1)].opPtr);}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 513 "sml.tab.ypp"
    {(yyval.opPtr)=(yyvsp[(1) - (1)].opPtr);}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 514 "sml.tab.ypp"
    {
                  if ((yyvsp[(2) - (2)].opPtr)==NULL){
                     (yyval.opPtr) = (yyvsp[(1) - (2)].opPtr);
                  }else{
                     if ((yyvsp[(1) - (2)].opPtr)==NULL){
                     	assert(false); //feng- I don't think this will happen!
                        (yyval.opPtr) = new ListNode(' ', (yyvsp[(2) - (2)].opPtr));
                     }else{
                        if ((yyvsp[(1) - (2)].opPtr)->getOpCode() == ' ')
                           (yyval.opPtr) = (yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
                        else
                           (yyval.opPtr) = new ListNode(' ', (yyvsp[(1) - (2)].opPtr), (yyvsp[(2) - (2)].opPtr));
                     }
                  }
               }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 531 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new SyntaxNode(DIMEN, new ValueNode(*(yyvsp[(2) - (2)].ival)));
               }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 534 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new SyntaxNode(WITHIN, (yyvsp[(2) - (2)].opPtr));
               }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 537 "sml.tab.ypp"
    {
            		YAC_LOG("match setexpression define rule");
                  (yyval.opPtr) = new OpNode(DEFINED, (yyvsp[(2) - (2)].opPtr));
               }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 541 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));
               }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 544 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(ORDERED); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 548 "sml.tab.ypp"
    {
					YAC_LOG("setexpression: LBRACE setexpression RBRACE -- rule matched");
					(yyval.opPtr)= new ListSet((yyvsp[(2) - (3)].opPtr));
				}
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 552 "sml.tab.ypp"
    {
         		YAC_LOG("creating indexing set");
				(yyval.opPtr) = new IndexingSet((yyvsp[(1) - (1)].opPtrIx));
             }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 557 "sml.tab.ypp"
    {
				YAC_LOG("create ListSet");
				(yyval.opPtr) = new ListSet((yyvsp[(1) - (1)].opPtr));
			}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 577 "sml.tab.ypp"
    {
             		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
             		YAC_LOG("setexpression: identifier -- rule matched");
             }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 582 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new CompositeSet((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 585 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new CompositeSet((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 588 "sml.tab.ypp"
    {
                  cerr << "FIXME: ubsetop indexing setexpression\n";
                  exit(2);
               }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 592 "sml.tab.ypp"
    {
                  (yyval.opPtr) = new SimpleSet((yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 595 "sml.tab.ypp"
    {
                  cerr << "FIXME: SETOF\n";
                  exit(2);
               }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 600 "sml.tab.ypp"
    {
                  (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr);
               }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 608 "sml.tab.ypp"
    { (yyval.opPtr) = NULL; }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 609 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 612 "sml.tab.ypp"
    {
                     if ((yyvsp[(1) - (1)].opPtr)==NULL) {(yyval.opPtr) = NULL;}
                     else{(yyval.opPtr) = new ListNode(COMMA, (yyvsp[(1) - (1)].opPtr));}
                  }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 616 "sml.tab.ypp"
    {
                     if ((yyvsp[(3) - (3)].opPtr)==NULL){
                        (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
                     }else{
                        if ((yyvsp[(1) - (3)].opPtr)==NULL){
                           (yyval.opPtr) = new ListNode(COMMA, (yyvsp[(3) - (3)].opPtr));
                        }else{
                           (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
                        }
                     }
                  }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 629 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 630 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 631 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 632 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode((yyvsp[(1) - (2)].optype), (yyvsp[(2) - (2)].opPtr));}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 633 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(IN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 634 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 635 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 636 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 639 "sml.tab.ypp"
    {(yyval.opPtr) = NULL;}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 640 "sml.tab.ypp"
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

  case 105:

/* Line 1806 of yacc.c  */
#line 649 "sml.tab.ypp"
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

  case 106:

/* Line 1806 of yacc.c  */
#line 660 "sml.tab.ypp"
    {(yyval.opPtr)=NULL;}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 661 "sml.tab.ypp"
    {(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);}
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 665 "sml.tab.ypp"
    {
                  (yyval.opPtr) = addItemToListOrCreate(COMMA, NULL, (yyvsp[(1) - (1)].opPtr));}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 667 "sml.tab.ypp"
    {
                  (yyval.opPtr) = addItemToListOrCreate(COMMA, (ListNode*)(yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 673 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 674 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 675 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 676 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(LE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 677 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(GE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 678 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 679 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(ASSIGN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 680 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 684 "sml.tab.ypp"
    {(yyval.opPtr) = new OpNode(IN, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 685 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SUFFIX, (yyvsp[(3) - (3)].opPtr));}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 690 "sml.tab.ypp"
    { (yyval.optype) = DIFF; }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 691 "sml.tab.ypp"
    { (yyval.optype) = SYMDIFF; }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 692 "sml.tab.ypp"
    { (yyval.optype) = CROSS; }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 695 "sml.tab.ypp"
    { (yyval.optype) = UNION; }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 696 "sml.tab.ypp"
    { (yyval.optype) = INTER; }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 705 "sml.tab.ypp"
    {
               /* this is a simple identifier in global context */
               YAC_LOG("identifier: iditem rule matched  "<<(yyvsp[(1) - (1)].opPtr));
               (yyval.opPtr) = SyntaxNode::find_var_ref_in_context(current_model, (yyvsp[(1) - (1)].opPtr));
            }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 710 "sml.tab.ypp"
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

               YAC_LOG("Merge identifier:"<< *((yyvsp[(1) - (3)].opPtr)) <<"-----> iditem:"<<*((yyvsp[(3) - (3)].opPtr)));
               /* merge argument lists */
               (yyval.opPtr)->merge((yyvsp[(1) - (3)].opPtr));
            }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 760 "sml.tab.ypp"
    {                               /* simple identifier */
			YAC_LOG("iditem : ID rule matched , "<<(yyvsp[(1) - (1)].string));
            (yyval.opPtr)=new IDNode((yyvsp[(1) - (1)].string));
         }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 764 "sml.tab.ypp"
    { /* subscripted id'fier */
            YAC_LOG("ID "<<(yyvsp[(1) - (4)].string)<<" ["<<(yyvsp[(3) - (4)].opPtr)<<"]");
            (yyval.opPtr) = new SyntaxNode(LSBRACKET, new IDNode((yyvsp[(1) - (4)].string)), (yyvsp[(3) - (4)].opPtr));
            //printf("%s\n", print_SyntaxNodesymb($$));
         }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 769 "sml.tab.ypp"
    {
            // This is of the type xh(-1,i) which is xh[i] at a previous stage
            // the ancestor information is conveyed by an ID SyntaxNode with 
            // two arguments, where the second argument is the ancestor
            // $3 is (long int*) => change its sign
            *(yyvsp[(3) - (6)].ival) = -(*(yyvsp[(3) - (6)].ival));
            SyntaxNode *nd = new IDNode((yyvsp[(1) - (6)].string), *(yyvsp[(3) - (6)].ival));
            (yyval.opPtr) = new SyntaxNode(LSBRACKET, nd, (yyvsp[(5) - (6)].opPtr));
         }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 778 "sml.tab.ypp"
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

  case 131:

/* Line 1806 of yacc.c  */
#line 803 "sml.tab.ypp"
    {
				YAC_LOG("expr_list: expr rule matched! "<<(yyvsp[(1) - (1)].opPtr));
               (yyval.opPtr) = new ListNode(COMMA, (yyvsp[(1) - (1)].opPtr));
            }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 807 "sml.tab.ypp"
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

  case 133:

/* Line 1806 of yacc.c  */
#line 820 "sml.tab.ypp"
    { YAC_LOG("expr: value rule matched!" <<(yyvsp[(1) - (1)].opPtr)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 821 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LBRACKET, (yyvsp[(2) - (3)].opPtr)); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 822 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('+', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 823 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('-', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 824 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('-', (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 825 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('*', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 826 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode('/', (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 827 "sml.tab.ypp"
    { (yyval.opPtr) = new OpNode(POWER, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 828 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(ELLIPSE, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 829 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LOGICAL_OR, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 830 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LOGICAL_AND, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
    							YAC_LOG("expr: expr LOGICAL_AND expr -- rule matched ["<<(yyvsp[(1) - (3)].opPtr)<<"]  ["<<(yyvsp[(3) - (3)].opPtr)<<"]"); 
    }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 833 "sml.tab.ypp"
    {
    	(yyval.opPtr) = new SyntaxNode(NE,(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
    	YAC_LOG("expr: expr NE expr -- rule matched ["<<(yyvsp[(1) - (3)].opPtr)<<"]  ["<<(yyvsp[(3) - (3)].opPtr)<<"]");
    }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 837 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode('!', (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 838 "sml.tab.ypp"
    {add_indexing((yyvsp[(2) - (2)].opPtrIx));}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 838 "sml.tab.ypp"
    {   /*feng - need a bracket to handle current indexing? eg. sum{}(expr) - sum{}(expr) vs sum{}(expr - sum{}(expr))*/
         /* reduction operator: do we need to keep track of the ID of the
            dummy variable(s)? */
         (yyval.opPtr) = new SyntaxNode((int)(yyvsp[(1) - (6)].optype), (yyvsp[(2) - (6)].opPtrIx), (yyvsp[(5) - (6)].opPtr));
         rem_indexing((yyvsp[(2) - (6)].opPtrIx));
      }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 844 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (4)].opPtr), (yyvsp[(4) - (4)].opPtr)); }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 845 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (6)].opPtr), (yyvsp[(4) - (6)].opPtr), (yyvsp[(6) - (6)].opPtr)); }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 846 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(FIRST, (yyvsp[(3) - (4)].opPtr)); }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 847 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(LAST, (yyvsp[(3) - (4)].opPtr)); }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 848 "sml.tab.ypp"
    { 
         //$$ = new SyntaxNode(EXPECTATION, new SyntaxNode(LBRACKET, $3));}
         (yyval.opPtr) = new SyntaxNode(EXPECTATION, (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 852 "sml.tab.ypp"
    { /* function definition */
         (yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (4)].optype), new SyntaxNode(LBRACKET, (yyvsp[(3) - (4)].opPtr)));
      }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 857 "sml.tab.ypp"
    { (yyval.optype)=ORD; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 858 "sml.tab.ypp"
    { (yyval.optype)=CARD; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 861 "sml.tab.ypp"
    { (yyval.optype)=SUM; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 862 "sml.tab.ypp"
    { (yyval.optype)=MAX; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 863 "sml.tab.ypp"
    { (yyval.optype)=MIN; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 864 "sml.tab.ypp"
    { (yyval.optype)=PROD; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 867 "sml.tab.ypp"
    {
            (yyval.opPtr)=new ValueNode(*(yyvsp[(1) - (1)].ival));
         }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 870 "sml.tab.ypp"
    { 
            (yyval.opPtr)=new ValueNode(*(yyvsp[(1) - (1)].fval));
         }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 873 "sml.tab.ypp"
    { 
     		YAC_LOG("value: identifier rule matched! "<<(yyvsp[(1) - (1)].opPtr));
            (yyval.opPtr)=new SyntaxNode(0, (yyvsp[(1) - (1)].opPtr));
         }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 877 "sml.tab.ypp"
    { 
            (yyval.opPtr) = new SyntaxNode(INFINITY);
         }
    break;



/* Line 1806 of yacc.c  */
#line 3471 "../src/parser/sml.tab.cpp"
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
#line 882 "sml.tab.ypp"


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
void begin_model(char *name, SyntaxNode *indexing) {
  AmplModel *new_mod = new AmplModel(name);
  ModelComp *newmc;
  
  LOG("Start Model: " << name << " "<<indexing);

  /* FIXME: include attrib in definition */
  newmc = new ModelComp(name, TMODEL, indexing, NULL);
  newmc->other = new_mod;

  new_mod->node = newmc;            /* add pointer-to-node to the model */
  current_model->addComp(newmc);
 
  //new_mod->setGlobalName();    
  /* and change current model */
  current_model = new_mod;
}

/* ----------------------------------------------------------------------------
begin_smodel
---------------------------------------------------------------------------- */
void begin_smodel(char *name, SyntaxNode *indexing, SyntaxNode *stochsets) {
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
 
   //new_mod->setGlobalName();    
   /* and change current model */
   current_model = new_mod;
   is_stoch_model = true;
}

/* ----------------------------------------------------------------------------
end_model
---------------------------------------------------------------------------- */
void end_model(char *name) {
  // Check end block name matches block name
  if (name && string(name) != current_model->name) {
    cerr << "end block '" << name << "' encountered in block '" << 
      current_model->name << "'" << endl;
    exit(1);
  }

  current_model = current_model->parent;
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
   YAC_LOG("[add_index] - indexing:["<< *indexing<<"] "<<SyntaxNode::print_SyntaxNodesymb(indexing));
   list_of_indexing[n_indexing] = indexing;
   YAC_LOG("[add_index] - length of indexing now: " << n_indexing+1);
   n_indexing++;
}

void rem_indexing(SyntaxNodeIx *indexing){
	YAC_LOG("[rem_indexing] length[" << n_indexing<<"]" );
	if (indexing){
    	assert(indexing==list_of_indexing[n_indexing-1]);
      	YAC_LOG("rem indexing expression to list: " << *indexing );
   	}
	n_indexing--;
   	YAC_LOG("[rem_indexing] length[" << n_indexing<<"]" );
}

/* ---------------------------------------------------------------------------
Stochastic model helper functions
---------------------------------------------------------------------------- */
void addStochInfo(ModelComp *newmcs, SyntaxNode *stochopt) {
   if(stochopt) {
     bool isDet = stochopt->getOpCode() == DETERMINISTIC;
     YAC_LOG("isDet["<<isDet<<"] is_deterministic_glo["<<is_deterministic_glo<<"]");
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
	errcode = chdir("tmp");
	assert(errcode==0);
   	AmplModel::root = new AmplModel("root");
   	current_model = AmplModel::root;
   	is_stoch_model = false;
   	is_deterministic_glo = false;
   	stages_glo = NULL;

	YAC_LOG("before model parse is done!");
   	errcode = yyparse();
   	assert(errcode==0);
   	YAC_LOG("model parse is done!");
   	
   	fclose(yyin);
   	errcode = chdir("..");
   	assert(errcode==0);
	return errcode;
}

