noinst_LIBRARIES = libengine.a
noinst_PROGRAMS = bt

libengine_a_SOURCES = newsgroup.cpp newsgroup.hpp config.cpp config.hpp console.cpp console.hpp newsgrouppost.cpp newsgrouppost.hpp tcpconnection.cpp tcpconnection.hpp netcentral.cpp netcentral.hpp news_constants.hpp nntpserver.cpp nntpserver.hpp thread.cpp thread.hpp netthread.cpp netthread.hpp
bt_SOURCES = blackbeardt.cpp libengine.a

AM_CXXFLAGS = -Wall
bt_LDADD=-L. -lengine -lncurses 
