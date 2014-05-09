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
    result.clear();
    parsePage(content);
}
bool SpiderFor163::searchNodeCallback(GumboNode *node, int &level)
{
    if (level == 0) {
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

                    level = 1;
                }
            }
        }
    } else if (level == 1) {
        GumboAttribute *href;
        if (node->v.element.tag == GUMBO_TAG_A &&
                // node->parent->v.element.tag == GUMBO_TAG_LI &&
                (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
            storedNodes.push_back(node);
            return false;
        }
    }
    return true;
}
void SpiderFor163::resultCallback(GumboNode *node)
{
    CrawlingResultItem out;
    string title = childrenToText(node);
    if (title.size() > 9) {
        out.title = title.c_str();
        out.url = gumbo_get_attribute(&node->v.element.attributes, "href")->value;
        out.date = time(0);
        result.push_back(out);
    }
}
