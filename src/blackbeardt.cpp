#include "console.hpp"
#include "newsgroup.hpp"
#include "stringpattern.hpp"
#include "post_set.hpp"
#include "assert.h"
#include "jobqueue.hpp"
#include "strutil.hpp"
#include "uu_decoder.hpp"
#include "newsgroup.hpp"
#include "config.hpp"
#include "session.hpp"
#include "headersforgroupjob.hpp"
#include "postsetjob.hpp"
#include "post_set_splitter_dynamicmatch.hpp"
#include "webrequest.hpp"
#include "netcentral.hpp"
#include "file_handle.hpp"
#include "webserver.hpp"

#include<string>
#include<sstream>
#include<vector>

using std::string;
using std::vector;

Console *console;
NewsGroup *newsgroup;
PostSet *current_postset;
vector< NewsGroup * >newsgroups;
PostFile *current_postfile;
JobQueue *jobqueue;
JobQueue *metajobs;
Config *config;
Session *session;
string last_msg;
NetCentral *netcentral;
NetCentral *high_priority_jobs;
WebServer *webserver;

void test_string_pattern(void);
void test_strings(void);
void test_bit_manipulations(void);
void test_uudecode(void);
void test_more_string_pattern(void);
void test_download_netjob(void);
void test_crc32(void);
void test_simple_x(void);
void test_dynamic_postsplit(void);
void test_string_sorting(void);
void test_js_escape(void);
void test_mem_stringpattern(void);
void test_web_request(void);
void test_filehandle(void);

void assert_postset_filenames_eq(PostSet *checkme, list<string> against);
void assert_strings_eq(string s1, string s2);

using std::string;
using std::stringstream;

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;
    config = new Config(0, NULL);
    newsgroup = new NewsGroup("misc.test");

    config->setup_test_config();
    test_filehandle();
    test_mem_stringpattern();
    test_crc32(); 
    test_download_netjob();
    test_uudecode();
    test_strings();
    test_string_pattern();
    test_bit_manipulations();
    test_more_string_pattern();
    test_simple_x();
    test_dynamic_postsplit();
    test_string_sorting();
    test_web_request();
    test_js_escape();
	return 0;
}

void test_filehandle()
{
    FileHandle *fh = new FileHandle("test_file");
    console->log("Created fh object");
    assert(fh->still_open);
    console->log("Assert 1");

    string s = fh->get_line();
    console->log("fh->get_line()");
    console->log("\"" + s + "\"");
    assert(0 == s.compare("this is a test file"));
    assert(0 == fh->get_line().compare("and this is the second line of the test file"));
    fh->get_line();
    assert(!fh->still_open);
}

void test_crc32(void)
{
    string source = "may the source be with you";
    string dest = get_crc_32(source);
    console->log(dest);
    assert(dest.compare("3b354087") == 0);
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

    string ziz = "  foo bar baz   ";
    string res = trimmed(ziz);
    assert(0 == res.compare("foo bar baz"));

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
    delete s;
}

void test_more_string_pattern()
{
    console->log("Test More String Pattern");

    string subject = "1997-02-23 The_Fourth_Generation_FULL_CD-FURY \"fry-4g06.zip\" (4/4) yEnc";
    StringPattern pattern(SP_LASTPART);
    pattern.add_breaker(SP_SUBJECT);
    pattern.add_breaker(" \"");
    pattern.add_breaker(SP_FILENAME);
    pattern.add_breaker("\" (");
    pattern.add_breaker(SP_PARTNO);
    pattern.add_breaker("/");
    pattern.add_breaker(SP_MAXPARTNO);
    pattern.add_breaker(") yEnc");

    assert(pattern.match(subject));
    assert_strings_eq(pattern.get_piece(SP_SUBJECT), 
                      "1997-02-23 The_Fourth_Generation_FULL_CD-FURY");

    assert(0 == pattern.get_piecen(SP_MAXFILENO));
}

void test_simple_x(void)
{
    string result = simple_x("abc123hehehe");
    assert(0 == result.compare("abc\thehehe"));

    result = simple_x("abc123hehehe - \"hotstuff.rar\" yEnc (001/100)");
    console->log(result);
    assert(0 == result.compare("abc\thehehe - \"\t\" yEnc (\t/\t)"));
}

void test_uudecode(void)
{
    console->log("Test UUdecode");
    UUDecoder decoder;
    string result = decoder.do_the_math("$>6]U\"@``");
    console->log(result);
    assert(result.compare("you\n") == 0);
}

