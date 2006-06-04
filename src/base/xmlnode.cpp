#include "xmlnode.hpp"

#include<sstream>
using std::stringstream;

XMLNode::XMLNode(string name)
{
    this->name = name;
    content = "";
}

XMLNode::~XMLNode()
{
}

string XMLNode::get_attr(string id)
{
    int max_length = attr_name.size();
    for(int i = 0; i < max_length; ++i) {
        if(0 == attr_name[i].compare(id)) {
            return attr_value[i];
        }
    }
    return "";
}

XMLNode *XMLNode::set_attr(string id, string value)
{
    int max_length = attr_name.size();
    for(int i = 0; i < max_length; ++i) {
        if(0 == attr_name[i].compare(id)) {
            attr_value[i] = value;
            return this;
        }
    }
    attr_name.push_back(id);
    attr_value.push_back(value);
    return this;
}

XMLNode *XMLNode::addChild(XMLNode *node)
{
    children.push_back(node);
    return node;
}

string XMLNode::as_text(string prefix)
{
    stringstream result;
    result << prefix << start_tag();
    // children or content, not both FIXME?
    if(0 == children.size()) {
        result << content;
    } else {
        result << "\n";
        int max_length = children.size();
        for(int i = 0; i < max_length; ++i){
            result << (children[i])->as_text(prefix + "  ") << "\n";
        }
        result << prefix;
    }
    result << end_tag();
    return result.str();
}

string XMLNode::start_tag()
{
    stringstream result;
    result << "<" << name;

    int max_length = attr_name.size();
    for(int i = 0; i < max_length; ++i){
        result << " " << attr_name[i] << "=\"" << attr_value[i] << "\"";
    }

    result << ">";
    return result.str();
}

string XMLNode::end_tag()
{
    return "</" + name + ">";
}

void XMLNode::parse_fragment(string fragment)
{
    string blanks = " \t\n";
    string openangle = "<";
    string equals = "=";
    string quote = "\"";

    string::size_type pos = fragment.find_first_not_of(blanks, 0);
    while(string::npos != pos){
        // this is either the start of a block or the start of our content
        if(fragment[pos] == '<'){
        } else {
            string::size_type pos2 = fragment.find_first_of("<", pos);
            if(string::npos == pos2) {
                content = fragment;
            } else {
                content = fragment.substr(pos, pos2);
            }
        }
        pos = fragment.find_first_not_of(blanks, pos+1);
    }
}

XMLNode *parse_xml_doc(string content)
{
    XMLNode *document = new XMLNode("document");
    document->parse_fragment(content);
    return document;
}

void XMLNode::find_for_tag_name(vector<XMLNode *>& result, string tag_name)
{
}
