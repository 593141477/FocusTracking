#include "SpiderFor163.h"
#include "common.h"
#include <string>
#include <iostream>
using std::string;
using std::endl;
using std::cout;

void SpiderFor163::doCrawling()
{
    string content = downloadWebPage("http://news.163.com/");
    original = content.c_str();
    GumboOutput *output = gumbo_parse(original);
    links.clear();
    searchForModClasses(output->root);
    result.clear();
    filterLinks();
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    original = NULL;

    for (std::vector<CrawlingResultItem>::iterator i = result.begin(); i != result.end(); ++i)
    {
        std::cout << i->url << endl;
        std::cout << Utility::u32string_to_utf8( i->title) << endl << endl;
    }
}
void SpiderFor163::filterLinks()
{
    std::vector<GumboNode*>::iterator it;
    for (it = links.begin(); it != links.end(); ++it)
    {
        string title;
        const GumboElement &element = (*it)->v.element;
        if(!element.children.length)
            continue;
        const GumboNode *ch = static_cast<const GumboNode *>(element.children.data[0]);
        if(ch->type == GUMBO_NODE_TEXT)
            title = ch->v.text.text;
        else
            title = string(original + ch->v.element.start_pos.offset, element.end_pos.offset - ch->v.element.start_pos.offset);
        if(title.size() > 9){
            CrawlingResultItem item;
            item.title = Utility::utf8string_to_u32(title.c_str());
            item.url = gumbo_get_attribute(&element.attributes, "href")->value;
            result.push_back(item);
        }
    }
}

void SpiderFor163::searchForArticleLinks(GumboNode *node)
{
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }
    GumboAttribute *href;
    if (node->v.element.tag == GUMBO_TAG_A &&
            // node->parent->v.element.tag == GUMBO_TAG_LI &&
            (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
        links.push_back(node);
        return;
    }

    GumboVector *children = &node->v.element.children;
    for (int i = 0; i < children->length; ++i) {
        searchForArticleLinks(static_cast<GumboNode *>(children->data[i]));
    }

}

void SpiderFor163::searchForModClasses(GumboNode *node)
{
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboAttribute *cls, *id;
    if (node->v.element.tag == GUMBO_TAG_DIV &&
            (cls = gumbo_get_attribute(&node->v.element.attributes, "class")) &&
            cls ) {
        const std::vector<string> &values = Utility::split(string(cls->value), ' ');
        if (values.end() != std::find(values.begin(), values.end(), "mod")) {
            if (((id = gumbo_get_attribute(&node->v.element.attributes, "id")) && 0 == strcmp("news", id->value)) ||
                    values.end() != std::find(values.begin(), values.end(), "hot-news") ||
                    values.end() != std::find(values.begin(), values.end(), "domestic") ||
                    values.end() != std::find(values.begin(), values.end(), "international")
               ) {

                // cout << "*** Mod Found: " << cls->value << " ***\n" << endl;
                searchForArticleLinks(node);
                return;
            }
        }
    }

    GumboVector *children = &node->v.element.children;
    for (int i = 0; i < children->length; ++i) {
        searchForModClasses(static_cast<GumboNode *>(children->data[i]));
    }
}