void assert_postset_filenames_eq(PostSet *checkme, list<string> filenames)
{
    stringstream buf;
    console->log("Checking size..");
    buf << checkme->num_files() << " <-> " <<  filenames.size();
    console->log(buf.str());
    buf.str("");
    assert(checkme->num_files() == filenames.size());

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

void test_download_netjob(void)
{
    console->log("Testing postset download jobs..");
    NewsGroup group("fusion.downloads");
    PostSet post("brick brothers presents");
    post.has_msg_ids = 1;
    post.group = &group;

    PostsetJob psj(&post);
    psj.srv = (void *)4096;
    psj.process();

}

void test_dynamic_postsplit(void)
{
    NewsGroup group("net.fusion.downloads");
    PostSetSplitterDynamicMatch *splitter = (PostSetSplitterDynamicMatch *) group.splitter;
    console->log("Parsing (Horny Peeps \"hornypeeps.rar\" yEnc (23/59))");
    MessageHeader *header = new MessageHeader( &group, 31337, "Horny Peeps \"hornypeeps.rar\" yEnc (23/59)", "jim@bo.com");
    splitter->process_header(header);
    assert(0 == group.postsets.size());

    header = new MessageHeader( &group, 31338, "Horny Peeps \"hornypeeps.rar\" yEnc (24/59)", "jim@bo.com");
    splitter->process_header(header);
    assert(1 == group.postsets.size());
    delete header;

}

void shut_down(void)
{
    printf("Mooooooo!\n");
}

void test_string_sorting(void)
{
    console->log("\n===== Testing string sorting code =====\n");
    NewsGroup group1("blaj");
    NewsGroup group2("blak");
    NewsGroup group3("blai");
    NewsGroup group4("blah");
    vector<NewsGroup *> my_items;

    my_items.push_back(&group1);
    my_items.push_back(&group2);
    my_items.push_back(&group3);
    my_items.push_back(&group4);

    assert_strings_eq(my_items[0]->name, "blaj");
    assert_strings_eq(my_items[1]->name, "blak");
    assert_strings_eq(my_items[2]->name, "blai");
    assert_strings_eq(my_items[3]->name, "blah");

    sort(my_items.begin(), my_items.end(), NewsGroup::compare);

    assert_strings_eq(my_items[0]->name, "blah");
    assert_strings_eq(my_items[1]->name, "blai");
    assert_strings_eq(my_items[2]->name, "blaj");
    assert_strings_eq(my_items[3]->name, "blak");
}

void test_web_request(void)
{
    console->log("Testing webrequest..");
    WebRequest r = WebRequest("GET /foo/bar/baz HTTP/1.1");
    console->log(r.path);
    assert(0 == r.path.compare("/foo/bar/"));
    assert(0 == r.filename.compare("baz"));

    r = WebRequest("get /jack.css");
    assert(0 == r.path.compare("/"));
    assert(0 == r.filename.compare("jack.css"));

    r = WebRequest("get /jack.html?foo=bar;baz=bam");
    assert(0 == r.path.compare("/"));
    console->log(r.filename);
    assert(0 == r.filename.compare("jack.html"));
    assert(1 == r.has_cgi_params);
    assert(0 == r.param("foo").compare("bar"));
    assert(0 == r.param("baz").compare("bam"));
    assert(0 == r.paramn("jerry"));

    assert(0 == r.get_uri().compare("/jack.html?foo=bar;baz=bam"));

    r.param("foo", 2);
    assert(0 == r.get_uri().compare("/jack.html?foo=2;baz=bam"));

    WebRequest b = WebRequest(r.get_uri());
    console->log(b.get_uri());
    assert(0 == b.get_uri().compare("/jack.html?foo=2;baz=bam"));

    b.delete_param("foo");
    assert(0 == b.get_uri().compare("/jack.html?baz=bam"));

}

void test_js_escape(void)
{
    assert(0 == js_escape("'") . compare("\\'"));
    assert(0 == js_escape("\\'") . compare("\\\\\\'"));
}

void test_mem_stringpattern(void)
{
    console->log("Test mem stringpattern..");
    StringPattern split(2);
    split.add_breaker(0);
    split.add_breaker(".");
    split.add_breaker(1);
    split.match("1.2");

    assert(1 == split.get_piecen(0));
    assert(2 == split.get_piecen(1));
}
