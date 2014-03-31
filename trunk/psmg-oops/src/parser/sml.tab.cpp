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
    #include "../model/ObjComp.h"
    #include "../model/ModelComp.h" 
    #include "../model/AmplModel.h"
    #include "../model/SyntaxNode.h"
    #include "../model/SyntaxNodeOP.h"
    #include "../model/SyntaxNodeID.h"
    #include "../model/SyntaxNodeValue.h"
    #include "../model/SyntaxNodeIDREF.h"
    #include "../model/SyntaxNodeIDREFM.h"
//    #include "../st_model/StochModel.h"
//    #include "../st_model/StochModelComp.h"
   
    #include "../sml/GlobalVariables.h"
    #include "../util/global_util_functions.h"
    
    using namespace std;

    void print_index_dummy_vars();
    void record_dummy_var();
    void restore_dummy_var();
 
    void begin_model(const string& name, SyntaxNode *indexing);
    void end_model(const string& name);
    void begin_smodel(const string& name, SyntaxNode *indexing, SyntaxNode *stochsets);
    void end_smodel(const string& name);

    extern int yylineno;
    int yylex(void);
    void yyerror(const char *s);

    static AmplModel *current_model;    /* this is the model currently active */
                                       /* this is the GLOBAL context */

	vector<string> index_dummy_vars;    
	vector<int> dummy_var_size;                                            
	bool isDummyVar(string&);
	                                                
    /* ---------------- stochastic global variables:------------------------ */
    static bool is_stoch_model;      /* true if inside stochastic model def */
    /* these are set by global stocastic modifier commands */
    static bool is_deterministic_glo;
    static SyntaxNode *stages_glo;
    extern FILE *yyin;

    void addStochInfo(ModelComp *newmc, SyntaxNode*);


/* Line 268 of yacc.c  */
#line 125 "../src/parser/sml.tab.cpp"

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
     ASSIGN = 386,
     MINUS = 387,
     PLUS = 388,
     DIVID = 389,
     TIMES = 390
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
#define ASSIGN 386
#define MINUS 387
#define PLUS 388
#define DIVID 389
#define TIMES 390




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 73 "sml.tab.ypp"

  int optype;
  objType otype;
  long *ival;
  double *fval;
  char* string;
  AmplModel* model;
  SyntaxNode *opPtr;



