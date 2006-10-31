#ifndef ___parpacket_inc
#define ___parpacket_inc 1

#include "SDL_types.h"

#include "md5_stuff.hpp"

typedef struct {
    char magic_sequence[8];
    unsigned long long int length;
    md5 packet_hash;
    md5 recovery_set_id;
    char type[16];
} par_header;

typedef struct {
    char file_id[16];
    md5 md5sum;
    md5 md516k;
    unsigned long long int length;
} par_filedesc_packet;

class ParPacket {
    public:
        ParPacket(int bar);
        ~ParPacket();

        int Foo(void);

        par_header header;
        char *body;
    private:
    protected:
};

#endif
