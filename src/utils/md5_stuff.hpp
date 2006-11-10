#ifndef ___md5_stuff_inc
#define ___md5_stuff_inc 1

#include <openssl/md5.h>
#include "SDL_types.h"
#include <string>

using std::string;

typedef unsigned char md5[16];
#define CMP_MD5(a,b) (!memcmp((a), (b), 16))

void hex_to_hash(unsigned char *hash, string hex);
string hash_to_hex(unsigned char *hash);

#endif
