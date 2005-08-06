noinst_PROGRAMS = blb bt

AM_CXXFLAGS = -Wall -D_REENTRANT -Ibase -Iscreens -Ijobs -Idecoders

bt_SOURCES = blackbeardt.cpp
blb_SOURCES = blackbeard.cpp

bt_LDADD = screens/libscreens.a base/libbase.a decoders/libdecoder.a jobs/libjobs.a -lncurses 
blb_LDADD = screens/libscreens.a base/libbase.a decoders/libdecoder.a jobs/libjobs.a -lncurses 

SUBDIRS = base jobs screens decoders
