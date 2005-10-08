#include "shutdownscreen.hpp"

#ifdef __WIN32__
#include<curses.h>
#else
#include<ncurses.h>
#endif

ShutDownScreen::ShutDownScreen()
{
}

ShutDownScreen::~ShutDownScreen()
{
}

void ShutDownScreen::render(void)
{
    mvaddnstr(ypos+1, xpos+1, "Shutdown in progress please wait", -1);
}

