
%{
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

void yyerror(char *); 
int yylex();
extern FILE *yyin;								
extern FILE *yyout;		

/** state keeping **/
int truncated = 0;
int found_truncated_elements = 0;

// necessary elements present for minimum viable tweet
int created_at_found = 0;
int id_str_found = 0;
int tweet_text_found = 0;
int user_found = 0;

char* tweet_text = NULL;
char* tweet_user_name = NULL;


/** pretty print **/
int in_extended; // if parser is inside "extended_tweet" obj
char* extra_tab = "";
int first_hashtag = 0;
int first_url = 0;

// these do not get cleared between tweets
char* id_arr[100] = { NULL }; // 100 tweets, pls no more, I don't want to manage memory

// functions
void error(char *s){
      fprintf(yyout, "\n\n%s\n\n", s);
      // exit(1);
}

void next_tweet() {
      truncated = 0;
      found_truncated_elements = 0;

      created_at_found = 0;
      id_str_found = 0;
      tweet_text_found = 0;
      user_found = 0;

      tweet_text = NULL;
      tweet_user_name = NULL;

      in_extended = 0;
      extra_tab = "";
      first_hashtag = 0;
      first_url = 0;
}

int starts_with(const char *a, const char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}

// validators
void validate_model_state(){
      if (created_at_found == 0) {
            error("No create_at found");
      }

      if (tweet_text_found == 0) {
            error("No tweet text found");
      }

      if (id_str_found == 0) {
            error("No tweet id found");
      }

      if (user_found == 0) {
            error("No user posting the tweet object found");
      }

      if (truncated == 0 && found_truncated_elements == 1) {
            error("truncated property is missing or is false");
      }
}

void validate_unique_id_str(char* id){
      char* target = id_arr[0];
      while(target != NULL) {
            if (strcmp(target, id) == 0) { // strings equal
                  error("Not unique id_string ids");
            }
            target++;
      }

      target = id;
}

void validate_hashtag(char* hashtag, long start, long end){
      if (start < 0 || end < 0 || start >= end) {
            error("Bad hashtag indices");
      }

      while (hashtag != NULL) {
            if (start > end || *(tweet_text + start) != *hashtag) {
                  error("Found hashtag is not included at tweet");
            }

            start++;
            hashtag++;
      }

      if (start != end) {
            error("Bad hashtag indices");
      }
}

void validate_display_text_upper_bound(long upper){
      if (upper > 140 || upper < 0 || upper > sizeof(tweet_text)) {
            error("bad display text upper bound");
      }
}

void validate_rt_text(char* d){
      if (starts_with(d, "RT@") == 0) {
            error("RT object does not start with RT@");
      }

      d += 3;

      if (starts_with(d, tweet_user_name) == 0) {
            error("RT@<s> : s != original tweet poster name");
      }
}

void validate_tweet_text(char* d){
      if (sizeof(d) > 140) {
            error("Too big tweet text");
      }
}

void validate_day_of_week(char* d){
      int invalid = 1;
      if (strcmp(d, "Mon") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Tue") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Wed") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Thu") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Fri") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Sat") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Sun") == 0) {
            invalid = 0;
      }
      if (invalid == 1) {
            error("Invalid day of week");
      } 
}

void validate_month(char* d){
      int invalid = 1;

      if (strcmp(d, "Jan") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Feb") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Mar") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Apr") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "May") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Jun") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Jul") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Aug") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Sep") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Oct") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Nov") == 0) {
            invalid = 0;
      }
      if (strcmp(d, "Dec") == 0) {
            invalid = 0;
      }
      if (invalid == 1) {
            error("Invalid month");
      } 
}

void validate_day_num(char* d){
      char *ptr;
      long ret;

      ret = strtol(d, &ptr, 10);

      if (ptr != NULL) {
            error("Invalid day");
      }

      if (ret > 31) {
            error("too many days");
      }

      if (ret < 1) {
            error("zero/negative days");
      }
}

void validate_time(char* d){
      int iter = 0;
      char *token = strtok(d, ":");

      while (token) {
            iter++;

            if (iter > 3) {
                  error("miliseconds not supported");
            }

            char *ptr;
            long ret;

            ret = strtol(d, &ptr, 10);

            if (ptr != NULL) {
                  error("Invalid time");
            }
            
            if (iter == 1) {
                  if (ret > 23 || ret < 0) {
                        error("invalid hours");
                  }
            }

            if (iter == 2) {
                  if (ret > 59 || ret < 0) {
                        error("invalid minutes");
                  }
            }

            if (iter == 3) {
                  if (ret > 59 || ret < 0) {
                        error("invalid seconds");
                  }
            }

            token = strtok(NULL, ":");
      }

      if (iter != 3) {
            error("time incomplete");
      }
}

