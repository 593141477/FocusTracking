#ifndef SPIDER_QQ_ROLL_H__
#define SPIDER_QQ_ROLL_H__

#include "common.h"
#include "CrawlingResult.h"
#include "GumboBasedSpider.h"
#include "JsonBasedSpider.h"
#include <vector>

class SpiderForQQRoll : public JsonBasedSpider,public GumboBasedSpider,public SpiderBase
{
	void doCrawling();
    bool searchNodeCallback(GumboNode *node, int &level);
    void resultCallback(GumboNode *node);
};
#endif