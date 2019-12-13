
<character>      ::= <letter> | <digit> | <symbol>
<letter>         ::= "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
<digit>          ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
<symbol>         ::=  "|" | " " | "!" | "#" | "$" | "%" | "&" | "(" | ")" | "*" | "+" | "," | "-" | "." | "/" | ":" | ";" | ">" | "=" | "<" | "?" | "@" | "[" | "\" | "]" | "^" | "_" | "`" | "{" | "}" | "~"
<integer>        ::= <integer><digit> | <digit>
<string>         ::= <string><character> | <character>
<hashtag_string> ::= "#"<string>
<t>              ::= <t> "\t" | <t> " " | <t> "\n" | <t> "" | "\t" | " " | "" | "\n"
<boolean>        ::= "true" | "false"

<root> ::= "{" <t> <contents> <t> "}"

<contents> ::= <created_at> <id_str> <text> <user> <place> <retweeted_status> <tweet> <truncated> <display_text_range> <extended_tweet> <entities> 


<created_at> ::= <t>""created_at": "" <day> " " <month> " " <time> " " <offset> " " <year> ""," <t>

<day> ::= <string>
<month> ::= <string>
<time> ::= <integer>  ":" <integer>  ":" <integer> 
<offset> :== ("+" | "-") <integer> 
<year> :== <integer> 


<id_str> ::= <t> ""id_str": "" <id_str_digits> ""," <t>
<id_str_digits> ::= <integer>

<text> ::= <t> ""text": "" <string> ""," <t>


<user_root> ::= <t> ""user": {" <user_contents> <t> "}," <t>
<user_contents> ::= <user_id> <user_name> <user_screen_name> <user_location> <user_url> <user_description>

<user_id> ::= <t> ""id": " <integer> "," <t>
<user_name> ::= <t>""name": " <string> "," <t>
<user_screen_name> ::= <t>""screen_name": " <string> "," <t>
<user_location> ::= <t>""location": " <string> "," <t>
<user_url> ::= <t>""url": " <string> "," <t>
<user_description> ::= <t>""description": " <string> "" <t>


<place> ::= <t>""place": {}," <t>

#### Optional Fields ####
<retweeted_status> ::= <t>""retweeted_status": {" <retweeted_status_contents> "}," <t>

<retweeted_status_contents> ::= <retweeted_status_text> <retweeted_status_user>

<retweeted_status_text> ::= <t>""text": " <string> "," <t>

<retweeted_status_user> ::= <t>""user": {" <retweeted_status_user_screen_name> "}" <t>
<retweeted_status_user_screen_name> ::= <t>""screen_name": " <string> "" <t>

<display_text_range> ::= <t>""display_text_range": [ 0," <integer> "]," <t>

<truncated> ::= <t>""truncated": " <boolean> "," <t>

<tweet> ::= <t>""tweet": {" <tweet_contents> "}," <t>
<tweet_status_contents> ::= <tweet_status_text> <tweet_status_user>

<tweet_status_text> ::= <t>""text": RT@" <string> " " <string> "," <t>

<tweet_status_user> ::= <t>""user": {" <tweet_status_user_screen_name> "}" <t>
<tweet_status_user_screen_name> ::= <t>""screen_name": " <string> "" <t>

<extended_tweet> ::= <t>""extended_tweet": {" <extended_tweet_contents> "}," <t>
<extended_tweet_contents> ::= <extended_tweet_contents_full_text> <display_text_range> <entities>
<extended_tweet_contents_full_text> ::= <t>""extended_tweet_contents_full_text": " <string> "," <t>

# Last Field of json at root level (entities)

<entities> ::= <t>""entities": {" <entities_content> <t>"}" <t>

<entities_content> ::= <hashtags> <entities_urls> <entities_user_mentions> 

<hashtags> ::= <t>""hashtags": {" <hashtags_array> "}," <t>
<hashtags_array> ::= <hashtags_array_entry> "," <hashtags_array> | <hashtags_array_entry>
<hashtags_array_entry> ::= "{" <hashtag_array_entry_object> "}"
<hashtag_array_entry_object> ::= <text> <indices>
<indices> ::= <t>""indices": [" <integer> " ," <integer> "]," <t>

<entities_urls> ::= <t>""urls": [" <t> <entities_urls_array> "]," <t>

<entities_urls_array> ::= <entities_urls_array_entry> "," <t> <entities_urls_array> | <entities_urls_array_entry> "" <t>
<entities_urls_array_entry> ::= <t>"{" <t> <entities_urls_array_entry_content> "}"

<entities_urls_array_entry_content> ::= <entities_urls_array_entry_content_url> <entities_urls_array_entry_content_unwound_root>
<entities_urls_array_entry_content_url> ::= <t>""url": " <string> ""," <t>

<entities_urls_array_entry_content_unwound_root> ::= <t>""unwound": {" <entities_urls_array_entry_content_unwound_content> <t>"}" <t>

<entities_urls_array_entry_content_unwound_content> ::= <entities_urls_array_entry_content_unwound_url> <entities_urls_array_entry_content_unwound_title>
<entities_urls_array_entry_content_unwound_url> ::= <t>""url": "" <string> ""," <t>
<entities_urls_array_entry_content_unwound_title> ::= <t>""title": "" <string> ""," <t>

<entities_user_mentions> ::= <t>""user_mentions": []," <t>
