#include "strutil.hpp"

#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include<sstream>

using std::string;
using std::stringstream;
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

