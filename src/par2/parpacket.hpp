#ifndef ___parpacket_inc
#define ___parpacket_inc 1

#include "SDL_types.h"

typedef struct {
    char magic_sequence[8];
    unsigned long long int length;
    char packet_hash[16];
    char recovery_set_id[16];
    char type[16];
} par_header;

typedef struct {
    char file_id[16];
    char md5[16];
    char md516k[16];
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
