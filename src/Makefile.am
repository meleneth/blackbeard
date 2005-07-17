noinst_LIBRARIES = libengine.a
noinst_PROGRAMS = blb bt

AM_CXXFLAGS = -Wall -D_REENTRANT

libengine_a_SOURCES = newsgroup.cpp newsgroup.hpp config.cpp config.hpp console.cpp console.hpp \
					  newsgrouppost.cpp newsgrouppost.hpp tcpconnection.cpp tcpconnection.hpp \
					  news_constants.hpp nntpserver.cpp nntpserver.hpp thread.cpp thread.hpp \
					  net_thread.cpp net_thread.hpp decoder.cpp decoder.hpp decoder_thread.cpp \
					  decoder_thread.hpp yenc_decoder.cpp yenc_decoder.hpp uu_decoder.cpp \
					  uu_decoder.hpp strutil.hpp strutil.cpp stringpattern.cpp stringpattern.hpp \
					  post_set.cpp post_set.hpp post_file.cpp post_file.hpp job.hpp job.cpp \
					  jobqueue.hpp jobqueue.cpp input_thread.cpp input_thread.hpp widget.cpp \
					  widget.hpp screen.cpp screen.hpp postsetlistscreen.cpp postsetlistscreen.hpp \
					  postsetdetailscreen.hpp postsetdetailscreen.cpp SDL_types.h globals.hpp \
					  keydefs.hpp newsgrouplistscreen.cpp newsgrouplistscreen.hpp

bt_SOURCES = blackbeardt.cpp libengine.a
blb_SOURCES = blackbeard.cpp libengine.a

bt_LDADD=-L. -lengine -lncurses -lpthread
blb_LDADD =-L. -lengine -lncurses -lpthread
