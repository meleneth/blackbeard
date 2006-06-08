#include "xmlparser.hpp"
#include "console.hpp"
#include "file_handle.hpp"

XMLParser::XMLParser()
{
    document_node = new XMLNode("document");
    current_node = document_node;
}

XMLParser::~XMLParser()
{
}

void XMLParser::parse_fragment(string fragment)
{
    string blanks = " \t\n";
    string openangle = "<";
    string equals = "=";
    string quote = "\"";

    string::size_type pos = fragment.find_first_not_of(blanks, 0);
    while(string::npos != pos){
        // this is either the start of a block or the start of our content
        if(fragment[pos] == '<'){
            // tag starts here
            string::size_type end_tag_pos = fragment.find_first_of(">", pos);
            if(end_tag_pos != string::npos) {
                if(fragment[pos+1] == '/') {
                    handle_end_tag(fragment.substr(pos + 2, end_tag_pos - pos - 2));
                } else {
                    handle_start_tag(fragment.substr(pos + 1, end_tag_pos - pos - 1));
                }
                pos = end_tag_pos + 1;
                if(pos > fragment.length()) 
                    pos = string::npos;
            } 
        } else {
            // content starts here - next non content is '<'
            string::size_type pos2 = fragment.find_first_of("<", pos);
            if(string::npos == pos2) {
                handle_content(fragment.substr(pos, fragment.length() - pos));
                pos = string::npos;
            } else {
                handle_content(fragment.substr(pos, pos2 - pos));
                pos = pos2;
            }
        }

        if(pos != string::npos) {
            pos = fragment.find_first_not_of(blanks, pos);
        }
    }
}

void XMLParser::handle_start_tag(string tag)
{
    console->log("handle_start_tag(" + tag + ")");

    XMLNode *new_node;

    string::size_type pos = tag.find_first_of(" ");
    if(pos != string::npos) {
        string attrs = tag.substr(pos, tag.length() - pos);
        tag = tag.substr(0, pos);
        new_node = new XMLNode(tag);

        console->log("Tag  : " + tag);
        console->log("Attrs: " + attrs);
        
        pos = attrs.find_first_not_of(" ", 0);
        while(pos != string::npos) { 
            string::size_type pos2 = attrs.find_first_of(" =", pos);
            string attr_name = attrs.substr(pos, pos2 - pos);
            console->log("Attr name: " + attr_name);

            pos = attrs.find_first_of("\"", pos2);
            pos2 = attrs.find_first_of("\"", pos + 1);
            string attr_value = attrs.substr(pos + 1, pos2 - pos - 1);
            console->log("Attr value: " + attr_value);

            new_node->set_attr(attr_name, attr_value);

            pos = attrs.find_first_not_of(" ", pos2 + 1);
        }
        console->log("We should be ready for PHUNKY SHIT");
    } else { 
        console->log("Got the else :/");
        new_node = new XMLNode(tag);
    }

    console->log("Pushing node..");
    node_stack.push_back(new_node);
    current_node->addChild(new_node);
    current_node = new_node;
    console->log("Done with this node");
}

void XMLParser::handle_end_tag(string tag)
{
    console->log("handle_end_tag(" + tag + ")");
    vector<XMLNode *>::iterator i = node_stack.end();
    if(i != node_stack.begin()){
        i--;
        current_node = *i;
        node_stack.erase(i);
        return;
    }
    current_node = NULL;
}

void XMLParser::handle_content(string content)
{
    console->log("handle_content(" + content + ")");
    current_node->content = content;
}

XMLNode *parse_xml_doc(string content)
{
    XMLParser *parser = new XMLParser();
    parser->parse_fragment(content);
    XMLNode *head = parser->document_node;
    delete parser;
    return head;
}


XMLNode *parse_xml_file(string filename)
{
    FileHandle *xml_handle = open_filehandle(filename);
    string xmlfile;
    while(xml_handle->still_open) {
        string z = xml_handle->get_line();
        console->log("READ FROM FILE: " + z);
        xmlfile += z;
    }
    xml_handle->close();
    close_finished_files();
    XMLParser *parser = new XMLParser();
    parser->parse_fragment(xmlfile);
    XMLNode *head = parser->document_node;
    delete parser;
    return head;
}


