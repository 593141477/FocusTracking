#include "SpiderForQQ.h"
#include "common.h"
#include <string>
#include <iostream>
using std::string;
using std::endl;
using std::cout;

void SpiderForQQ::doCrawling()
{
    string content = downloadWebPage("http://news.qq.com/china_index.shtml");
    parsePage(content);

    for (std::vector<CrawlingResultItem>::iterator i = result.begin(); i != result.end(); ++i) {
        std::cout << i->url << endl;
        std::cout << Utility::u32string_to_utf8( i->title) << endl << endl;
    }
}
bool SpiderForQQ::searchNodeCallback(GumboNode *node, int &level)
{
    if (level == 0) {
        GumboAttribute *cls, *id;
        if (node->v.element.tag == GUMBO_TAG_A &&
                gumbo_get_attribute(&node->v.element.attributes, "href") &&
                (cls = gumbo_get_attribute(&node->v.element.attributes, "class")) ) {
            const std::vector<string> &values = Utility::split(string(cls->value), ' ');
            if (values.end() != std::find(values.begin(), values.end(), "linkto")) {
                storedNodes.push_back(node);
                return false;
            }
        }
    }
    return true;
}
bool SpiderForQQ::convertingCallback(GumboNode* node, CrawlingResultItem &out)
{
    string title = childrenToText(node);
    out.title = Utility::utf8string_to_u32(title.c_str());
    out.url = gumbo_get_attribute(&node->v.element.attributes, "href")->value;
    return true;
}
