#ifndef SPIDER_QQ_H__
#define SPIDER_QQ_H__

#include "common.h"
#include "CrawlingResult.h"
#include "GumboBasedSpider.h"
#include <vector>

class SpiderForQQ : public GumboBasedSpider
{
	std::string host_url;
	void doCrawling();
    bool searchNodeCallback(GumboNode *node, int &level);
    bool convertingCallback(GumboNode* node, CrawlingResultItem &out);
};
#endif