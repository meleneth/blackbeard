noinst_PROGRAMS = blb bt

AM_CXXFLAGS = -Wall -D_REENTRANT -Inetutils -Ibase -Iscreens -Ijobs -g

bt_SOURCES = blackbeardt.cpp
blb_SOURCES = blackbeard.cpp

bt_LDADD =  screens/libscreens.a jobs/libjobs.a base/libbase.a netutils/libnetutils.a -lncurses 
blb_LDADD = screens/libscreens.a jobs/libjobs.a base/libbase.a netutils/libnetutils.a -lncurses 

SUBDIRS = netutils base jobs screens 
