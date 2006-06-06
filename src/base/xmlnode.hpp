#ifndef ___xmlnode_inc
#define ___xmlnode_inc 1

#include "SDL_types.h"
#include <vector>
#include<string>
using std::vector;
using std::string;

class XMLNode {
    public:
        XMLNode(string name);
        ~XMLNode();
        
        string name;
        string content;
        vector<string> attr_name;
        vector<string> attr_value;
        vector<XMLNode *> children;

        string get_attr(string id);
        XMLNode *set_attr(string id, string value);
        XMLNode *addChild(XMLNode *node);
        string start_tag();
        string end_tag();
        string as_text(string prefix);
        void find_for_tag_name(vector<XMLNode *>& result, string tag_name);
    private:
    protected:
};


#endif
