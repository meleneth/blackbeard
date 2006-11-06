#ifndef ___webshared_inc
#define ___webshared_inc 1

#include "SDL_types.h"
#include <string>

#include "webrequest.hpp"
#include "post_set.hpp"

std::string postset_status_line(WebRequest *request, PostSet *set);
std::string size_image_string(unsigned long long num_bytes);

#endif
