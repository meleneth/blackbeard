#include "getheadersforgroup.hpp"
#include"globals.hpp"

GetHeadersForGroup::GetHeadersForGroup()
{
    finished = 0;
}

GetHeadersForGroup::~GetHeadersForGroup()
{
}

GetXover *GetHeadersForGroup::get_next_job(void)
{
    if(finished)
        return NULL;
    GetXover *g = new GetXover();
    g->group_name = group_name;
    g->lower_id = lower_id;
    if ((lower_id + HEADERS_PER_CHUNK) > upper_id ){
        g->upper_id = upper_id;
        finished = 1;
    }else{
        g->upper_id = lower_id + HEADERS_PER_CHUNK;
        lower_id += HEADERS_PER_CHUNK;
    }
}

