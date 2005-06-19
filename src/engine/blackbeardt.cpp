#include"console.hpp"
#include"newsgroup.hpp"

Console *console;

void test_header_scoop(void);

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;

    char buf[10] = "hello";
    std::string my_string = buf;
    
    buf[0] = 'x';

    console->log(buf);
    console->log(my_string);

    test_header_scoop();
	return 0;
}

void test_header_scoop(void)
{
    console->log("Test NewsGroup::header_scoop");
    NewsGroup *group = new NewsGroup("alt.binaries.erotica.tigger");

    group->header_scoop("2329    Star Wars Clone Wars - File 11 of 11: \"Clone Wars Chapter 20.mpg\" yEnc (256/258)        Homer P Wylde <brick@house.com> 18 Jun 2005 05:50:02 GMT<d90cma$9hk$2328@answer.sectorfour>             345632  2678    Xref: answer.sectorfour misc.test:2329 "); 
}
