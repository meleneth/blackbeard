#include "newsgroup.hpp"

/* Quick Code Checks (first char of NNTP code) */
const int CMD_OK      		= 2;
const int CMD_ERROR   		= 4;
const int CMD_UNKNOWN 		= 5;

/* Base NNTP commands rfc(977) */
const int READY_POST  		= 200;
const int READY_NOT_POST 	= 201;
const int SLAVE_MODE 		= 202;
const int CLOSING 		= 205;
const int GROUP_SELECTED 	= 211;
const int LIST_FOLLOWS 		= 215;
const int BOTH_FOLLOW 		= 220;
const int HEAD_FOLLOWS 		= 221;
const int BODY_FOLLOWS 		= 222;
const int ARTICLE_OK 		= 223;
const int GLIST_FOLLOWS 	= 231;
const int TRANSFER_OK 		= 235;
const int POST_OK 		= 240;
const int AUTH_OK 		= 281;
const int SEND_TRANSFER 	= 335;
const int SEND_POST 		= 340;
const int DISCONNECTED 		= 400;
const int NO_SUCH_GROUP 	= 411;
const int NO_GROUP_SELECTED 	= 412;
const int NO_ARTICLE_SELECTED	= 420;
const int NO_NEXT 		= 421;
const int NO_PREVIOUS 		= 422;
const int NO_ARTICLE_NUMBER 	= 423;
const int NO_ARTICLE_FOUND 	= 430;
const int NOT_WANTED 		= 435;
const int TRANSFER_FAILED 	= 436;
const int REJECTED 		= 437;
const int NO_POSTING_ALLOWED 	= 440;
const int POST_FAILED 		= 441;
const int COMMAND_ERROR 	= 500;
const int COMMAND_SYNTAX 	= 501;
const int ACCESS_RESTRICTION 	= 502;
const int PROGRAM_FAULT 	= 503;

/* Extended NNTP commands (rfc 2980) */	
const char* AUTHINFO 	= "authinfo";
const char* ARTICLE 	= "article";
const char* BODY 	= "body";
const char* CHECK 	= "check";
const char* DATE 	= "date";
const char* GROUP 	= "group";
const char* HEAD 	= "head";
const char* HELP 	= "help";
const char* IHAVE 	= "ihave";
const char* LAST 	= "last";
const char* LIST 	= "list";
const char* LISTGROUP 	= "listgroup";
const char* MODE_STREAM = "mode stream";
const char* MODE_READER = "mode reader";
const char* NEWSGROUPS  = "newsgroups";
const char* NEWNEWS 	= "newnews";
const char* NEXT 	= "next";
const char* POST 	= "post";
const char* QUIT 	= "quit";
const char* SLAVE 	= "slave";
const char* STAT 	= "stat";
const char* TAKETHIS 	= "takethis";
const char* XGTITLE 	= "xgtitle";
const char* XHDR 	= "xhdr";
const char* XOVER 	= "xover";
const char* XPAT 	= "xpat";

// Public data members go here.
NewsGroup::NewsGroup(int bar) // Constructor
{
}
    
NewsGroup::~NewsGroup() // Destructor
{
}

int
NewsGroup::status_code()
{
	return return_code;
}

// Private members go here.
// Protected members go here.
