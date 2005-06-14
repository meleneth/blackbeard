#include"console.hpp"


Console *console;

int main(int argc, char *argv[])
{
    console = new Console();
    console->print_logs = 1;

    char buf[10] = "hello";
    std::string my_string = buf;
    
    buf[0] = 'x';

    console->log(buf);
    console->log(my_string);
	return 0;
}
