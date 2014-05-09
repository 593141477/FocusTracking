#include "GumboBasedSpider.h"
#include "common.h"
#include <string>
#include <iostream>
#include <ctime>
using std::string;
using std::endl;
using std::cout;

void GumboBasedSpider::parsePage(const std::string &content)
{
    original = content.c_str();
    GumboOutput *output = gumbo_parse(original);
    storedNodes.clear();
    searchNode(output->root, 0);
    convertResult();
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    original = NULL;
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
        resultCallback(*it);
    }
}

std::string GumboBasedSpider::childrenToText(GumboNode *node)const
{
    if (node->type != GUMBO_NODE_ELEMENT) {
        return (node->type == GUMBO_NODE_TEXT) ? node->v.text.text : string();
    }

    string txt;
    GumboVector *children = &node->v.element.children;
    for (int i = 0; i < children->length; ++i) {
        txt += childrenToText(static_cast<GumboNode *>(children->data[i]));
    }

    return txt;
}
