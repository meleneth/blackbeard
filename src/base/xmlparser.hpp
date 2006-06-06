#ifndef ___xmlparser_inc
#define ___xmlparser_inc 1

#include<vector>

using std::vector;

#include "SDL_types.h"
#include "xmlnode.hpp"

class XMLParser {
    public:
        XMLParser();
        ~XMLParser();

        void parse_fragment(string fragment);
        void handle_start_tag(string tag);
        void handle_end_tag(string tag);
        void handle_content(string content);

        XMLNode *document_node;
        XMLNode *current_node;

        vector<XMLNode *> node_stack;
    private:
    protected:
};

XMLNode *parse_xml_doc(string filename);

#endif