void validate_offset(char* d){
      if (*d != '-' || *d != '+') {
            error("No sign present in offset");
      }

      d++;

      char *ptr;
      long ret;

      ret = strtol(d, &ptr, 10);

      if (ptr != NULL) {
            error("Invalid offset");
      }

      if (ret > 13) {
            error("too big time zone offset");
      }

      if (ret < -13) {
            error("too small time zone offset");
      }
}

void validate_year(char* d){
      char *ptr;
      long ret;

      ret = strtol(d, &ptr, 10);

      if (ptr != NULL) {
            error("Invalid year");
      }

      if (ret > 2019) {
            error("this tweet is from the future wtf");
      }

      if (ret < 2006) {
            error("This tweet is from before twitter servers went online, interesting");
      }
}

void validate_date(char* d){
      int iter = 0;
      char *token = strtok(d, " ");

      while (token) {
            iter++;

            if (iter > 6) {
                  error("Too many tokens on date");
            }
            
            if (iter == 1) {
                  validate_day_of_week(token);
            }

            if (iter == 2) {
                  validate_month(token);
            }

            if (iter == 3) {
                  validate_day_num(token);
            }

            if (iter == 4) {
                  validate_time(token);
            }

            if (iter == 5) {
                  validate_offset(token);
            }

            if (iter == 6) {
                  validate_year(token);
            }

            token = strtok(NULL, " ");
      }
      if (iter != 6) {
            error("Date incomplete");
      }
}

%}

%union {
long ival;
char *sval;
}

%token <ival> INT
%token <ival> BOOLEAN
%token <sval> STRING
%token <sval> STRING_LIT
%token <sval> CHAR
%token <sval> _HASHTAG
%token <sval> UNARY

%token BRACKET_START
%token BRACKET_END

%token ARRAY_START
%token ARRAY_END

%token COLON
%token COMMA
%token QUOTE

%token LINE
%token TAB

// JSON KEYS
%token CREATED_AT
%token ID_STR 
%token TEXT
%token USER 
%token ID
%token NAME
%token SCREEN_NAME
%token LOCATION
%token URLS
%token URL
%token DESCRIPTION
%token RETWEETED_STATUS
%token TWEET
%token TRUNCATED
%token DISPLAY_TEXT_RANGE
%token EXTENDED_TWEET
%token FULL_TEXT
%token ENTITIES
%token INDICES
%token HASHTAG    
%token HASHTAGS
%token UNWOUND
%token TITLE
%token USER_MENTIONS

// Eipame oti einai keno ara ignore
%token PLACE 

%type<sval> user_sn text
%type<ival> display_text_range

%%

program: | program _t | _t program | program one_tweet_object | one_tweet_object 
      {
            fprintf(yyout, "\n\nFinished parsing input");
      }
;

one_tweet_object: BRACKET_START _t
            contents 
            _t tweet_end
      { 
            validate_model_state();
            next_tweet(); // clears state
      }
;

tweet_end: BRACKET_END
      {
            fprintf(yyout, "\n}\n\n");
      }
;

contents: contents created_at
            | contents id_str
            | contents contents_text
            | contents user
            | contents place
            | contents retweeted_status
            | contents tweet
            | contents truncated 
            | contents contents_display_text_range
            | contents extended_tweet
            | contents entities
            | created_at
            | id_str
            | contents_text
            | user
            | place 
            | retweeted_status
            | tweet
            | truncated
            | contents_display_text_range
            | extended_tweet
            | entities
      {
      }
;

// - extended_tweet
extended_tweet: _t EXTENDED_TWEET col e_br_start _t
                  _t FULL_TEXT col e_full_text _lc
                  e_display_text_range
                  extended_entities _t
                  _t e_br_end _lc
      {
            found_truncated_elements = 1;
            fprintf(yyout, "\t},\n");
      }
;

// extended_tweet helpers
e_br_start: BRACKET_START
      {
            in_extended = 1;
            extra_tab = "\t";
      }
;

e_br_end: BRACKET_END
      {
            in_extended = 0;
            extra_tab = "";
      }
;

e_full_text: STRING_LIT
      {
            fprintf(yyout, "\t\"extended_tweet\": {\n");
            fprintf(yyout, "\t\t\"full_text\": \"%s\",\n", $1);            
      }
;

