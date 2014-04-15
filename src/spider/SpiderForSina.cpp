#include "SpiderForSina.h"
#include "common.h"
#include <string>
#include <iostream>
using std::string;
using std::u32string;

void SpiderForSina::doCrawling()
{
    string content = downloadWebPage("http://news.sina.com.cn/");
    parsePage(content);
}
bool SpiderForSina::searchNodeCallback(GumboNode *node, int &level)
{
    if (level == 0) {
        GumboAttribute *cls, *id;
        if (node->v.element.tag == GUMBO_TAG_DIV &&
                (id = gumbo_get_attribute(&node->v.element.attributes, "id")) &&
                strcmp(id->value, "blk_yw_01") == 0 ) {
                    level = 1;
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
bool SpiderForSina::convertingCallback(GumboNode* node, CrawlingResultItem &out)
{
    string title = childrenToText(node);
    u32string tmp = Utility::utf8string_to_u32(title.c_str());
    if(tmp.length() > 5){
        out.title = tmp;
        out.url = gumbo_get_attribute(&node->v.element.attributes, "href")->value;
        return true;
    }
    return false;
}
