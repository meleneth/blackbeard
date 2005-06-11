noinst_LIBRARIES = libengine.a
noinst_PROGRAMS = bt

libengine_a_SOURCES = newsgroup.cpp newsgroup.hpp config.cpp config.hpp console.cpp console.hpp newsgrouppost.cpp newsgrouppost.hpp tcpconnection.hpp tcpconnection.cpp
bt_SOURCES = blackbeardt.cpp libengine.a

AM_CXXFLAGS = @SDL_CFLAGS@  -I../../libs/glpng
bt_LDADD=-L. -lengine -lncurses @SDL_LIBS@
