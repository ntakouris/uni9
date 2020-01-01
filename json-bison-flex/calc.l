
%{
#include "y.tab.h"  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

%}
        
%option noyywrap

num	        [0-9]+
character   [a-zA-Z0-9]
string      {character}+
hashtag     [#]{string}
nl          [\r?\n]+
t           [ |\t]+
unary       [+|-]

%%

"true"                      { yylval.ival = TRUE; return BOOLEAN; }
"false"                     { yylval.ival = FALSE; return BOOLEAN; }

"\"created_at\""          { return CREATED_AT; }
"\"id_str\""              { return ID_STR; }
"\"text\""                { return TEXT; }
"\"place\""             { return PLACE; }
"\"user\""                { return USER; }
"\"id\""                  { return ID; }
"\"name\""                { return NAME; }
"\"screen_name\""         { return SCREEN_NAME; }
"\"location\""            { return LOCATION; }
"\"urls\""                 { return URLS; }
"\"url\""                 { return URL; }
"\"description\""         { return DESCRIPTION; }
"\"retweeted_status\""    { return RETWEETED_STATUS; }
"\"tweet\""               { return TWEET; }
"\"truncated\""           { return TRUNCATED; }
"\"display_text_range\""  { return DISPLAY_TEXT_RANGE; }
"\"extended_tweet\""      { return EXTENDED_TWEET; }
"\"full_text\""           { return FULL_TEXT; }
"\"entities\""            { return ENTITIES; }
"\"indices\""             { return INDICES; }
"\"hashtags\""            { return HASHTAGS; }
"\"hashtag\""             { return HASHTAG; }
"\"unwound\""             { return UNWOUND; }
"\"title\""               { return TITLE; }
"\"user_mentions\""       { return USER_MENTIONS; }


","                     { return COMMA; }
"["                     { return ARRAY_START; }
"]"                     { return ARRAY_END; }
"{"                     { return BRACKET_START; }
"}"                     { return BRACKET_END; }
" "                     { return TAB; }
":"                     { return COLON; }
"\""                    { return QUOTE; }

{num} 	        { yylval.ival = atol(yytext);   return INT; }	
{character}     { yylval.sval = yytext;         return CHAR; }
{string}        { yylval.sval = yytext;         return STRING; }
\"[^\"]*\"      {   // String literals
                    *(yytext + strlen(yytext) - 1) = 0;
                    yytext++;  
                    yylval.sval = yytext; 
                    return STRING_LIT; 
                }
{hashtag}       { yylval.sval = yytext;         return _HASHTAG; }
{unary}         { yylval.sval = yytext;         return UNARY; }

{nl}            { return LINE; }
{t}             { return TAB; }
%%
