#ifndef SPIDER_BASE_H__
#define SPIDER_BASE_H__

#include "common.h"
#include "CrawlingResult.h"

enum SpiderState_t{
	SPIDER_FREE,
	SPIDER_WORKING,
	SPIDER_HAS_DATA
};

class SpiderBase
{
	SpiderState_t state;
	CrawlingResult result;
	virtual void doCrawling()=0;
public:
	SpiderBase();
	~SpiderBase();
	RetType StartCrawling();
	RetType StopCrawling();
	bool IsCrawlingResultAvailable();
	CrawlingResult GetCrawlingResult();
};
#endif