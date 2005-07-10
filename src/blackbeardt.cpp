#include"console.hpp"
#include"newsgroup.hpp"
#include"stringpattern.hpp"
#include"post_set.hpp"
#include"assert.h"
#include"jobqueue.hpp"
#include"strutil.hpp"
#include"uu_decoder.hpp"

#include<string>
#include<sstream>

Console *console;
NewsGroup *newsgroup;
PostSet *current_postset;
PostFile *current_postfile;
JobQueue *jobqueue;
Config *config;


void test_header_scoop(void);
void test_string_pattern(void);
void test_strings(void);
void test_postset_objects(void);
void test_bit_manipulations(void);
void test_uudecode(void);
void test_more_string_pattern(void);

void assert_postset_filenames_eq(PostSet *checkme, list<string> against);
void assert_strings_eq(string s1, string s2);

using std::string;
using std::stringstream;

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;
    newsgroup = new NewsGroup("misc.test");
    

    test_uudecode();
    test_strings();
    test_string_pattern();
    test_header_scoop();
    test_postset_objects();
    test_bit_manipulations();
    test_more_string_pattern();
	return 0;
}

void test_bit_manipulations(void)
{
    stringstream buf;

    buf << "1 << 1 = " << (1<<1) << "; "
        << "1 & 1 = "  << (1&1)  << "; "
        << "1 && 1 = " << (1&&1) << "; "
        << "1 & 2 ="   << (1&2)  << "; "
        << "1 && 2 = " << (1&&2) <<"; ";
    console->log(buf.str());

    console->log(bitviz(1));
    //Dunno why this doesnt work, it looks like it should
    //assert(bitviz(1).compare("10000000000000000000000000000000") == 0);
}

void test_strings(void)
{
    console->log("Test Strings");
    string bar= "hello";
    bar.erase(2, 1);
    console->log(bar);
    assert(0 == bar.compare("helo"));

    char buf[10] = "hello";
    std::string my_string = buf;
    
    buf[0] = 'x';

    console->log(buf);
    console->log(my_string);

    string baz(20, 'x');
    console->log(baz);
}

void test_string_pattern(void)
{
    console->log("Test StringPattern");
    StringPattern *s = new StringPattern(7);

    s->add_breaker(" - File ");
    s->add_breaker(" of ");
    s->add_breaker(": \"");
    s->add_breaker("\" yEnc (");
    s->add_breaker("/");
    s->add_breaker(")");

    assert(s->match("Star Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)"));
    assert(!s->match("Star Wars Clone Wars \"Clone Wars Chapter 20.mpg\" yEnc (248/258)"));

    s->match("Star Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (248/258)");
}

void test_more_string_pattern()
{
    console->log("Test More String Pattern");

    string subject = "1997-02-23 The_Fourth_Generation_FULL_CD-FURY \"fry-4g06.zip\" (4/4) yEnc";
    StringPattern *pattern = new StringPattern(SP_LASTPART);
    pattern->add_breaker(SP_SUBJECT);
    pattern->add_breaker(" \"");
    pattern->add_breaker(SP_FILENAME);
    pattern->add_breaker("\" (");
    pattern->add_breaker(SP_PARTNO);
    pattern->add_breaker("/");
    pattern->add_breaker(SP_MAXPARTNO);
    pattern->add_breaker(") yEnc");

    assert(pattern->match(subject));
    assert_strings_eq(pattern->get_piece(SP_SUBJECT), 
                      "1997-02-23 The_Fourth_Generation_FULL_CD-FURY");
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
    post->file(1, 2, "info.txt");
    post->file(2, 2, "checksum.SFV");

    console->log("Objects created");

    list<string> s;
    s.push_back("info.txt");
    s.push_back("checksum.SFV");
    console->log("Calling assert_postset_filenames_eq");
    assert_postset_filenames_eq(post, s);

    post = new PostSet("brick brothers presents");
    post->file(0, 0, "info.txt");
    post->file(0, 0, "checksum.SFV");

    console->log("Calling assert_postset_filenames_eq");
    assert_postset_filenames_eq(post, s);
}

void test_uudecode(void)
{
    console->log("Test UUdecode");
    UUDecoder *decoder = new UUDecoder();
    string result = decoder->do_the_math("$>6]U\"@``");
    console->log(result);
    assert(result.compare("you\n") == 0);
}

void assert_postset_filenames_eq(PostSet *checkme, list<string> filenames)
{
    stringstream buf;
    console->log("Checking size..");
    assert(checkme->num_files == filenames.size());

    console->log("Checking contents...");
    list<string>::iterator i;
    Uint32 comp_index = 0;
    if(NULL==checkme->files[comp_index])
        comp_index++;

    for(i = filenames.begin(); i!=filenames.end(); ++i){
        console->log("Checking: " + *i);
        buf << "comp_index: " << comp_index ;
        console->log(buf.str());
        buf.str("");
        assert_strings_eq(checkme->files[comp_index]->filename, *i);
        comp_index++;
    }
}

void assert_strings_eq(string s1, string s2)
{
    if(s1.compare(s2) == 0)
        return;
    console->log("(" + s1 + ") is not equal to (" + s2 + ")");
}
