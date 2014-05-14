#ifndef SPIDER_SINA_ROLL_H__
#define SPIDER_SINA_ROLL_H__

#include "common.h"
#include "CrawlingResult.h"
#include "JsonBasedSpider.h"
#include <vector>

class SpiderForSinaRoll : public JsonBasedSpider,public SpiderBase
{
	void doCrawling();
};
#endif