/* Line 293 of yacc.c  */
#line 443 "../src/parser/sml.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 455 "../src/parser/sml.tab.cpp"

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
#define YYLAST   492

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  141
/* YYNRULES -- Number of states.  */
#define YYNSTATES  252

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   390

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   136,     2,     2,     2,     2,     2,     2,
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
     135
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    14,    15,
      25,    26,    27,    42,    43,    44,    53,    54,    56,    57,
      61,    63,    65,    67,    69,    71,    73,    74,    76,    81,
      82,    85,    87,    91,    93,    97,    99,   103,   105,   109,
     111,   115,   116,   124,   125,   128,   131,   132,   136,   139,
     141,   142,   150,   151,   154,   156,   158,   160,   163,   166,
     169,   171,   173,   175,   177,   179,   181,   182,   190,   191,
     193,   195,   199,   201,   203,   206,   209,   212,   216,   217,
     226,   233,   235,   237,   239,   241,   242,   247,   251,   255,
     257,   259,   261,   263,   265,   269,   273,   277,   280,   284,
     286,   288,   290,   292,   294,   296,   298,   300,   302,   304,
     311,   316,   317,   322,   324,   328,   330,   332,   336,   339,
     343,   347,   351,   355,   359,   363,   368,   375,   380,   385,
     386,   393,   395,   397,   399,   401,   403,   405,   407,   409,
     411,   413
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     138,     0,    -1,    -1,   138,   151,    -1,   143,    -1,   140,
      -1,   146,    -1,    -1,    -1,   115,     3,   141,   152,   110,
     142,   104,   138,   105,    -1,    -1,    -1,   115,     3,   144,
     152,   119,   116,   111,   155,   112,   110,   145,   104,   138,
     105,    -1,    -1,    -1,   120,   147,   153,   110,   148,   104,
     138,   105,    -1,    -1,   117,    -1,    -1,   120,   150,   153,
      -1,   159,    -1,   164,    -1,   169,    -1,   174,    -1,   176,
      -1,   139,    -1,    -1,   153,    -1,   104,   155,   154,   105,
      -1,    -1,   110,   182,    -1,   156,    -1,   155,   106,   156,
      -1,   186,    -1,   157,    89,   186,    -1,     4,    -1,   189,
      15,   189,    -1,     3,    -1,   111,   158,   112,    -1,     3,
      -1,   158,   106,     3,    -1,    -1,    82,     3,   160,   152,
     149,   161,   107,    -1,    -1,   161,   162,    -1,    18,     4,
      -1,    -1,   131,   163,   178,    -1,    66,   186,    -1,    19,
      -1,    -1,    81,     3,   165,   152,   149,   166,   107,    -1,
      -1,   166,   167,    -1,    25,    -1,    24,    -1,    22,    -1,
     168,   189,    -1,    10,   189,    -1,   113,   189,    -1,    92,
      -1,    93,    -1,    95,    -1,    96,    -1,    94,    -1,    91,
      -1,    -1,    80,     3,   170,   152,   149,   171,   107,    -1,
      -1,   172,    -1,   173,    -1,   172,   106,   173,    -1,    25,
      -1,    24,    -1,    92,   189,    -1,    93,   189,    -1,    10,
     189,    -1,   114,     3,   189,    -1,    -1,    86,     3,   175,
     152,   149,   110,   189,   107,    -1,   177,     3,   149,   110,
     189,   107,    -1,    83,    -1,    84,    -1,   153,    -1,   186,
      -1,    -1,    51,   179,   153,   186,    -1,   178,   180,   178,
      -1,   178,   181,   178,    -1,   100,    -1,   103,    -1,   101,
      -1,    99,    -1,   102,    -1,   186,    89,   186,    -1,   111,
     182,   112,    -1,   182,   184,   182,    -1,   185,   182,    -1,
     189,   183,   189,    -1,    91,    -1,    92,    -1,    93,    -1,
      95,    -1,    96,    -1,    94,    -1,    61,    -1,    62,    -1,
      65,    -1,     3,    -1,   121,   111,     4,   112,   128,   186,
      -1,   186,   108,   188,   109,    -1,    -1,   186,   128,   187,
     186,    -1,   189,    -1,   188,   106,   189,    -1,   193,    -1,
     186,    -1,   111,   189,   112,    -1,   132,   189,    -1,   189,
     133,   189,    -1,   189,   132,   189,    -1,   189,   135,   189,
      -1,   189,   134,   189,    -1,   189,    90,   189,    -1,   189,
     131,   189,    -1,    58,   182,    59,   189,    -1,    58,   182,
      59,   189,    60,   189,    -1,   118,   111,   188,   112,    -1,
     191,   111,   189,   112,    -1,    -1,   192,   190,   153,   111,
     189,   112,    -1,    78,    -1,    70,    -1,    97,    -1,    98,
      -1,    87,    -1,    57,    -1,    56,    -1,    88,    -1,     4,
      -1,     5,    -1,     6,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   131,   131,   132,   135,   136,   137,   144,   147,   143,
     175,   179,   174,   198,   202,   197,   210,   211,   217,   216,
     231,   232,   233,   234,   235,   236,   246,   247,   250,   260,
     261,   268,   269,   277,   281,   286,   290,   295,   301,   307,
     313,   328,   327,   351,   352,   368,   373,   372,   380,   385,
     395,   394,   416,   417,   433,   434,   435,   436,   437,   438,
     441,   442,   443,   444,   445,   446,   453,   452,   472,   473,
     477,   481,   489,   490,   491,   492,   493,   494,   501,   500,
     525,   542,   543,   549,   554,   561,   560,   568,   571,   577,
     578,   579,   582,   583,   590,   595,   598,   602,   606,   612,
     613,   614,   615,   616,   617,   620,   621,   623,   634,   654,
     659,   665,   664,   679,   684,   693,   698,   702,   703,   704,
     705,   706,   707,   708,   709,   710,   711,   712,   716,   721,
     720,   730,   731,   732,   733,   736,   737,   738,   739,   742,
     745,   748
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
  "INDEX", "DVAR", "DOT", "STAGE", "NODE", "ASSIGN", "MINUS", "PLUS",
  "DIVID", "TIMES", "'!'", "$accept", "statements", "block", "blockblock",
  "$@1", "$@2", "stochblock", "$@3", "$@4", "stageblock", "$@5", "$@6",
  "stochattr_opt", "$@7", "statement", "indexing_opt", "indexing_set",
  "indexing_condition_opt", "indexing_setexpr_list",
  "indexing_setexpr_item", "indexing_dummy_list", "indexing_dummy",
  "setdef", "$@8", "setattributes_opt", "setattribute", "$@9", "paramdef",
  "$@10", "paramattributes_opt", "paramattribute", "relop", "vardef",
  "$@11", "varattributes_opt", "varattributes", "varattribute", "consdef",
  "$@12", "objdef", "objdef_type", "setexpression", "$@13", "bsetop",
  "ubsetop", "lexpr", "b_compare_op", "blogic_op", "ulogic_op",
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
     385,   386,   387,   388,   389,   390,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   137,   138,   138,   139,   139,   139,   141,   142,   140,
     144,   145,   143,   147,   148,   146,   149,   149,   150,   149,
     151,   151,   151,   151,   151,   151,   152,   152,   153,   154,
     154,   155,   155,   156,   156,   156,   156,   157,   157,   158,
     158,   160,   159,   161,   161,   162,   163,   162,   162,   162,
     165,   164,   166,   166,   167,   167,   167,   167,   167,   167,
     168,   168,   168,   168,   168,   168,   170,   169,   171,   171,
     172,   172,   173,   173,   173,   173,   173,   173,   175,   174,
     176,   177,   177,   178,   178,   179,   178,   178,   178,   180,
     180,   180,   181,   181,   182,   182,   182,   182,   182,   183,
     183,   183,   183,   183,   183,   184,   184,   185,   186,   186,
     186,   187,   186,   188,   188,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   190,
     189,   191,   191,   191,   191,   192,   192,   192,   192,   193,
     193,   193
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     0,     0,     9,
       0,     0,    14,     0,     0,     8,     0,     1,     0,     3,
       1,     1,     1,     1,     1,     1,     0,     1,     4,     0,
       2,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     0,     7,     0,     2,     2,     0,     3,     2,     1,
       0,     7,     0,     2,     1,     1,     1,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     0,     7,     0,     1,
       1,     3,     1,     1,     2,     2,     2,     3,     0,     8,
       6,     1,     1,     1,     1,     0,     4,     3,     3,     1,
       1,     1,     1,     1,     3,     3,     3,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       4,     0,     4,     1,     3,     1,     1,     3,     2,     3,
       3,     3,     3,     3,     3,     4,     6,     4,     4,     0,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,    81,    82,     0,     0,
      13,    25,     5,     4,     6,     3,    20,    21,    22,    23,
      24,     0,    66,    50,    41,    78,     7,     0,    16,    26,
      26,    26,    26,    26,    26,     0,     0,    17,    18,     0,
      16,    27,    16,    16,    16,     0,     0,   108,   139,   140,
     141,   137,   136,     0,   132,   131,   135,   138,   133,   134,
       0,     0,     0,     0,    29,    31,     0,   116,     0,     0,
     129,   115,    14,     0,     0,    68,    52,    43,     0,     8,
       0,   108,   139,   107,     0,     0,     0,   116,     0,   108,
       0,     0,   116,     0,     0,     0,   118,     0,     0,     0,
       0,     0,   111,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    19,     0,     0,    73,    72,     0,     0,
       0,     0,    69,    70,     0,     0,     0,     0,     0,     0,
       0,     0,   105,   106,     0,    97,     0,    99,   100,   101,
     104,   102,   103,     0,     0,    38,   117,     0,   113,     0,
      32,    30,    28,    34,     0,     0,    36,   123,   124,   120,
     119,   122,   121,     0,     0,     2,    80,    76,    74,    75,
       0,    67,     0,     0,    56,    55,    54,    65,    60,    61,
      64,    62,    63,    51,     0,    53,     0,     0,    49,     0,
      42,    46,    44,     0,     2,     0,    95,   125,    96,    94,
      98,    40,     0,   127,     0,   110,   112,   128,     0,     0,
      77,    71,    58,    59,    57,    45,    48,     0,    79,     0,
       0,     0,   114,     0,     0,    15,    85,    83,    47,    84,
       9,     0,   126,   109,   130,     0,    92,    89,    91,    93,
      90,     0,     0,    11,     0,    87,    88,     0,    86,     2,
       0,    12
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    11,    12,    33,   127,    13,    34,   247,    14,
      27,   112,    39,    73,    15,    40,    41,    99,    64,    65,
      66,    91,    16,    31,   125,   192,   217,    17,    30,   124,
     185,   186,    18,    29,   121,   122,   123,    19,    32,    20,
      21,   228,   235,   241,   242,    85,   143,   134,    86,    92,
     155,   147,    88,   111,    69,    70,    71
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -160
static const yytype_int16 yypact[] =
{
    -160,     5,  -160,    -2,    26,    47,  -160,  -160,    52,    59,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,    67,  -160,  -160,  -160,  -160,  -100,   -12,   -60,   -12,
     -12,   -12,   -12,   -12,   -12,   166,   -33,  -160,  -160,   -14,
     -60,  -160,   -60,   -60,   -60,    -9,   -85,    18,   -67,  -160,
    -160,  -160,  -160,   162,  -160,  -160,  -160,  -160,  -160,  -160,
     252,    -6,     0,   298,   -90,  -160,    37,   254,    10,    20,
    -160,  -160,  -160,   -12,   298,    16,  -160,  -160,    23,  -160,
     -18,  -160,  -160,  -160,   162,   -29,   162,   -77,   331,   -89,
     298,   -88,  -106,    42,   298,   130,   -76,   166,   162,    32,
       1,   298,  -160,   298,   298,   298,   298,   298,   298,   298,
     298,   -12,    34,  -160,   135,   298,  -160,  -160,   298,   298,
     133,    46,    33,  -160,   221,   -10,   298,    51,    53,   -51,
     308,   298,  -160,  -160,   162,   -13,     1,  -160,  -160,  -160,
    -160,  -160,  -160,   298,   177,  -160,  -160,   -70,   192,    69,
    -160,   -13,  -160,  -106,   -42,     1,   192,  -160,   210,   -76,
     -76,    93,    93,    66,    73,  -160,  -160,   192,   192,   192,
     298,  -160,    16,   298,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,   298,  -160,   298,   182,  -160,     1,
    -160,  -160,  -160,   158,  -160,   166,  -160,   -53,   -13,  -106,
     192,  -160,   298,  -160,    60,  -160,  -106,  -160,   298,   352,
     192,  -160,   192,   192,   192,  -160,  -106,    25,  -160,   364,
     -59,   298,   192,     1,   246,  -160,  -160,  -160,    48,  -106,
    -160,    77,   192,  -106,  -160,   -12,  -160,  -160,  -160,  -160,
    -160,    25,    25,  -160,     1,    48,    48,    85,  -106,  -160,
     372,  -160
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -160,  -159,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,    31,  -160,  -160,   128,   -27,  -160,    -3,    97,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,    24,  -160,  -160,  -160,
    -160,  -147,  -160,  -160,  -160,   -71,  -160,  -160,  -160,   -32,
    -160,   101,     9,  -160,  -160,  -160,  -160
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -40
static const yytype_int16 yytable[] =
{
      36,    22,   101,    67,    81,     2,   209,   221,   187,   188,
     132,   133,   136,   129,   104,   135,    97,   -39,   144,   -10,
      98,    87,   102,   -39,   145,   103,   115,   151,    81,    23,
     131,   101,   132,   133,    80,   219,   202,   104,   -35,   -35,
     116,   117,   203,   -35,    68,   -35,   113,    97,   132,   133,
      24,   102,    87,   231,    87,    25,   189,    37,   108,   109,
      38,   196,    26,   198,   202,    67,    87,   205,   153,    93,
      28,    75,    96,    76,    77,    78,   226,    72,   105,   106,
     107,   108,   109,   114,   164,     3,     4,     5,     6,     7,
     250,     8,    35,   130,   245,   246,    74,   190,   128,    93,
     104,    79,    87,   148,   199,    94,    68,   -37,   118,   119,
     148,    95,   156,   157,   158,   159,   160,   161,   162,   163,
       9,   191,    62,   206,   167,    10,   100,   168,   169,    35,
     120,   110,   104,   126,   149,   193,   170,   152,   165,   172,
     197,   105,   106,   107,   108,   109,    62,   236,   237,   238,
     239,   240,   200,   171,   146,   194,   104,   216,    42,    43,
      44,    45,    46,    67,   195,    81,    82,    49,    50,    47,
      48,    49,    50,   105,   106,   107,   108,   109,   207,   210,
     201,   204,   212,   104,   208,   229,   215,   243,   223,   249,
     227,   233,   220,   213,   150,   214,   211,   105,   106,   107,
     108,   109,   154,     0,    68,     0,     0,     0,   244,   229,
     229,   222,   248,     0,   227,   227,     0,   224,    51,    52,
      53,     0,    51,    52,    53,   104,     0,    83,     0,     0,
     232,   173,    54,     0,     0,     0,    54,     0,     0,     0,
      55,     0,   166,   174,    55,   175,   176,     0,   104,    56,
      57,     0,     0,    56,    57,    89,    82,    49,    50,    58,
      59,     0,     0,    58,    59,   218,   105,   106,   107,   108,
     109,     0,     0,    84,     0,     0,     0,    60,     0,     0,
      61,     0,   104,    62,    61,     0,     0,    62,     0,   105,
     106,   107,   108,   109,    63,     0,     0,     0,    63,     0,
     104,    81,    82,    49,    50,     0,     0,     0,    51,    52,
      53,     0,   177,   178,   179,   180,   181,   182,     0,     0,
       0,     0,    54,   105,   106,   107,   108,   109,   183,     0,
      55,     0,     0,     0,   184,     0,   104,     0,     0,    56,
      57,   -40,   106,   107,   108,   109,     0,     0,     0,    58,
      59,     0,     0,     0,    51,    52,    53,     0,   234,   -33,
     -33,     0,   101,    90,   -33,     0,   -33,     0,    54,     0,
      61,     0,     0,    62,     0,     0,    55,   105,   106,   107,
     108,   109,   102,     0,    63,    56,    57,     0,     0,     0,
       0,     0,     0,     0,     0,    58,    59,     0,   104,   137,
     138,   139,   140,   141,   142,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,    61,     0,     0,    62,
     146,   104,   137,   138,   139,   140,   141,   142,     0,     0,
      63,     0,     3,     4,     5,     6,     7,     0,     8,   105,
     106,   107,   108,   109,     3,     4,     5,     6,     7,     0,
       8,     0,     3,     4,     5,     6,     7,   225,     8,     0,
       0,     0,   105,   106,   107,   108,   109,     9,     0,   230,
       0,     0,    10,     0,     0,     0,     0,   251,     0,     9,
       0,     0,     0,     0,    10,     0,     0,     9,     0,     0,
       0,     0,    10
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-160))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-40))

