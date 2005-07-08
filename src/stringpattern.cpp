#include"news_constants.hpp"
#include"stringpattern.hpp"
#include"globals.hpp"

StringPattern::StringPattern(Uint32 max_part_no)
{
    part_order.resize(max_part_no);
}
    
StringPattern::~StringPattern() 
{
}

int StringPattern::does_match(string target)
{
    list<string>::iterator i;
    i = breakers.begin();
    while(i != breakers.end()){
        size_t index = target.find(*i);

        if(index != string::npos){
            target.erase(0, (*i).length() + index);
        }else{
            return 0;
        }

        ++i;
    }
    return 1;
}

void StringPattern::add_breaker(string target)
{
    breakers.push_back(target);
}

void StringPattern::add_breaker(Uint32 target)
{
    part_order[target] = breakers.size();
}

void StringPattern::pieces(string target)
{
    list<string>::iterator i;
    results.clear();
    i = breakers.begin();
    while(i != breakers.end()){
        size_t index = target.find(*i);

        if(index != string::npos){
            string flesh = target.substr(0, index);
            results.push_back(flesh);
            target.erase(0, (*i).length() + index);
        }else{
            return;
        }

        ++i;
    }
    results.push_back(target);
}

string StringPattern::get_piece(Uint32 index)
{
    return results[part_order[index]];
}

Uint32 StringPattern::get_piecen(Uint32 index)
{
    return atoi(get_piece(index).c_str());
}

