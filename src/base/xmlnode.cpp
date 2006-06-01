#include "xmlnode.hpp"

XMLNode::XMLNode(string name)
{
    this->name = name;
}

XMLNode::~XMLNode()
{
}

string XMLNode::get_attr(string id)
{
}

XMLNode *XMLNode::set_attr(string id, string value)
{
    return this;
}

XMLNode *XMLNode::addChild(XMLNode *node)
{
    return node;
}

string XMLNode::as_text(string prefix)
{
    return "";
}

XMLNode *parse_xml_doc(string filename)
{
}

void XMLNode::find_for_tag_name(vector<XMLNode *>& result, string tag_name)
{
}
