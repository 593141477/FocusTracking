#include "GumboBasedSpider.h"
#include "common.h"
#include <string>
#include <iostream>
using std::string;
using std::endl;
using std::cout;

void GumboBasedSpider::parsePage(const std::string &content)
{
    original = content.c_str();
    GumboOutput *output = gumbo_parse(original);
    storedNodes.clear();
    result.clear();
    searchNode(output->root, 0);
    convertResult();
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    original = NULL;
    
    for (std::vector<CrawlingResultItem>::iterator i = result.begin(); i != result.end(); ++i) {
        std::cout << i->url << endl;
        std::cout << Utility::u32string_to_utf8( i->title) << endl << endl;
    }
}

void GumboBasedSpider::searchNode(GumboNode *node, int level)
{
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    if (!searchNodeCallback(node, level))
        return;

    GumboVector *children = &node->v.element.children;
    for (int i = 0; i < children->length; ++i) {
        searchNode(static_cast<GumboNode *>(children->data[i]), level);
    }

}

void GumboBasedSpider::convertResult()
{
    std::vector<GumboNode *>::iterator it;
    for (it = storedNodes.begin(); it != storedNodes.end(); ++it) {
        CrawlingResultItem item;
        if (convertingCallback(*it, item)) {
            result.push_back(item);
        }
    }
}

std::string GumboBasedSpider::childrenToText(GumboNode *node)const
{
    const GumboElement &element = node->v.element;
    if (!element.children.length)
        return string();
    const GumboNode *ch = static_cast<const GumboNode *>(element.children.data[0]);
    if (ch->type == GUMBO_NODE_TEXT)
        return ch->v.text.text;
    else
        return string(original + ch->v.element.start_pos.offset, element.end_pos.offset - ch->v.element.start_pos.offset);
}
