#include "SpiderForQQ.h"
#include "common.h"
#include <string>
#include <iostream>
using std::string;
using std::endl;
using std::cout;

void SpiderForQQ::doCrawling()
{
    host_url = "http://news.qq.com";

    string content = downloadWebPage(host_url + "/china_index.shtml");
    parsePage(content);
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
    out.title = title;
    out.url = gumbo_get_attribute(&node->v.element.attributes, "href")->value;
    if(out.url.size() && out.url[0] == '/')
        out.url = host_url + out.url;
    return true;
}