static const yytype_int16 yycheck[] =
{
      27,     3,   108,    35,     3,     0,   165,    60,    18,    19,
      61,    62,    89,    84,    90,    86,   106,   106,   106,   119,
     110,    53,   128,   112,   112,    15,    10,    98,     3,     3,
      59,   108,    61,    62,   119,   194,   106,    90,   105,   106,
      24,    25,   112,   110,    35,   112,    73,   106,    61,    62,
       3,   128,    84,   112,    86,     3,    66,   117,   134,   135,
     120,   112,     3,   134,   106,    97,    98,   109,   100,    60,
       3,    40,    63,    42,    43,    44,    51,   110,   131,   132,
     133,   134,   135,    74,   111,    80,    81,    82,    83,    84,
     249,    86,   104,    84,   241,   242,   110,   107,   116,    90,
      90,   110,   134,    94,   136,   111,    97,    89,    92,    93,
     101,   111,   103,   104,   105,   106,   107,   108,   109,   110,
     115,   131,   121,   155,   115,   120,    89,   118,   119,   104,
     114,   111,    90,   110,     4,   126,     3,   105,   104,   106,
     131,   131,   132,   133,   134,   135,   121,    99,   100,   101,
     102,   103,   143,   107,   112,   104,    90,   189,    30,    31,
      32,    33,    34,   195,   111,     3,     4,     5,     6,     3,
       4,     5,     6,   131,   132,   133,   134,   135,   112,   170,
       3,   112,   173,    90,   111,   217,     4,   110,   128,   104,
     217,   223,   195,   184,    97,   186,   172,   131,   132,   133,
     134,   135,   101,    -1,   195,    -1,    -1,    -1,   235,   241,
     242,   202,   244,    -1,   241,   242,    -1,   208,    56,    57,
      58,    -1,    56,    57,    58,    90,    -1,    65,    -1,    -1,
     221,    10,    70,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      78,    -1,   107,    22,    78,    24,    25,    -1,    90,    87,
      88,    -1,    -1,    87,    88,     3,     4,     5,     6,    97,
      98,    -1,    -1,    97,    98,   107,   131,   132,   133,   134,
     135,    -1,    -1,   111,    -1,    -1,    -1,   111,    -1,    -1,
     118,    -1,    90,   121,   118,    -1,    -1,   121,    -1,   131,
     132,   133,   134,   135,   132,    -1,    -1,    -1,   132,    -1,
      90,     3,     4,     5,     6,    -1,    -1,    -1,    56,    57,
      58,    -1,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,    70,   131,   132,   133,   134,   135,   107,    -1,
      78,    -1,    -1,    -1,   113,    -1,    90,    -1,    -1,    87,
      88,   131,   132,   133,   134,   135,    -1,    -1,    -1,    97,
      98,    -1,    -1,    -1,    56,    57,    58,    -1,   112,   105,
     106,    -1,   108,   111,   110,    -1,   112,    -1,    70,    -1,
     118,    -1,    -1,   121,    -1,    -1,    78,   131,   132,   133,
     134,   135,   128,    -1,   132,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    -1,    90,    91,
      92,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,   121,
     112,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
     132,    -1,    80,    81,    82,    83,    84,    -1,    86,   131,
     132,   133,   134,   135,    80,    81,    82,    83,    84,    -1,
      86,    -1,    80,    81,    82,    83,    84,   105,    86,    -1,
      -1,    -1,   131,   132,   133,   134,   135,   115,    -1,   105,
      -1,    -1,   120,    -1,    -1,    -1,    -1,   105,    -1,   115,
      -1,    -1,    -1,    -1,   120,    -1,    -1,   115,    -1,    -1,
      -1,    -1,   120
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   138,     0,    80,    81,    82,    83,    84,    86,   115,
     120,   139,   140,   143,   146,   151,   159,   164,   169,   174,
     176,   177,     3,     3,     3,     3,     3,   147,     3,   170,
     165,   160,   175,   141,   144,   104,   153,   117,   120,   149,
     152,   153,   152,   152,   152,   152,   152,     3,     4,     5,
       6,    56,    57,    58,    70,    78,    87,    88,    97,    98,
     111,   118,   121,   132,   155,   156,   157,   186,   189,   191,
     192,   193,   110,   150,   110,   149,   149,   149,   149,   110,
     119,     3,     4,    65,   111,   182,   185,   186,   189,     3,
     111,   158,   186,   189,   111,   111,   189,   106,   110,   154,
      89,   108,   128,    15,    90,   131,   132,   133,   134,   135,
     111,   190,   148,   153,   189,    10,    24,    25,    92,    93,
     114,   171,   172,   173,   166,   161,   110,   142,   116,   182,
     189,    59,    61,    62,   184,   182,    89,    91,    92,    93,
      94,    95,    96,   183,   106,   112,   112,   188,   189,     4,
     156,   182,   105,   186,   188,   187,   189,   189,   189,   189,
     189,   189,   189,   189,   153,   104,   107,   189,   189,   189,
       3,   107,   106,    10,    22,    24,    25,    91,    92,    93,
      94,    95,    96,   107,   113,   167,   168,    18,    19,    66,
     107,   131,   162,   189,   104,   111,   112,   189,   182,   186,
     189,     3,   106,   112,   112,   109,   186,   112,   111,   138,
     189,   173,   189,   189,   189,     4,   186,   163,   107,   138,
     155,    60,   189,   128,   189,   105,    51,   153,   178,   186,
     105,   112,   189,   186,   112,   179,    99,   100,   101,   102,
     103,   180,   181,   110,   153,   178,   178,   145,   186,   104,
     138,   105
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
#line 144 "sml.tab.ypp"
    {  
				YAC_MODEL_LOG("blockblock: -- started -- "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 147 "sml.tab.ypp"
    {   
				begin_model((yyvsp[(2) - (5)].string),(yyvsp[(4) - (5)].opPtr));
			}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 149 "sml.tab.ypp"
    {
               	end_model((yyvsp[(2) - (9)].string));
               	restore_dummy_var();
            }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 175 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("stochblock: -- started - "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 179 "sml.tab.ypp"
    {
				is_stoch_model = true;
				assert(false); //$8 is already included in the the indexing
				begin_smodel((char*)(yyvsp[(2) - (10)].string), (yyvsp[(4) - (10)].opPtr), (yyvsp[(8) - (10)].opPtr));
    	    }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 184 "sml.tab.ypp"
    {
               end_smodel((char*)(yyvsp[(2) - (14)].string));
               
               is_stoch_model = false;
	           restore_dummy_var();
               YAC_MODEL_LOG("vardef: -- end -- "<<(yyvsp[(2) - (14)].string));
            }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 198 "sml.tab.ypp"
    {
				assert(is_stoch_model == true);
				record_dummy_var();		
			}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 202 "sml.tab.ypp"
    {    
				stages_glo = (yyvsp[(3) - (4)].opPtr);
	        }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 204 "sml.tab.ypp"
    {
                stages_glo = NULL;
				restore_dummy_var();            	
            }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 210 "sml.tab.ypp"
    {(yyval.opPtr) = NULL;}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 212 "sml.tab.ypp"
    {
                	assert(is_stoch_model==true);
                  	(yyval.opPtr) = new SyntaxNode(DETERMINISTIC);
             	}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 217 "sml.tab.ypp"
    {
                	assert(is_stoch_model==true);
             		record_dummy_var();
             	}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 221 "sml.tab.ypp"
    {
                	restore_dummy_var(); 
                  	(yyval.opPtr) = (yyvsp[(3) - (3)].opPtr);
               	}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 246 "sml.tab.ypp"
    {		(yyval.opPtr)=NULL;	}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 247 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 251 "sml.tab.ypp"
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
#line 260 "sml.tab.ypp"
    { (yyval.opPtr)=NULL; }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 262 "sml.tab.ypp"
    {
				  			(yyval.opPtr)=(yyvsp[(2) - (2)].opPtr);
				  		}
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 268 "sml.tab.ypp"
    {  (yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 270 "sml.tab.ypp"
    {
                  assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
                  (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
                  (yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
			}
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 278 "sml.tab.ypp"
    {  
							(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr); 
						}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 282 "sml.tab.ypp"
    {
			              	assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
		              		(yyval.opPtr) = new SyntaxNode(IN, (yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
			           	}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 287 "sml.tab.ypp"
    {
			           		(yyval.opPtr) = new SyntaxNodeValue(*(yyvsp[(1) - (1)].ival));
			           	}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 291 "sml.tab.ypp"
    {
			           		(yyval.opPtr) = new SyntaxNode(DOTDOT, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
			           	}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 296 "sml.tab.ypp"
    { 
						SyntaxNodeID* idnode = new SyntaxNodeID((yyvsp[(1) - (1)].string));
						index_dummy_vars.push_back(idnode->id);
						(yyval.opPtr) = new SyntaxNode(COMMA, idnode);
					}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 302 "sml.tab.ypp"
    { 
			  			(yyval.opPtr) = (yyvsp[(2) - (3)].opPtr);
			  		}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 308 "sml.tab.ypp"
    {
					SyntaxNodeID* idnode = new SyntaxNodeID((yyvsp[(1) - (1)].string));
					index_dummy_vars.push_back(idnode->id);
					(yyval.opPtr) = new SyntaxNode(COMMA, idnode);
				}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 314 "sml.tab.ypp"
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
#line 328 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("setdef: start -- ID=["<<(yyvsp[(2) - (2)].string)<<"]");
			record_dummy_var();
		}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 332 "sml.tab.ypp"
    {
            YAC_MODEL_LOG("indexing_opt=["<<(yyvsp[(4) - (7)].opPtr)<<"]");
            YAC_MODEL_LOG("stochattr_opt=["<<(yyvsp[(5) - (7)].opPtr)<<"]");
            YAC_MODEL_LOG("setattributes_opt=["<<(yyvsp[(6) - (7)].opPtr)<<"]");
            ModelComp *newmc;
            if (is_stoch_model){
            	assert(false); //TODO: stochastic fix
 //              newmc = new StochModelComp($2, TSET, $4, $6);
 //              addStochInfo(newmc, $5);
            }else{
               newmc = new SetComp((yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].opPtr), (yyvsp[(6) - (7)].opPtr));
            }
            current_model->addComp(newmc);
           	
           	restore_dummy_var();
           	YAC_MODEL_LOG("setdef: -- end -- "<<(yyvsp[(2) - (7)].string));
        }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 351 "sml.tab.ypp"
    { (yyval.opPtr)=NULL; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 353 "sml.tab.ypp"
    {
                 		if((yyvsp[(1) - (2)].opPtr)!=NULL)
                 		{
                 			assert((yyvsp[(1) - (2)].opPtr)->opCode==COMMA);
                 			(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
                 			(yyval.opPtr) = (yyvsp[(1) - (2)].opPtr);
                 		}
                 		else
                 		{
                 			(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(2) - (2)].opPtr));
                 		}
                 	}
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 369 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(DIMEN, new SyntaxNodeValue(*(yyvsp[(2) - (2)].ival)));
	           	}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 373 "sml.tab.ypp"
    {
            		record_dummy_var();
            	}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 376 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNodeOP(ASSIGN, (yyvsp[(3) - (3)].opPtr));
	            	restore_dummy_var();
				}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 381 "sml.tab.ypp"
    {
					assert((yyvsp[(2) - (2)].opPtr)->opCode == IDREF);
					(yyval.opPtr) = new SyntaxNode(WITHIN, (yyvsp[(2) - (2)].opPtr));
				}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 386 "sml.tab.ypp"
    { 
            		(yyval.opPtr) = new SyntaxNode(ORDERED); 
            	}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 395 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("paramdef: -- start -- "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 398 "sml.tab.ypp"
    {
			   	ModelComp *newmc;
			   	if (is_stoch_model){
			   		YAC_MODEL_LOG("paramdef: -- stochInfo --"<<(yyvsp[(5) - (7)].opPtr));
			   		assert(false); //TODO: stochastic fix
//			      	StochModelComp *newmcs = new StochModelComp($2,TPARAM,$4,$6);
//			      	addStochInfo(newmcs, $5);
//			      	newmc = newmcs;
			   	}else{
			   		newmc = new ParamComp((yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].opPtr), (yyvsp[(6) - (7)].opPtr));
			   	}
               	current_model->addComp(newmc);
               	
               	restore_dummy_var();
               	YAC_MODEL_LOG("paramdef: -- end -- "<<(yyvsp[(2) - (7)].string));
            }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 416 "sml.tab.ypp"
    { (yyval.opPtr) = NULL; }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 418 "sml.tab.ypp"
    {
                   	 	if((yyvsp[(1) - (2)].opPtr)!=NULL)
                 		{
                 			assert((yyvsp[(1) - (2)].opPtr)->opCode == COMMA);
                 			(yyvsp[(1) - (2)].opPtr)->push_back((yyvsp[(2) - (2)].opPtr));
                 			(yyval.opPtr) = (yyvsp[(1) - (2)].opPtr);
                 		}
                 		else
                 		{
                 			(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(2) - (2)].opPtr));
                   	 	}
                   	 }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 433 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 434 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 435 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SYMBOLIC);}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 436 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNodeOP((yyvsp[(1) - (2)].optype), (yyvsp[(2) - (2)].opPtr));}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 437 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 438 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNodeOP(DEFINED, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 441 "sml.tab.ypp"
    {(yyval.optype)=LE;}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 442 "sml.tab.ypp"
    {(yyval.optype)=GE;}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 443 "sml.tab.ypp"
    {(yyval.optype)=LT;}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 444 "sml.tab.ypp"
    {(yyval.optype)=GT;}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 445 "sml.tab.ypp"
    {(yyval.optype)=EQ;}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 446 "sml.tab.ypp"
    {(yyval.optype)=NE;}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 453 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("vardef: -- start -- "<<(yyvsp[(2) - (2)].string));
			record_dummy_var();
		}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 456 "sml.tab.ypp"
    {
            ModelComp *newmc;
            if (is_stoch_model){
            	assert(false); //TODO: stochastic fix
//               	newmc = new StochModelComp($2, TVAR, $4, $6);
//               	addStochInfo(newmc, $5);
            }else{
               newmc = new VarComp((yyvsp[(2) - (7)].string), (yyvsp[(4) - (7)].opPtr), (yyvsp[(6) - (7)].opPtr));
            }

            current_model->addComp(newmc);
          	restore_dummy_var();
          	YAC_MODEL_LOG("vardef: -- end -- "<<(yyvsp[(2) - (7)].string));
        }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 472 "sml.tab.ypp"
    {(yyval.opPtr)=NULL;}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 473 "sml.tab.ypp"
    {(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 478 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr)); 
               }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 482 "sml.tab.ypp"
    {
               		assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
             		(yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
             		(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr);
               }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 489 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(BINARY);}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 490 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(INTEGER);}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 491 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNodeOP(LE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 492 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNodeOP(GE, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 493 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(DEFAULT, (yyvsp[(2) - (2)].opPtr));}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 494 "sml.tab.ypp"
    {(yyval.opPtr) = new SyntaxNode(SUFFIX, new SyntaxNodeID((yyvsp[(2) - (3)].string)), (yyvsp[(3) - (3)].opPtr));}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 501 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("consdef: -- start -- "<<(yyvsp[(2) - (2)].string));
				record_dummy_var();
			}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 505 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("consdef: -- indexing -- "<<(yyvsp[(4) - (8)].opPtr));
				ModelComp *newmc;
	            if (is_stoch_model){
					assert(false); //TODO: stochastic fix
//	               newmc = new StochModelComp($2, $4, $7);
//	               addStochInfo(newmc, $5);
	            }else{
	               newmc = new ConsComp((yyvsp[(2) - (8)].string), (yyvsp[(4) - (8)].opPtr), (yyvsp[(7) - (8)].opPtr));
	            }
	            current_model->addComp(newmc);
	            
	            restore_dummy_var();
	            YAC_MODEL_LOG("consdef: -- end -- "<<(yyvsp[(2) - (8)].string));
			}
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 526 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("objdef: -- "<<(yyvsp[(2) - (6)].string));
			ModelComp *newmc;
            if (is_stoch_model){
	            assert(false); //TODO: stochastic fix
//               newmc = new StochModelComp($2, $1, NULL, $5); //as NULL for indexing
//               addStochInfo(newmc, $5);
            }else{
               newmc = new ObjComp((yyvsp[(2) - (6)].string), (yyvsp[(1) - (6)].otype), (yyvsp[(5) - (6)].opPtr));
            }
            current_model->addComp(newmc);
            
            YAC_MODEL_LOG("objdef: -- end -- "<<(yyvsp[(2) - (6)].string));
		}
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 542 "sml.tab.ypp"
    { (yyval.otype) = TMAX; }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 543 "sml.tab.ypp"
    { (yyval.otype) = TMIN; }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 550 "sml.tab.ypp"
    {
					YAC_MODEL_LOG("setexpression: indexing -- "<<(yyvsp[(1) - (1)].opPtr));
					(yyval.opPtr)= (yyvsp[(1) - (1)].opPtr);
				}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 555 "sml.tab.ypp"
    {
		     		YAC_MODEL_LOG("setexpression: identifier -- rule matched -- "<<(yyvsp[(1) - (1)].opPtr));
		     		assert((yyvsp[(1) - (1)].opPtr)->opCode == IDREF);
		     		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
             	}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 561 "sml.tab.ypp"
    { 
	         		YAC_MODEL_LOG("setexpression - setof -- start ");
	         		record_dummy_var();
	         	}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 564 "sml.tab.ypp"
    {
	         		restore_dummy_var();
	         		(yyval.opPtr) = new SyntaxNode(SETOF,(yyvsp[(3) - (4)].opPtr),(yyvsp[(4) - (4)].opPtr));
	            }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 568 "sml.tab.ypp"
    {
					(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               	}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 571 "sml.tab.ypp"
    {
                  	(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
               	}
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 577 "sml.tab.ypp"
    { (yyval.optype) = DIFF; }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 578 "sml.tab.ypp"
    { (yyval.optype) = SYMDIFF; }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 579 "sml.tab.ypp"
    { (yyval.optype) = CROSS; }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 582 "sml.tab.ypp"
    { (yyval.optype) = UNION; }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 583 "sml.tab.ypp"
    { (yyval.optype) = INTER; }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 590 "sml.tab.ypp"
    {
			YAC_MODEL_LOG("lexpr: identifier IN identifier - rule matched! "<<(yyvsp[(1) - (3)].opPtr)<<" "<<(yyvsp[(3) - (3)].opPtr));
			assert((yyvsp[(3) - (3)].opPtr)->opCode == IDREF);
			(yyval.opPtr) = new SyntaxNode(IN,(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
		}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 595 "sml.tab.ypp"
    {
            (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr); 
		}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 599 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype),(yyvsp[(1) - (3)].opPtr),(yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 603 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (2)].optype),(yyvsp[(2) - (2)].opPtr));
     	}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 607 "sml.tab.ypp"
    {
     		(yyval.opPtr) = new SyntaxNode((yyvsp[(2) - (3)].optype), (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr));
     	}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 612 "sml.tab.ypp"
    { (yyval.optype) = NE; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 613 "sml.tab.ypp"
    { (yyval.optype) = LE; }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 614 "sml.tab.ypp"
    { (yyval.optype) = GE; }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 615 "sml.tab.ypp"
    { (yyval.optype) = LT; }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 616 "sml.tab.ypp"
    { (yyval.optype) = GT; }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 617 "sml.tab.ypp"
    { (yyval.optype) = EQ; }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 620 "sml.tab.ypp"
    { (yyval.optype) = AND; }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 621 "sml.tab.ypp"
    {  (yyval.optype) = OR; }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 623 "sml.tab.ypp"
    {  (yyval.optype) = NOT; }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 635 "sml.tab.ypp"
    {
			   	YAC_MODEL_LOG("identifier: ID  -- "<<(yyvsp[(1) - (1)].string));
			   	string key = (yyvsp[(1) - (1)].string);
			   	if(isDummyVar(key))
			   	{
					(yyval.opPtr) = new SyntaxNodeID((yyvsp[(1) - (1)].string));
			   	}
			   	else
			   	{
			   		ModelComp* 	ref = current_model->findModelComp(key);
			   		if(ref->type == TMODEL){
			   		   	(yyval.opPtr) = new SyntaxNodeIDREFM(IDREFM,new SyntaxNodeID((yyvsp[(1) - (1)].string)),static_cast<AmplModel*>(ref));
				   	}
				   	else
				   	{
				   	   	(yyval.opPtr) = new SyntaxNodeIDREF(IDREF,new SyntaxNodeID((yyvsp[(1) - (1)].string)),ref);
				   	}
			   	}
            }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 655 "sml.tab.ypp"
    {
          		assert(is_stoch_model==true);
	            (yyval.opPtr) = new SyntaxNode(ANCESTOR, new SyntaxNodeValue(*(yyvsp[(3) - (6)].ival)), (yyvsp[(6) - (6)].opPtr));
          	}
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 660 "sml.tab.ypp"
    {
          		(yyvsp[(1) - (4)].opPtr)->push_back((yyvsp[(3) - (4)].opPtr));
          		(yyval.opPtr) = (yyvsp[(1) - (4)].opPtr);
          	}
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 665 "sml.tab.ypp"
    {
               	(yyval.model) = current_model;
          		assert((yyvsp[(1) - (2)].opPtr)->opCode == IDREFM);
          		current_model = static_cast<SyntaxNodeIDREFM*>((yyvsp[(1) - (2)].opPtr))->ref;
			}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 671 "sml.tab.ypp"
    {
				 (yyval.opPtr) = new SyntaxNode(DOT, (yyvsp[(1) - (4)].opPtr), (yyvsp[(4) - (4)].opPtr)); 
				 current_model = (yyvsp[(3) - (4)].model);
			}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 680 "sml.tab.ypp"
    {
				YAC_MODEL_LOG("expr_list: expr rule matched! "<<(yyvsp[(1) - (1)].opPtr));
               	(yyval.opPtr) = new SyntaxNode(COMMA,(yyvsp[(1) - (1)].opPtr));
            }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 685 "sml.tab.ypp"
    { 
         		YAC_MODEL_LOG("expr_list: expr_list COMMA expr -- "<<(yyvsp[(1) - (3)].opPtr));
         		YAC_MODEL_LOG("expr_list: expr_list COMMA expr -- "<<(yyvsp[(3) - (3)].opPtr));
               	assert((yyvsp[(1) - (3)].opPtr)->opCode == COMMA);
               	(yyval.opPtr) = (yyvsp[(1) - (3)].opPtr)->push_back((yyvsp[(3) - (3)].opPtr));
            }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 694 "sml.tab.ypp"
    { 
		YAC_MODEL_LOG("expr: value rule matched!" <<(yyvsp[(1) - (1)].opPtr));
		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
	}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 699 "sml.tab.ypp"
    {
		(yyval.opPtr) = (yyvsp[(1) - (1)].opPtr);
	}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 702 "sml.tab.ypp"
    { (yyval.opPtr) = (yyvsp[(2) - (3)].opPtr); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 703 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(MINUS, (yyvsp[(2) - (2)].opPtr)); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 704 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(PLUS, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 705 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(MINUS, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 706 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(TIMES, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 707 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(DIVID, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 708 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(POWER, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 709 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNodeOP(ASSIGN, (yyvsp[(1) - (3)].opPtr), (yyvsp[(3) - (3)].opPtr)); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 710 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (4)].opPtr), (yyvsp[(4) - (4)].opPtr)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 711 "sml.tab.ypp"
    { (yyval.opPtr) = new SyntaxNode(IF, (yyvsp[(2) - (6)].opPtr), (yyvsp[(4) - (6)].opPtr), (yyvsp[(6) - (6)].opPtr)); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 713 "sml.tab.ypp"
    { 
      	(yyval.opPtr) = new SyntaxNode(EXPECTATION, (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 717 "sml.tab.ypp"
    { 
      	(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (4)].optype), (yyvsp[(3) - (4)].opPtr));
      }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 721 "sml.tab.ypp"
    {	
	  	record_dummy_var();
	  }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 724 "sml.tab.ypp"
    {  
		restore_dummy_var();
      	(yyval.opPtr) = new SyntaxNode((yyvsp[(1) - (6)].optype), (yyvsp[(3) - (6)].opPtr), (yyvsp[(5) - (6)].opPtr));
      }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 730 "sml.tab.ypp"
    { (yyval.optype)=ORD;   }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 731 "sml.tab.ypp"
    { (yyval.optype)=CARD;  }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 732 "sml.tab.ypp"
    { (yyval.optype) = SIN; }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 733 "sml.tab.ypp"
    { (yyval.optype) = COS; }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 736 "sml.tab.ypp"
    { (yyval.optype)=SUM; }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 737 "sml.tab.ypp"
    { (yyval.optype)=MAX; }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 738 "sml.tab.ypp"
    { (yyval.optype)=MIN; }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 739 "sml.tab.ypp"
    { (yyval.optype)=PROD; }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 742 "sml.tab.ypp"
    {
            (yyval.opPtr)=new SyntaxNodeValue(*(yyvsp[(1) - (1)].ival));
         }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 745 "sml.tab.ypp"
    { 
            (yyval.opPtr)=new SyntaxNodeValue(*(yyvsp[(1) - (1)].fval));
         }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 748 "sml.tab.ypp"
    { 
            (yyval.opPtr) = new SyntaxNode(INFINITE);
         }
    break;



/* Line 1806 of yacc.c  */
#line 3188 "../src/parser/sml.tab.cpp"
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
#line 753 "sml.tab.ypp"


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
void begin_model(const string& name, SyntaxNode *indexing) 
{
	YAC_MODEL_LOG("begin_model  ["<< name <<"] indexing["<<indexing->print()<<"] -- curr model["<<current_model->name<<"]");
	AmplModel *new_mod = new AmplModel(name,current_model,indexing);
	
	current_model->addComp(new_mod);
	  
	/* and change current model */
	current_model = new_mod;
	YAC_MODEL_LOG("     current model now "<<current_model->name);
}

/* ----------------------------------------------------------------------------
begin_smodel
---------------------------------------------------------------------------- */
void begin_smodel(const string& name, SyntaxNode *indexing, SyntaxNode *stochsets) 
{
	assert(false); // TODO fix stocahstic!
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
 //  StochModel* new_mod = new StochModel(name, stages, nodes, anc, prob, current_model);
  
 //  ModelComp* newmc = new ModelComp(name, TMODEL, indexing, NULL);
 //  newmc->other = new_mod;

 //  new_mod->node = newmc;            /* add pointer-to-node to the model */
 //  current_model->addComp(newmc);
 
   /* and change current model */
 //  current_model = new_mod;
}

/* ----------------------------------------------------------------------------
end_model
---------------------------------------------------------------------------- */
void end_model(const string& name) 
{
	YAC_MODEL_LOG("end_model -- ["<<current_model->name<<"]");
	// Check end block name matches block name
	if (string(name) != current_model->name) {
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
void end_smodel(const string& name){
  // current_model is a StochModel -> convert this into a tree a FlatModels
  YAC_MODEL_LOG("end_smodel -- ["<<current_model->name<<"]");
  assert(false); //TODO fix stochastic
  // Check end block name matches block name
  if (name != current_model->name) {
    cerr << "end stochastic block '" << name << "' encountered in stochastic "
      "block '" << current_model->name << "'" << endl;
    exit(1);
  }

  // this is the ModelComp pointing to the StochModel
 // ModelComp *mc = current_model->node; 
  
  // point that to the expanded flat model tree
 // mc->other = current_model->expandToFlatModel();

  // and change the name of the ModelComp of this model to the name of the 
  // new (AmplModel) model. 
  // (this is a concatenation of the StochModel name and the name of the 
  // first stage)
//  mc->id = mc->other->name;

  // and go back to the parent 
  current_model = current_model->parent;
  is_stoch_model = false;
  
  assert(AmplModel::root == current_model);
  //AmplModel::root->isStochastic = true;
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
	string name = "root";
	
   	AmplModel::root = new AmplModel(name,NULL,NULL);
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

