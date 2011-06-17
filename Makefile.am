EXTRA_DIST = reconf genclassfile fix-perms.sh

ACLOCAL_AMFLAGS = -I m4

noinst_PROGRAMS = src/blb src/bt src/par2/p2
noinst_LIBRARIES = src/libbase.a src/libjobs.a src/libnetutils.a src/libpar2.a src/libscreens.a src/libutils.a src/libpar2.a

AM_CXXFLAGS = -Wall -D_REENTRANT -Isrc/utils -Isrc/netutils -Isrc/base -Isrc/screens -Isrc/jobs -Isrc/par2 -g -pg $(OPENSSL_INCLUDES)

src_bt_SOURCES = src/blackbeardt.cpp
src_blb_SOURCES = src/blackbeard.cpp
src_par2_p2_SOURCES = src/par2/p2.cpp


src_bt_LDADD =  src/libscreens.a src/libjobs.a src/libbase.a src/libpar2.a src/libnetutils.a src/libutils.a -lncurses -lbz2 -lcrypto
src_blb_LDADD = src/libscreens.a src/libjobs.a src/libbase.a src/libpar2.a src/libnetutils.a src/libutils.a -lncurses -lbz2 -lcrypto
src_par2_p2_LDADD = src/utils/console.o src/utils/file_handle.o src/utils/md5_stuff.o src/libpar2.a -lncurses -lcrypto

src_bt_LDFLAGS = -pg
src_blb_LDFLAGS = -pg

src_libbase_a_SOURCES = src/base/config.cpp \
    src/base/news_constants.hpp \
    src/base/newsgrouppost.cpp \
    src/base/nntpserver.hpp \
    src/base/post_set.cpp \
    src/base/config.hpp \
    src/base/newsgroup.cpp \
    src/base/newsgrouppost.hpp \
    src/base/post_file.cpp \
    src/base/post_set.hpp \
    src/base/keydefs.hpp \
    src/base/newsgroup.hpp \
    src/base/nntpserver.cpp \
    src/base/post_file.hpp \
    src/base/messageheader.cpp \
    src/base/messageheader.hpp \
    src/base/post_set_splitter.cpp \
    src/base/post_set_splitter.hpp \
    src/base/post_set_splitter_dynamicmatch.cpp \
    src/base/post_set_splitter_dynamicmatch.hpp \
    src/base/database.cpp \
    src/base/database.hpp \
    src/base/filepiece.cpp \
    src/base/filepiece.hpp \
    src/base/mnzb.cpp \
    src/base/mnzb.hpp \
    src/base/xmlnode.cpp \
    src/base/xmlnode.hpp \
    src/base/xmlparser.cpp \
    src/base/xmlparser.hpp \
    src/base/post_set_splitter_filenamematch.cpp \
    src/base/post_set_splitter_filenamematch.hpp

src_libjobs_a_SOURCES = src/jobs/headersforgroupjob.cpp \
    src/jobs/headersforgroupjob.hpp \
    src/jobs/job.cpp \
    src/jobs/job.hpp \
    src/jobs/jobqueue.cpp \
    src/jobs/jobqueue.hpp \
    src/jobs/postsetjob.cpp \
    src/jobs/postsetjob.hpp \
    src/jobs/bodyretrieverjob.cpp \
    src/jobs/bodyretrieverjob.hpp \
    src/jobs/subjectfileloaderjob.cpp \
    src/jobs/subjectfileloaderjob.hpp \
    src/jobs/newsgrouplistretrieverjob.cpp \
    src/jobs/newsgrouplistretrieverjob.hpp \
    src/jobs/netjob.hpp \
    src/jobs/netjob.cpp \
    src/jobs/decoder.cpp \
    src/jobs/decoder.hpp \
    src/jobs/uu_decoder.cpp \
    src/jobs/uu_decoder.hpp \
    src/jobs/yenc_decoder.cpp \
    src/jobs/yenc_decoder.hpp \
    src/jobs/netcentral.cpp \
    src/jobs/netcentral.hpp \
    src/jobs/postfilejob.cpp \
    src/jobs/postfilejob.hpp \
    src/jobs/groupupdater.cpp \
    src/jobs/groupupdater.hpp \
    src/jobs/jobparent.cpp \
    src/jobs/jobparent.hpp \
    src/jobs/decoderfactory.cpp \
    src/jobs/decoderfactory.hpp \
    src/jobs/verifypostfilejob.hpp \
    src/jobs/verifypostfilejob.cpp

src_libnetutils_a_SOURCES = src/netutils/tcpconnection.cpp \
    src/netutils/tcpconnection.hpp \
    src/netutils/webserver.cpp \
    src/netutils/webserver.hpp \
    src/netutils/SDL_types.h \
    src/netutils/tcplistener.cpp \
    src/netutils/tcplistener.hpp \
    src/netutils/webrequest.cpp \
    src/netutils/webrequest.hpp \
    src/netutils/webdatafetcher.cpp \
    src/netutils/webdatafetcher.hpp \
    src/netutils/webfilefetcher.cpp \
    src/netutils/webfilefetcher.hpp \
    src/netutils/webjoblist.cpp \
    src/netutils/webjoblist.hpp \
    src/netutils/webnewsgroups.cpp \
    src/netutils/webnewsgroups.hpp \
    src/netutils/webpostsets.cpp \
    src/netutils/webpostsets.hpp \
    src/netutils/webpostfiles.cpp \
    src/netutils/webpostfiles.hpp \
    src/netutils/webviewfile.cpp \
    src/netutils/webviewfile.hpp \
    src/netutils/websearch.cpp \
    src/netutils/websearch.hpp \
    src/netutils/webdefines.hpp \
    src/netutils/webshared.cpp \
    src/netutils/webshared.hpp 

src_libpar2_a_SOURCES = src/par2/pararchive.cpp \
    src/par2/pararchive.hpp \
    src/par2/parpacket.cpp \
    src/par2/parpacket.hpp

src_libscreens_a_SOURCES = src/screens/newsgrouplistscreen.cpp \
    src/screens/postsetdetailscreen.cpp \
    src/screens/postsetlistscreen.cpp \
    src/screens/screen.cpp \
    src/screens/session.cpp \
    src/screens/widget.cpp \
    src/screens/newsgrouplistscreen.hpp \
    src/screens/postsetdetailscreen.hpp \
    src/screens/postsetlistscreen.hpp \
    src/screens/screen.hpp \
    src/screens/session.hpp \
    src/screens/widget.hpp \
    src/screens/scrollablelist.hpp \
    src/screens/shutdownscreen.cpp \
    src/screens/shutdownscreen.hpp \
    src/screens/activejobsscreen.cpp \
    src/screens/activejobsscreen.hpp \
    src/screens/activenetjobsscreen.cpp \
    src/screens/activenetjobsscreen.hpp \
    src/screens/textviewerscreen.cpp \
    src/screens/textviewerscreen.hpp \
    src/screens/downloadedpostsetsscreen.cpp \
    src/screens/downloadedpostsetsscreen.hpp

src_libutils_a_SOURCES = src/utils/console.cpp\
    src/utils/console.hpp\
    src/utils/SDL_types.h\
    src/utils/strutil.cpp\
    src/utils/strutil.hpp\
    src/utils/file_handle.cpp\
    src/utils/file_handle.hpp\
    src/utils/stringpattern.cpp\
    src/utils/stringpattern.hpp\
    src/utils/datetime.cpp\
    src/utils/datetime.hpp\
    src/utils/md5_stuff.cpp\
    src/utils/md5_stuff.hpp
