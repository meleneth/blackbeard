noinst_PROGRAMS = blb

blb_SOURCES = blackbeard.cpp engine/libengine.a

AM_CXXFLAGS = @SDL_CFLAGS@ -Iengine -Wall

SUBDIRS = engine
LDADD = -Lengine -lengine -lncurses -lpth @SDL_LIBS@
