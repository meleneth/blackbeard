#include "mnzb.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "post_file.hpp"
#include "file_handle.hpp"
#include "xmlparser.hpp"
#include "console.hpp"
#include "md5_stuff.hpp"

#include <bzlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>

using std::stringstream;

mNZB::mNZB()
{
}

mNZB::~mNZB()
{
}

void mNZB::save_postset(PostSet *set)
{
    if(!set->dirty)
    return;
    this->set = set;
    XMLNode *head = new XMLNode("nzb");
    head->set_attr("xmlns", "http://www.newzbin.com/DTD/2003/nzb");
    
    vector<PostFile *>::iterator i;

    for (i = set->files.begin(); i != set->files.end(); ++i){
        if(*i){
            head->addChild(postfile_node(*i));
        }
    }

    string dest_dir = config->blackbeard_data_dir + "/" + set->group->name;
    m_mkdir(dest_dir);

    string filename = dest_dir + "/" + nzb_filename() + ".bz2";
    string doc = head->as_text("");

    FILE*   f;
    BZFILE* b;
    int     bzerror;

    f = fopen ( filename.c_str(), "wb" );
    if ( !f ) {
     /* handle error */
        console->log("bz2 write error!");
    }
    b = BZ2_bzWriteOpen( &bzerror, f, 5, 0, 30);
    if (bzerror != BZ_OK) {
     BZ2_bzWriteClose ( &bzerror, b, 0, 0, 0);
     /* handle error */
        console->log("bz2 write error!!");
    }

    /* get data to write into buf, and set nBuf appropriately */
    char *doc_str = (char *)doc.c_str();
    BZ2_bzWrite ( &bzerror, b, doc_str, doc.length() );
    if (bzerror == BZ_IO_ERROR) { 
      BZ2_bzWriteClose ( &bzerror, b, 0, 0, 0);
      /* handle error */
       console->log("bz2 write error!!");
    }

    BZ2_bzWriteClose( &bzerror, b, 0, 0, 0);
    if (bzerror == BZ_IO_ERROR) {
     /* handle error */
        console->log("bz2 write error!!!");
    }
    fclose(f);

    delete head;
}

string mNZB::nzb_filename()
{
    return get_crc_32(set->subject) + ".nzb";
}

XMLNode *mNZB::postfile_node(PostFile *file)
{
    XMLNode *node = new XMLNode("file");
    XMLNode *groups = new XMLNode("groups");
    XMLNode *group = new XMLNode("group");
    XMLNode *segments = new XMLNode("segments");

    group->content = file->post_set->group->name;
    groups->addChild(group);
    node->addChild(groups);
    node->addChild(segments);

    // NZB
    node->set_attr("poster", file->post_set->poster);
    // use strptime here
    // Required: FIXME node->set_attr("date", file->post_set->date);
    node->set_attr("subject", file->filename);
    // custom
    node->set_attr("status", file->status);
    node->set_attr("hash", hash_to_hex(file->hash));

    Uint32 number = 1;

    list<FilePiece *>::iterator p;    
    for(p = file->pieces.begin(); p!=file->pieces.end(); ++p) {
        XMLNode *segment = new XMLNode("segment");
        FilePiece *piece = *p;
        // NZB
        segment->set_attr("bytes", piece->num_bytes);
        segment->set_attr("number", number++);
        segment->content = piece->msg_id;
        // custom
        segment->set_attr("article_no", piece->article_no);
        segment->set_attr("status", piece->status);

        segments->addChild(segment);
    }

    return node;
}

Uint32 _file_exists(string filename)
{
    struct stat buf;
    return -1 != stat(filename.c_str(), &buf);
}

#define MNZB_BUFFER_SIZE 50000

void mNZB::load_postset(PostSet *set)
{
    set->has_pieces_loaded = 1;

    if(config->test_mode)
        return;

    this->set = set;
    string dest_dir = config->blackbeard_data_dir + "/" + set->group->name;
    string full_filename = dest_dir + "/" + nzb_filename() + ".bz2";

//    console->log("full filename is " + full_filename);

    if(!_file_exists(full_filename)) {
//        console->log("Ouch! " + full_filename + " does not exist - could NOT load NZB");
        set->has_pieces_loaded = 1;
        return;
    }
    string compressed_file_contents;

    char readbuf[MNZB_BUFFER_SIZE];

    FILE*   f;
    BZFILE* b;
    int     nBuf;
    int     bzerror;

    f = fopen ( full_filename.c_str(), "rb" );
    if ( !f ) {
      console->log("bzip2 read errror!");
      /* handle error */
    }
    b = BZ2_bzReadOpen ( &bzerror, f, 0, 0, NULL, 0);
    if ( bzerror != BZ_OK ) {
      BZ2_bzReadClose ( &bzerror, b );
      console->log("bzip2 read errror!");
      /* handle error */
    }

    bzerror = BZ_OK;
    int got_bytes = 0;
    while ( bzerror == BZ_OK ) {
      nBuf = BZ2_bzRead ( &bzerror, b, readbuf + got_bytes, MNZB_BUFFER_SIZE - got_bytes);
      got_bytes += nBuf;
      if ( (bzerror == BZ_OK) || (bzerror == BZ_STREAM_END) ) {
        /* do something with buf[0 .. nBuf-1] */
        int i=0;
        int last_string = 0;

        while(i<got_bytes){
            if(readbuf[i] == '\n') {
                readbuf[i] = 0;
                compressed_file_contents.append(readbuf + last_string);
                last_string = i + 1;
            }
            i++;
        }
        memmove(readbuf, readbuf + last_string, got_bytes - last_string);
        got_bytes -= last_string;
      }
    }
    readbuf[got_bytes] = 0;
    compressed_file_contents.append(readbuf);

    if ( bzerror != BZ_STREAM_END ) {
       BZ2_bzReadClose ( &bzerror, b );
      console->log("bzip2 read errror!!!");
       /* handle error */
    } else {
       BZ2_bzReadClose ( &bzerror, b );
    }
    fclose(f);
    
    XMLParser *parser = new XMLParser();
    parser->parse_fragment(compressed_file_contents);
    XMLNode *parsed = parser->document_node;
    delete parser;

    //XMLNode *parsed = parse_xml_file(full_filename);
    vector<XMLNode *> files;

    parsed->find_for_tag_name(files, "file");
    vector<XMLNode *>::iterator i;
    
    for(i = files.begin(); i != files.end(); ++i) {
        restore_file(set, *i);
    }

    delete parsed;
}


void mNZB::restore_file(PostSet *set, XMLNode *file_node)
{

    vector<XMLNode *> pieces;
    vector<XMLNode *>::iterator i;

    file_node->find_for_tag_name(pieces, "segment");

    // If we are restoring, there are no pieces or anything already
    // so we can just slam info in there
    PostFile *file = set->file(file_node->get_attr("subject"));
    hex_to_hash(file->hash, file_node->get_attr("hash"));
    //console->log("Restoring file " + file->filename);

    for(i = pieces.begin(); i!=pieces.end(); ++i){
        XMLNode *node = *i;
        Uint32 article_num = node->get_attr_num("article_no");
        Uint32 num_bytes = node->get_attr_num("bytes");
        string message_id = node->content;
        FilePiece *piece = file->saw_message_id(article_num, message_id, num_bytes);

        piece->status = (PIECE_STATUS)node->get_attr_num("status");
    }
    file->update_status_from_pieces();
}


