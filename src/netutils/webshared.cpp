#include "webshared.hpp"
#include "strutil.hpp"

#include <sstream>
#include<iomanip>

using std::setprecision;
using std::stringstream;

std::string postset_status_line(WebRequest *request, PostSet *set)
{
    WebRequest r = WebRequest(request->get_uri());

    r.delete_param("tick");
    r.param("psi", set->index);
    r.filename = "postfiles";

    stringstream s;
    s   << "ps_" << set->group->index() << "_" << set->index
        << "|| |" << set->num_files()
        << "|| |" << size_image_string(set->num_bytes())
        << "|| |" << human_readable_bytes(set->num_bytes());

    r.filename = "downloadpostset";
    s   << "|| ping_url('" << r.get_uri() << "')| Download"
        << "|| |" << setprecision(3) << set->completed_percent() << "%";

    r.filename = "postfiles";
    s   << "||ui.open_screen_with_url_data('"<< r.get_uri() << "')|" << js_escape( replace_substrings(set->subject, "|", "").substr(0, 120));

    return s.str();
}

string size_image_string(unsigned long long num_bytes)
{
#define CD_BYTES 600000000ULL
    if(num_bytes <  CD_BYTES)
        return "img: floppy.png";

    if(num_bytes < (2 * CD_BYTES))
        return "img: one_cd.png";

    if(num_bytes < (3 * CD_BYTES))
        return "img: two_cds.png";

    if(num_bytes < (4 * CD_BYTES))
        return "img: three_cds.png";

    if(num_bytes < (5 * CD_BYTES))
        return "img: four_cds.png";

    if(num_bytes < (6 * CD_BYTES))
        return "img: five_cds.png";

    if(num_bytes < (12 * CD_BYTES))
        return "img: one_dvd.png";

    if(num_bytes < (18 * CD_BYTES))
        return "img: two_dvds.png";

    if(num_bytes < (24 * CD_BYTES))
        return "img: three_dvds.png";

    return "";
}


