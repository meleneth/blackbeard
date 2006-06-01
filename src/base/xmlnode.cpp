#include "xmlnode.hpp"

#include<sstream>
using std::stringstream;

XMLNode::XMLNode(string name)
{
    this->name = name;
    value = "";
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
        result << value;
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

XMLNode *parse_xml_doc(string filename)
{
}

void XMLNode::find_for_tag_name(vector<XMLNode *>& result, string tag_name)
{
}
