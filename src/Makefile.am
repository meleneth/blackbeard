noinst_PROGRAMS = blb

blb_SOURCES = blackbeard.cpp

AM_CXXFLAGS = -Iengine -Wall -D_REENTRANT

SUBDIRS = engine
LDADD = -Lengine -lengine -lncurses -lpthread