e_display_text_range: display_text_range
      {
            found_truncated_elements = 1;
            fprintf(yyout, "\t\t\"display_text_range\": [0, %ld],\n", $1);
      }
;

// - entities
entities: _t ENTITIES col ent_start _t
            _t hashtags _lc
            _t urls _lc
            _t USER_MENTIONS col ARRAY_START _t ARRAY_END _t
            _t ent_end
      {
            
      }
;

// entities helpers
ent_start: BRACKET_START
      {
            fprintf(yyout, "%s\t\"entities\": {\n", extra_tab);
      }
;

ent_end: BRACKET_END
      {
            fprintf(yyout, "%s\t}\n", extra_tab);
      }
;

urls: URLS col u_start _t urls_a _t ARRAY_END
      {
            if (first_url != 1) {
                  fprintf(yyout, "\n\t\t\t");
            }
            fprintf(yyout, "]\n");
      }
;

// urls helpers
u_start: ARRAY_START
      {
            first_url = 1;
            fprintf(yyout, "\t\t\t\"urls\": [");
      }
;

urls_a: | url_obj u_lc urls_a | url_obj
      {

      }
;

u_lc: _lc
      {
            fprintf(yyout, ",\n");
      }
;

url_obj: _t BRACKET_START _t
            URL col STRING_LIT _lc
            _t UNWOUND col BRACKET_START _t
            _t URL col STRING_LIT _lc
            _t TITLE col STRING_LIT _t
            _t BRACKET_END _t
            _t BRACKET_END _t
      {
            if (first_url == 1) {
                  fprintf(yyout, "\n");
            }

            first_url = 0;
            fprintf(yyout, "\t\t\t\t{\n");

            fprintf(yyout, "\t\t\t\t\t\"url\": \"%s\",\n", $6);
            fprintf(yyout, "\t\t\t\t\t\"unwound\": {,\n");
            fprintf(yyout, "\t\t\t\t\t\t\"url\": \"%s\",\n", $16);
            fprintf(yyout, "\t\t\t\t\t\t\"title\": \"%s\"\n", $21);

            fprintf(yyout, "\t\t\t\t\t}\n");
            fprintf(yyout, "\t\t\t\t}");
      }
;

extended_entities: _t ENTITIES col ent_start _t
            _t hashtags
            ent_end
      {
            
      }
;

// - entities:hashtags
hashtags: _t HASHTAGS col h_start _hashtags_a ARRAY_END _t
      {
            if (first_hashtag != 1) {
                  fprintf(yyout, "\n%s\t\t", extra_tab);
            }
            fprintf(yyout, "]");
            if (in_extended != 1) {
                  fprintf(yyout, ",");
            }
            fprintf(yyout, "\n");
      }
;

// hashtag arr helpers
h_start: ARRAY_START
      {
            first_hashtag = 1;
            fprintf(yyout, "%s\t\t\"hashtags\": [", extra_tab);
      }
;

_hashtags_a: | hashtags_a
      {

      }
;

hashtags_a: hashtags_obj h_lc hashtags_a | hashtags_obj
      {

      }
;

h_lc: _lc
      {
            fprintf(yyout, ",\n");
      }
;

hashtags_obj: _t BRACKET_START _t
                  text
                  _t INDICES col ARRAY_START _t 
                  INT _lc INT 
                  _t ARRAY_END _t _t BRACKET_END _t
      {
            if (first_hashtag == 1) {
                  fprintf(yyout, "\n");
            }

            first_hashtag = 0;
            fprintf(yyout, "%s\t\t\t{\n", extra_tab);

            fprintf(yyout, "%s\t\t\t\t\"text\": \"%s\",\n", extra_tab, $4);
            fprintf(yyout, "%s\t\t\t\t\"indices\": [%ld, %ld]\n", extra_tab, $10, $12);

            fprintf(yyout, "%s\t\t\t}", extra_tab);
      }
;

// - display_text_range | Do not print here
display_text_range: _t DISPLAY_TEXT_RANGE col ARRAY_START _t
                        INT _lc
                        INT _t
                        ARRAY_END _lc
      {
            $$ = $8;
      }
;

contents_display_text_range: display_text_range 
      {
            fprintf(yyout, "\t\"display_text_range\": [0, %ld],\n", $1);
      }
;

// - truncated
truncated:  _t TRUNCATED col 
            BOOLEAN 
            _lc 
      {
            truncated = $4 == TRUE ? 1 : 0;
            fprintf(yyout, "\t\"truncated\": \"%s\",\n", ($4 == TRUE ? "true" : "false"));
      }
;

