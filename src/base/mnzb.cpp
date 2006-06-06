#include "mnzb.hpp"
#include "strutil.hpp"
#include "config.hpp"
#include "post_file.hpp"
#include "file_handle.hpp"
#include "xmlparser.hpp"

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

    string doc = head->as_text("");
    string dest_dir = config->blackbeard_data_dir + "/" + set->group->name;
    m_mkdir(dest_dir);
    FileHandle *handle = open_filehandle(dest_dir + "/" + nzb_filename());
    handle->write_x_bytes_at(doc.size(), 0, doc.c_str());
    handle->close();
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

void mNZB::load_postset(PostSet *set)
{
    this->set = set;
    string dest_dir = config->blackbeard_data_dir + "/" + set->group->name;
    string full_filename = dest_dir + "/" + nzb_filename();

    FileHandle *nzb_handle = open_filehandle(full_filename);
    string xmlfile;
    while(nzb_handle->still_open) {
        xmlfile += nzb_handle->get_line();
    }

    XMLNode *parsed = parse_xml_doc(xmlfile);
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
    string message_id = file_node->content;

    PostFile *file = new PostFile(set);
    file->filename = file_node->get_attr("subject");

    // If we are restoring, there are no pieces or anything already
    // so we can just slam info in there
    
    vector<XMLNode *>::iterator i;
    for(i = pieces.begin(); i!=pieces.end(); ++i){
        XMLNode *node = *i;
        FilePiece *new_piece = new FilePiece(node->get_attr_num("article_no"), 
                                             node->content, 
                                             (PIECE_STATUS)node->get_attr_num("status"), 
                                             file, 
                                             node->get_attr_num("num_bytes"));
        file->pieces.push_back(new_piece);
    }
    file->_num_file_pieces = file->pieces.size();
}


