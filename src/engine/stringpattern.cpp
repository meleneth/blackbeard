#include "stringpattern.hpp"
#include"globals.hpp"

// Public data members go here.
StringPattern::StringPattern() 
{
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

void StringPattern::pieces(string target, vector<string>& result)
{
    list<string>::iterator i;
    i = breakers.begin();
    while(i != breakers.end()){
        size_t index = target.find(*i);

        if(index != string::npos){
            string flesh = target.substr(0, index);
            result.push_back(flesh);
            console->log("Fleshy bit: " + flesh);
            target.erase(0, (*i).length() + index);
        }else{
            return;
        }

        ++i;
    }
}

// Private members go here.
// Protected members go here.
