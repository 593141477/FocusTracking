#include "SpiderBase.h"

SpiderBase::SpiderBase()
{
	state = SPIDER_FREE;
}
SpiderBase::~SpiderBase()
{

}

RetType SpiderBase::StartCrawling()
{
	doCrawling();
	return RET_OK;
}
RetType SpiderBase::StopCrawling()
{
	return RET_OK;
}
bool SpiderBase::IsCrawlingResultAvailable()
{
	return state == SPIDER_HAS_DATA;
}
CrawlingResult SpiderBase::GetCrawlingResult()
{
	if(!IsCrawlingResultAvailable())
		return CrawlingResult();

	return result;
}
