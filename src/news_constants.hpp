#ifndef ___news_constants_inc
#define ___news_constants_inc 1

typedef enum { SP_SUBJECT, SP_FILENO, SP_MAXFILENO,
               SP_FILENAME, SP_PARTNO, SP_MAXPARTNO, SP_LASTPART } SubjectPart;

/* Quick Code Checks (first char of NNTP code) */
#define CMD_OK       2
#define CMD_ERROR    4
#define CMD_UNKNOWN  5

/* Base NNTP commands rfc(977) */
#define READY_POST           200
#define READY_NOT_POST       201
#define SLAVE_MODE           202
#define CLOSING              205
#define GROUP_SELECTED       211
#define LIST_FOLLOWS         215
#define BOTH_FOLLOW          220
#define HEAD_FOLLOWS         221
#define BODY_FOLLOWS         222
#define ARTICLE_OK           223
#define GLIST_FOLLOWS        231
#define TRANSFER_OK          235
#define POST_OK              240
#define AUTH_OK              281
#define SEND_TRANSFER        335
#define SEND_POST            340
#define DISCONNECTED         400
#define NO_SUCH_GROUP        411
#define NO_GROUP_SELECTED    412
#define NO_ARTICLE_SELECTED  420
#define NO_NEXT              421
#define NO_PREVIOUS          422
#define NO_ARTICLE_NUMBER    423
#define NO_ARTICLE_FOUND     430
#define NOT_WANTED           435
#define TRANSFER_FAILED      436
#define REJECTED             437
#define NO_POSTING_ALLOWED   440
#define POST_FAILED          441
#define COMMAND_ERROR        500
#define COMMAND_SYNTAX       501
#define ACCESS_RESTRICTION   502
#define PROGRAM_FAULT        503

/* Extended NNTP commands (rfc 2980) */    
#define AUTH_USER     "authinfo user "
#define AUTH_PASS     "authinfo pass "
#define ARTICLE       "article "
#define BODY          "body "
#define DATE          "date "
#define GROUP         "group "
#define HEAD          "head "
#define HELP          "help"
#define LAST          "last"
#define LIST          "list"
#define LISTGROUP     "listgroup"
#define MODE_READER   "mode reader"
#define NEXT          "next"
#define POST          "post "
#define QUIT          "quit"
#define STAT          "stat "
#define XOVER         "xover "
#define OVERVIEW_FMT  "LIST OVERVIEW.FMT"

/* Responses (rfc 2980) */
#define AUTH_REQUIRED  "480"

#endif
