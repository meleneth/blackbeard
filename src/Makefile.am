noinst_PROGRAMS = blb

blb_SOURCES = blackbeard.cpp

AM_CXXFLAGS = @SDL_CFLAGS@ -Iengine

SUBDIRS = engine
LDADD = engine/libengine.a -lncurses @SDL_LIBS@
