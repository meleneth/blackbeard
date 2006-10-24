noinst_PROGRAMS = blb bt

AM_CXXFLAGS = -Wall -D_REENTRANT -Iutils -Inetutils -Ibase -Iscreens -Ijobs -Ipar2 -g -pg

bt_SOURCES = blackbeardt.cpp
blb_SOURCES = blackbeard.cpp

bt_LDADD =  screens/libscreens.a jobs/libjobs.a base/libbase.a par2/libpar2.a netutils/libnetutils.a utils/libutils.a -lncurses -lbz2 -lcrypto
blb_LDADD = screens/libscreens.a jobs/libjobs.a base/libbase.a par2/libpar2.a netutils/libnetutils.a utils/libutils.a -lncurses -lbz2 -lcrypto
bt_LDFLAGS = -pg
blb_LDFLAGS = -pg

SUBDIRS = utils netutils par2 base jobs screens
