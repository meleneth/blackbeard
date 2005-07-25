#include "shutdownscreen.hpp"

#include<ncurses.h>

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

