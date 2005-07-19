noinst_PROGRAMS = blb bt

AM_CXXFLAGS = -Wall -D_REENTRANT -Ibase -Ithreads -Iscreens -Ijobs -Idecoders

bt_SOURCES = blackbeardt.cpp
blb_SOURCES = blackbeard.cpp

bt_LDADD = screens/libscreens.a threads/libthreads.a  base/libbase.a decoders/libdecoder.a jobs/libjobs.a -lncurses -lpthread
blb_LDADD = screens/libscreens.a threads/libthreads.a  base/libbase.a decoders/libdecoder.a jobs/libjobs.a -lncurses -lpthread

SUBDIRS = base threads jobs screens decoders
