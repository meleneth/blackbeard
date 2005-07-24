#ifndef ___getheadersforgroup_inc
#define ___getheadersforgroup_inc 1

#include"SDL_types.h"
#include"getxover.hpp"
#include"job.hpp"

#define HEADERS_PER_CHUNK 5000

class GetHeadersForGroup : public Job{
    public:
        GetHeadersForGroup();
        ~GetHeadersForGroup();

        GetXover *get_next_job(void);

        string group_name;
        Uint32 lower_id;
        Uint32 upper_id;
        Uint32 finished;
    private:
    protected:
};

#endif
