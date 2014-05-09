#ifndef SPIDER_SINA_H__
#define SPIDER_SINA_H__

#include "common.h"
#include "CrawlingResult.h"
#include "GumboBasedSpider.h"
#include <vector>

class SpiderForSina : public GumboBasedSpider,public SpiderBase
{
	void doCrawling();
    bool searchNodeCallback(GumboNode *node, int &level);
    void resultCallback(GumboNode *node);
};
#endif