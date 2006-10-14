noinst_LIBRARIES = libpar2.a
noinst_PROGRAMS = p2

libpar2_a_SOURCES = pararchive.cpp pararchive.hpp parpacket.cpp parpacket.hpp
p2_SOURCES = libpar2.a p2.cpp

AM_CXXFLAGS = -Wall -D_REENTRANT -I../utils
p2_LDADD = ../utils/console.o ../utils/file_handle.o libpar2.a -lncurses -lcrypto

