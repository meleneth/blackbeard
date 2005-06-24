#include"console.hpp"
#include"newsgroup.hpp"
#include"stringpattern.hpp"
#include"post_set.hpp"
#include"assert.h"

Console *console;
NewsGroup *newsgroup;
PostSet *current_postset;


void test_header_scoop(void);
void test_string_pattern(void);
void test_postset_objects(void);

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;
    newsgroup = new NewsGroup("misc.test");
    

    char buf[10] = "hello";
    std::string my_string = buf;
    
    buf[0] = 'x';

    console->log(buf);
    console->log(my_string);

    test_string_pattern();
    test_header_scoop();
    test_postset_objects();
	return 0;
}

void test_string_pattern(void)
{
    console->log("Test StringPattern");
    StringPattern *s = new StringPattern();

    s->add_breaker(" - File ");
    s->add_breaker(" of ");
    s->add_breaker(": \"");
    s->add_breaker("\" yEnc (");
    s->add_breaker("/");
    s->add_breaker(")");

    assert(s->does_match("Star Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)"));
    assert(!s->does_match("Star Wars Clone Wars \"Clone Wars Chapter 20.mpg\" yEnc (248/258)"));

    vector<string> pieces;
    s->pieces("Star Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)", pieces);
}

void test_header_scoop(void)
{
    console->log("Test NewsGroup::header_scoop");
    NewsGroup *group = new NewsGroup("alt.binaries.erotica.tigger");

    group->header_scoop("2321\tStar Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)\tHomer P Wylde <brick@house.com>\t18 Jun 2005 05:50:02 GMT\t<d90cma$9hk$2320@answer.sectorfour>\t\t344451\t2669\tXref: answer.sectorfour misc.test:2321");
}

void test_postset_objects(void)
{
    console->log("Test PostSet");

    PostSet *post = new PostSet("brick brothers presents");
    PostFile *info = post->file(1, 2, "info.txt");
    PostFile *sfv = post->file(2, 2, "checksum.SFV");
}
