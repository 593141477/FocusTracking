#ifndef SPIDER_163_H__
#define SPIDER_163_H__

#include "common.h"
#include "CrawlingResult.h"
#include "SpiderBase.h"
#include "gumbo.h"
#include <vector>

class SpiderFor163 : public SpiderBase
{
    const char* original;
    std::vector<GumboNode*> links;
    void searchForModClasses(GumboNode *node);
    void searchForArticleLinks(GumboNode *node);
    void filterLinks();
    virtual void doCrawling();

};
#endif