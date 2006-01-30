#ifndef ___decoderfactory_inc
#define ___decoderfactory_inc 1

#include "SDL_types.h"
#include "post_file.hpp"
#include "decoder.hpp"
#include "yenc_decoder.hpp"
#include "uu_decoder.hpp"

Decoder *decoder_for_body(NewsGroupPost *post, PostFile *file);

#endif
