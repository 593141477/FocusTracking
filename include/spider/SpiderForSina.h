#ifndef SPIDER_SINA_H__
#define SPIDER_SINA_H__

#include "common.h"
#include "CrawlingResult.h"
#include "GumboBasedSpider.h"
#include <vector>

class SpiderForSina : public GumboBasedSpider
{
	void doCrawling();
    bool searchNodeCallback(GumboNode *node, int &level);
    bool convertingCallback(GumboNode* node, CrawlingResultItem &out);
};
#endif