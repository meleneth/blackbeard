#include"news_constants.hpp"
#include"stringpattern.hpp"
#include"globals.hpp"

StringPattern::StringPattern(Uint32 max_part_no)
{
    part_order.resize(max_part_no);
    results.resize(max_part_no);
    for(Uint32 i=0; i<max_part_no; i++){
        part_order[i] = 31337;
        results[i] = "";
    }
    this->max_part_no = max_part_no;
}
    
StringPattern::~StringPattern() 
{
}

void StringPattern::add_breaker(string target)
{
    breakers.push_back(target);
}

void StringPattern::add_breaker(Uint32 target)
{
    part_order[target] = breakers.size();
}

int StringPattern::match(string target)
{
    list<string>::iterator s;

    for(Uint32 i=0; i<max_part_no; i++){
        results[i] = "";
    }

    Uint32 count = 0;
    s = breakers.begin();
    while(s != breakers.end()){
        size_t index = target.find(*s);

        if(index == string::npos)
            return 0;
    
        results[count] = target.substr(0, index);
        target.erase(0, (*s).length() + index);
        count++;
        s++;
    }
    results[count] = target;
    return 1;
}

string StringPattern::get_piece(Uint32 index)
{
    if(part_order[index] == 31337)
        return "";
    return results[part_order[index]];
}

Uint32 StringPattern::get_piecen(Uint32 index)
{
    string res = get_piece(index);
    return res.compare("") == 0 ? 0 : atoi(res.c_str());
}

