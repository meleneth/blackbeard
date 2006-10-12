#include "database.hpp"
#include "config.hpp"
#include "console.hpp"
#include "mnzb.hpp"
#include "xmlnode.hpp"
#include "xmlparser.hpp"
#include "strutil.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>

using std::stringstream;


void restore_newsgroups()
{
    // Parse XML file that has list of newsgroups
    XMLNode *document = parse_xml_file(config->newsgroups_xml_filename());
    vector<XMLNode *> groups;
    document->find_for_tag_name(groups, "newsgroup");

    Uint32 max_no = groups.size();
    for(Uint32 i=0; i<max_no; ++i) {
        NewsGroup *group = group_for_name(groups[i]->content);
        group->is_subscribed = 1;
        group->first_article_number = groups[i]->get_attr_num("min_article_no");
        group->last_article_number = groups[i]->get_attr_num("max_article_no");
        group->_num_postsets = groups[i]->get_attr_num("num_postsets");
    }

}

void restore_postsets(NewsGroup *group)
{
    XMLNode *document = parse_xml_file(config->newsgroup_postsets_filename() + "/" + group->name + ".xml");
    vector<XMLNode *> sets;
    document->find_for_tag_name(sets, "postset");

    Uint32 max_no = sets.size();
    for(Uint32 i=0; i<max_no; ++i) {
        XMLNode *setinfo = sets[i];
        PostSet *set = group->postset_for_subject(setinfo->content);
        set->_num_bytes = atol(setinfo->get_attr("num_bytes").c_str());
        set->_num_files = setinfo->get_attr_num("num_files");
        set->_min_article_no = setinfo->get_attr_num("min_article_no");
        set->_max_article_no = setinfo->get_attr_num("max_article_no");
        if(group->last_article_number < set->_max_article_no) {
            group->last_article_number = set->_max_article_no;
        }
    }
}

void restore_postfiles(PostSet *set)
{
    mNZB nzb;
    nzb.load_postset(set);
}
        
    
void save_postsets(NewsGroup *group)
{
    XMLNode *document = new XMLNode("postsets");

    Uint32 max_no = group->postsets.size();
    for(Uint32 i=0; i<max_no; i++){
        PostSet *set = group->postsets[i];
        if(set->min_article_no() > group->first_article_number) {
            XMLNode *setnode = new XMLNode("postset");
            setnode->content = set->subject;
            setnode->set_attr("min_article_no", set->min_article_no());
            setnode->set_attr("max_article_no", set->max_article_no());
            setnode->set_attr("num_files", set->num_files());
            setnode->set_attr("num_bytes", set->num_bytes());

            document->addChild(setnode);
            if(set->has_pieces_loaded) {
                mNZB nzb;
                nzb.save_postset(set);
            }
        } else {
            remove_postset_file(set);
        }
    }

    document->write_to_file(config->newsgroup_postsets_filename() + "/" + group->name + ".xml");
    delete document;
}

void save_subscribed_groups()
{
    XMLNode *document = new XMLNode("newsgroups");

    Uint32 max_no = newsgroups.size();
    for(Uint32 i=0; i<max_no; i++){
        NewsGroup *group = newsgroups[i];
        if(group->is_subscribed){
            XMLNode *groupnode = new XMLNode("newsgroup");
            groupnode->content = group->name;
            groupnode->set_attr("min_article_no", group->first_article_number);
            groupnode->set_attr("max_article_no", group->last_article_number);
            groupnode->set_attr("num_postsets", group->num_postsets());
            document->addChild(groupnode);
            if(group->has_postsets_loaded){
                console->log("Saving postsets for group " + group->name);
                save_postsets(group);
            }
        }
    }
    console->log("Writing main newsgroups.xml...");
    document->write_to_file(config->newsgroups_xml_filename());
    delete document;
}

Uint32 db_file_exists(string filename)
{
    struct stat buf;
    return -1 != stat(filename.c_str(), &buf);
}

void remove_postset_file(PostSet *set)
{
    console->log("Expiring postset " + set->subject);
    string filename = config->blackbeard_data_dir + "/" + set->group->name + "/" + get_crc_32(set->subject) + ".nzb.bz2";
    
    unlink(filename.c_str());
}

