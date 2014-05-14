#ifndef SPIDER_163_ROLL_H__
#define SPIDER_163_ROLL_H__

#include "common.h"
#include "CrawlingResult.h"
#include "JsonBasedSpider.h"
#include <vector>

class SpiderFor163Roll : public JsonBasedSpider,public SpiderBase
{
	void doCrawling();
private:
	time_t datetime2time_t(const std::string &s)const;
};
#endif