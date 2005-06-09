noinst_LIBRARIES = libengine.a
noinst_PROGRAMS = bt

libengine_a_SOURCES = newsgroup.cpp newsgroup.hpp config.cpp config.hpp console.cpp console.hpp 
bt_SOURCES = blackbeardt.cpp libengine.a

AM_CXXFLAGS = @SDL_CFLAGS@  -I../../libs/glpng
bt_LDFLAGS = -L. -lengine

