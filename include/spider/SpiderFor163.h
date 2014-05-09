#ifndef SPIDER_163_H__
#define SPIDER_163_H__

#include "common.h"
#include "CrawlingResult.h"
#include "GumboBasedSpider.h"
#include <vector>

class SpiderFor163 : public GumboBasedSpider,public SpiderBase
{
	void doCrawling();
    bool searchNodeCallback(GumboNode *node, int &level);
    void resultCallback(GumboNode *node);
};
#endif