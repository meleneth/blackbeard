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

Uint32 XMLNode::get_attr_num(string id)
{
    string value = get_attr(id);
    return atoi(value.c_str());
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

XMLNode *XMLNode::set_attr(string id, Uint32 value)
{
    stringstream s;
    s << value;
    return set_attr(id, s.str());
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

void XMLNode::find_for_tag_name(vector<XMLNode *>& result, string tag_name)
{
    if(0 == name.compare(tag_name))
        result.push_back(this);

    for(vector<XMLNode *>::iterator i = children.begin(); i != children.end(); ++i) {
        (*i)->find_for_tag_name(result, tag_name);
    }
}
