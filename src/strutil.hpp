#ifndef ___strutil_inc
#define ___strutil_inc 1

#include <string>
#include <algorithm>
#include <vector>

#include"SDL_types.h"
#include"globals.hpp"

using std::string;
using std::vector;

void Tokenize(const string& str, vector<string>& tokens, const string& delimiters);

#endif