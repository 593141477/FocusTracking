#include "SpiderForQQRoll.h"
#include "common.h"
#include <string>
#include <iostream>
#include <ctime>
using std::string;
using std::endl;
using std::cout;

void SpiderForQQRoll::doCrawling()
{
    string content = downloadWebPage("http://roll.news.qq.com/interface/roll.php?site=news&date=&page=1&mode=1&of=json");
    result.clear();
    auto html = (string)parseJSON(content)["data"]["article_info"];
    parsePage(html);
}

bool SpiderForQQRoll::searchNodeCallback(GumboNode *node, int &level)
{
    if (node->v.element.tag == GUMBO_TAG_LI) {
        storedNodes.push_back(node);
        return false;
    }
    return true;
}
void SpiderForQQRoll::resultCallback(GumboNode *node)
{
    GumboVector *children = &node->v.element.children;
    CrawlingResultItem out;
    for (int i = 0; i < children->length; ++i) {
        GumboAttribute *cls;
        GumboNode *child = static_cast<GumboNode *>(children->data[i]);
        if (child->type != GUMBO_NODE_ELEMENT)
            continue;
        switch (child->v.element.tag) {
        case GUMBO_TAG_A:
            out.url = gumbo_get_attribute(&child->v.element.attributes, "href")->value;
            out.title = childrenToText(child);
            break;
        case GUMBO_TAG_SPAN:
            if((cls = gumbo_get_attribute(&child->v.element.attributes, "class"))
                && 0 == strcmp(cls->value, "t-time")){

                time_t _time = time(0);
                tm *t = localtime(&_time);
                if(t){
                    int mon, d, h, min;
                    sscanf(childrenToText(child).c_str(),
                        "%d-%d%d:%d", &mon, &d, &h, &min);
                    t->tm_mon = mon - 1;
                    t->tm_mday = d;
                    t->tm_hour = h;
                    t->tm_min = min;
                    t->tm_sec = 0;
                    out.date = mktime(t);
                }
            }
            break;
        default:
            break;
        }
    }
    result.push_back(out);
}