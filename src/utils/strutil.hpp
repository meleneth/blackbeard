#ifndef ___strutil_inc
#define ___strutil_inc 1

#include <string>
#include <algorithm>
#include <vector>

#include"SDL_types.h"

using std::string;
using std::vector;

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);
string bitviz(Uint32 showme);
string trimmed(string shaggy);
Uint32 is_whitespace(char c);
string get_crc_32(string process_me);
void generate_crc_table(Uint32 *table);
string replace_all_substrings(string from_me, string take_me, string for_me);
string replace_substrings(string from_me, string take_me, string for_me);

string xml_escape(string escape_me);
string xml_unescape(string unescape_me);
string js_escape(string escape_me);
string safe_dirname(string unsafe);
string human_readable_bytes(double num_bytes);

#endif
