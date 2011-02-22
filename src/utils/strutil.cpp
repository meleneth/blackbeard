#include "strutil.hpp"

#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <sstream>
#include <iomanip>
#include <math.h>
#include "strutil.hpp"
#include "console.hpp"

using std::string;
using std::stringstream;
using std::setprecision;
using std::ofstream;
using std::hex;

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

string bitviz(Uint32 showme)
{
    Uint32 i;
    string res('-', 32);

    for(i = 0; i<32 ; i++){
        res[i] = (showme & (1<<i)) ? '1' : '0';
    }
    return res;
}

string trimmed(string shaggy)
{
    Uint32 start = 0;
    Uint32 end = shaggy.length() -1;
    while(is_whitespace(shaggy[start]) && start < end){
        start++;
    }
    while(is_whitespace(shaggy[end]) && start < end){
        end--;
    }

    return shaggy.substr(start, end-1);
}

Uint32 is_whitespace(char c)
{
    switch(c){
        case ' ':
        case '\t':
            return 1;
        default:
        return 0;
    }
}

string get_crc_32(string process_me)
{
    static int generated_table = 0;
    static Uint32 CRC32Table[256];

    if(!generated_table){
        generate_crc_table(CRC32Table);
        generated_table = 1;
    }

    Uint32 max_no = process_me.length();
    Uint32 CRC = 0;

    for(Uint32 i=0; i<max_no; i++){
        CRC = (CRC >> 8 )^ CRC32Table[ process_me[i] ^ (( CRC ) & 0x000000FF )];
    }

    stringstream result;
    result << hex << CRC;
    return result.str();
}

void generate_crc_table(Uint32 *table)
{
    Uint32 Polynomial = 0xEDB88320;
    Uint32 CRC;

    for(int i = 0; i < 256; i++){
        CRC = i;
        for(int j = 8; j > 0; j--) {
            if(CRC & 1){
                CRC = (CRC >> 1) ^ Polynomial;
            } else {
                CRC >>= 1;
            }
        }
        *(table + i)= CRC;
    }
}

string replace_all_substrings(string from_me, string take_me, string for_me)
{
    size_t s = from_me.find(take_me);
    Uint32 t_length = take_me.length();
    while(s < string::npos){
        from_me.replace(s, t_length, for_me);
        s=from_me.find(take_me);
    }
    return from_me;
}

string replace_substrings(string from_me, string take_me, string for_me)
{
    string result;

    size_t s = from_me.find(take_me);
    Uint32 t_length = take_me.length();
    while(s < string::npos){
        result += (from_me.substr(0, s) + for_me);
        from_me.replace(0, s + t_length, "");
        s=from_me.find(take_me);
    }
    return result + from_me;
}

string xml_escape(string escape_me)
{
    escape_me = replace_substrings(escape_me, "&", "&amp;");
    escape_me = replace_substrings(escape_me, "\"", "&x22;");
    escape_me = replace_substrings(escape_me, "<", "&lt;");
    return replace_substrings(escape_me, ">", "&gt;");
}

string xml_unescape(string unescape_me)
{
    unescape_me = replace_substrings(unescape_me, "&x22;", "\"");
    unescape_me = replace_substrings(unescape_me, "&gt;", ">");
    unescape_me = replace_substrings(unescape_me, "&lt;", "<");
    return replace_substrings(unescape_me, "&amp;", "&");
}

string js_escape(string escape_me)
{
    return replace_substrings(replace_substrings(escape_me, "\\", "\\\\"), "'", "\\'");
}

string safe_dirname(string unsafe)
{
    string s = unsafe;
    Uint32 max_no = s.length();
    Uint32 is_unsafe;
    for(Uint32 i=0; i<max_no; ++i){
        is_unsafe = 1;
        if(s[i] >= 'a' && s[i] <= 'z')
            is_unsafe = 0;
        if(s[i] >= 'A' && s[i] <= 'Z')
            is_unsafe = 0;
        if(is_unsafe)
            s[i] = '_';
    }

    size_t x = s.find("__");
    while(x < string::npos){
        s.replace(x, 2, "_");
        x=s.find("__");
    }
    return s;
}

string human_readable_bytes(unsigned long long int num_bytes)
{
    stringstream result;

    unsigned long long int Kay  = static_cast<unsigned long long int>(pow(2, 10));
    unsigned long long int Mega = static_cast<unsigned long long int>(pow(2, 20));
    unsigned long long int Giga = static_cast<unsigned long long int>(pow(2, 30));

    if(num_bytes < 1)
        return "0b";

    //result << setprecision(2);

    if(num_bytes > Giga) {
        double bytes = num_bytes/Giga;
        bytes = trunc(bytes * 10) / 10;
        result << bytes << "GB";
    } else if (num_bytes > Mega) {
        result << trunc(num_bytes / Mega) << "MB";
    } else {
        result << trunc(num_bytes / Kay) << "kB";
    }
    
    return result.str();
}

string file_extension(string filename)
{
    if(filename.size() < 4)
        return "";
    vector<string> pieces;

    Tokenize(filename, pieces, ".");
    Uint32 num = pieces.size() -1;
    return pieces[num];
}

