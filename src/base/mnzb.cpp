#include "mnzb.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "post_file.hpp"
#include "file_handle.hpp"
#include "xmlparser.hpp"
#include "console.hpp"

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

    head->write_to_file(dest_dir + "/" + nzb_filename());
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

    Uint32 max_piece_no = file->pieces.size();
    Uint32 number = 1;
    for(Uint32 piece_index=0; piece_index<max_piece_no; ++piece_index) {
        XMLNode *segment = new XMLNode("segment");
        FilePiece *piece = file->pieces[piece_index];
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

void mNZB::load_postset(PostSet *set)
{
    set->has_pieces_loaded = 1;
    this->set = set;
    string dest_dir = config->blackbeard_data_dir + "/" + set->group->name;
    string full_filename = dest_dir + "/" + nzb_filename();

    console->log("full filename is " + full_filename);

    if(!_file_exists(full_filename)) {
        console->log("Ouch! " + full_filename + " does not exist - could NOT load NZB");
        set->has_pieces_loaded = 1;
        return;
    }

    XMLNode *parsed = parse_xml_file(full_filename);
    vector<XMLNode *> files;

    parsed->find_for_tag_name(files, "file");
    vector<XMLNode *>::iterator i;
    
    for(i = files.begin(); i != files.end(); ++i) {
        restore_file(set, *i);
    }

}


void mNZB::restore_file(PostSet *set, XMLNode *file_node)
{

    vector<XMLNode *> pieces;
    file_node->find_for_tag_name(pieces, "segment");

    // If we are restoring, there are no pieces or anything already
    // so we can just slam info in there
    PostFile *file = set->file(file_node->get_attr("subject"));
    console->log("Restoring file " + file->filename);
    Uint32 num_pieces = 0;

    Uint32 max_piece_no = pieces.size();
    for(Uint32 i = 0; i<max_piece_no; ++i){
        num_pieces++;
        XMLNode *node = pieces[i];
        Uint32 article_num = node->get_attr_num("article_no");
        Uint32 num_bytes = node->get_attr_num("bytes");
        string message_id = node->content;
        FilePiece *piece = file->saw_message_id(article_num, message_id , num_bytes);
        piece->status = (PIECE_STATUS)node->get_attr_num("status");
        if(piece->status == 4) {
            file->_num_downloaded_pieces++;
        }
    }
    file->update_status_from_pieces();
    file->_num_file_pieces = file->pieces.size();
}


