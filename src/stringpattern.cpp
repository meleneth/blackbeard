#include"news_constants.hpp"
#include"stringpattern.hpp"
#include"globals.hpp"

StringPattern::StringPattern(Uint32 max_part_no)
{
    part_order.resize(max_part_no);
    results.resize(max_part_no);
    for(unsigned int i=0; i<max_part_no; i++){
        part_order[i] = 0;
        results[i] = "";
    }
    this->max_part_no = max_part_no;
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

int StringPattern::pieces(string target)
{
    list<string>::iterator s;
    results.clear();

    for(Uint32 i=0; i<max_part_no; i++){
        results[i] = "";
    }

    s = breakers.begin();
    while(s != breakers.end()){
        size_t index = target.find(*s);

        if(index == string::npos){
            return 0;
        }else{
            string flesh = target.substr(0, index);
            results.push_back(flesh);
            target.erase(0, (*s).length() + index);
        }
        ++s;
    }
    results.push_back(target);
    return 1;
}

string StringPattern::get_piece(Uint32 index)
{
    return results[part_order[index]];
}

Uint32 StringPattern::get_piecen(Uint32 index)
{
    string res = get_piece(index);
    return res.compare("") == 0 ? 0 : atoi(res.c_str());
}