// - retweeted_status
retweeted_status:       _t RETWEETED_STATUS col BRACKET_START _lc
                        text
                        _t user_sn _t
                        BRACKET_END _lc
      {
            fprintf(yyout, "\t\"retweeted_status\": {\n");
            fprintf(yyout, "\t\t\"text\": %s,\n", $6);

            fprintf(yyout, "\t\t\"user\": {\n");
                  fprintf(yyout, "\t\t\t\"screen_name\": \"%s\",\n", $8);
            fprintf(yyout, "\t\t},\n");

            fprintf(yyout, "\t},\n");
      }
;

// - tweet
tweet:      _t TWEET col BRACKET_START _lc
            text
            _t user_sn _t
            BRACKET_END _lc
      {
            fprintf(yyout, "\t\"tweet\": {\n");
            fprintf(yyout, "\t\t\"text\": %s,\n", $6);

            fprintf(yyout, "\t\t\"user\": {\n");
                  fprintf(yyout, "\t\t\t\"screen_name\": \"%s\",\n", $8);
            fprintf(yyout, "\t\t},\n");

            fprintf(yyout, "\t},\n");
      }
;

user_sn:    _t USER col BRACKET_START _lc _t
            SCREEN_NAME col STRING_LIT _lc _t
            BRACKET_END _lc
      { 
            $$ = $9;
      }
;

// - place : is ignored
place: _t PLACE col BRACKET_START _t BRACKET_END _lc 
      { 
            fprintf(yyout, "\t\"place\": {},\n"); 
      }
;

// - user
user: _t USER col BRACKET_START _lc
      user_id user_name user_screen_name user_location user_url user_description
      _lc BRACKET_END _lc
      {
            user_found = 1;
      }
;

user_id: _t ID col INT _lc
      {
            fprintf(yyout, "\t\"user\": {\n"); // usr obj start
            fprintf(yyout, "\t\t\"id\": %ld,\n", $4);
      }
;

user_name: _t NAME col STRING_LIT _lc
      {
            fprintf(yyout, "\t\t\"name\": \"%s\",\n", $4);
            tweet_user_name = $4;
      }
;

user_screen_name: _t SCREEN_NAME col STRING_LIT _lc
      {
            fprintf(yyout, "\t\t\"screen_name\": \"%s\",\n", $4);
      }
;

user_location: _t LOCATION col STRING_LIT _lc
      {
            fprintf(yyout, "\t\t\"location\": \"%s\",\n", $4);
      }
;

user_url: _t URL col STRING_LIT _lc
      {
            fprintf(yyout, "\t\t\"url\": \"%s\",\n", $4);
      }
;

user_description: _t DESCRIPTION col STRING_LIT
      {
            fprintf(yyout, "\t\t\"description\": \"%s\"\n", $4);

            fprintf(yyout, "\t},\n"); // usr obj end
      }
;

// - contents_text
contents_text: text 
      {
            fprintf(yyout, "\t\"text\": \"%s\",\n", $1);
            tweet_text = $1;
            validate_tweet_text($1);
            tweet_text_found = 1;
      }
;

// - text | Don't print or validate here
text: _t TEXT col STRING_LIT _lc 
      {
            $$ = $4;
      }
;

// -- id_str
id_str: _t ID_STR col STRING_LIT _lc 
      {     
            fprintf(yyout, "\t\"id_str\": \"%s\",\n", $4);
            validate_unique_id_str($4);
            id_str_found = 1;
      }
;

// -- created_at
created_at: _t CREATED_AT col STRING_LIT _lc 
      {
            char* date = $4;

            validate_date(date);
            
            fprintf(yyout, "{\n\t\"created_at\": \"%s\",\n", $4);

            created_at_found = 1;
      }
;

// - optionals
col: COLON
    | TAB COLON
    | TAB COLON TAB
    | COLON TAB
    {}
;

_t: | TAB _t | LINE _t | TAB | LINE {} ;

_lc: LINE
        | TAB LINE
        | COMMA LINE
        | COMMA TAB LINE
        | TAB COMMA TAB LINE
        | TAB 
        | TAB COMMA
        | COMMA TAB
        | TAB COMMA TAB
        | COMMA
        | _t
      {}
;

%%								    


void yyerror(char *s) {
fprintf(stderr, "%s\n", s);
}									

int main ( int argc, char **argv  ) 
{
      extern int yydebug;
      yydebug = 1;

      ++argv; --argc;
      if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
      else
            yyin = stdin;
      yyout = fopen ( "output", "w" );	
      fprintf(yyout, "=== O U T P U T ===\n");
      yyparse();	    
      return 0;
}   
                                                            
