noinst_PROGRAMS = blb bt

AM_CXXFLAGS = -Wall -D_REENTRANT -Iutils -Inetutils -Ibase -Iscreens -Ijobs -Ipar2 -g

bt_SOURCES = blackbeardt.cpp
blb_SOURCES = blackbeard.cpp

bt_LDADD =  screens/libscreens.a jobs/libjobs.a base/libbase.a par2/libpar2.a netutils/libnetutils.a utils/libutils.a -lncurses -lsqlite3
blb_LDADD = screens/libscreens.a jobs/libjobs.a base/libbase.a par2/libpar2.a netutils/libnetutils.a utils/libutils.a -lncurses -lsqlite3

SUBDIRS = utils netutils par2 base jobs screens
