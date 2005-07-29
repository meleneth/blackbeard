#include "strutil.hpp"

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
    Uint32 end = shaggy.length();
    while(is_whitespace(shaggy[start]) && start < end){
        start++;
    }
    while(is_whitespace(shaggy[end]) && start < end){
        end--;
    }

    return shaggy.substr(start, end);
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

