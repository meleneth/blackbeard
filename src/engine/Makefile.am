noinst_LIBRARIES = libengine.a
noinst_PROGRAMS = bt

libengine_a_SOURCES = newsgroup.cpp newsgroup.hpp config.cpp config.hpp console.cpp console.hpp \
					  newsgrouppost.cpp newsgrouppost.hpp tcpconnection.cpp tcpconnection.hpp \
					  news_constants.hpp nntpserver.cpp nntpserver.hpp thread.cpp thread.hpp \
					  netthread.cpp netthread.hpp decoder.cpp decoder.hpp decoder_thread.cpp \
					  decoder_thread.hpp yenc_decoder.cpp yenc_decoder.hpp uu_decoder.cpp \
					  uu_decoder.hpp
bt_SOURCES = blackbeardt.cpp libengine.a

AM_CXXFLAGS = -Wall -D_REENTRANT
bt_LDADD=-L. -lengine -lncurses -lpthread
