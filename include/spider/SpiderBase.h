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
    bool extractCharsetFromHeader(const char *field, std::string &outCharset) const;
    std::pair<std::string,std::string> httpGet(std::string url) const;
    std::string charsetConv(std::string source, std::string charset) const;
protected:
    SpiderState_t state;
    CrawlingResult result;
    virtual void doCrawling()=0;
    std::string downloadWebPage(std::string url) const;
public:
    SpiderBase();
    ~SpiderBase();
    RetType StartCrawling();
    RetType StopCrawling();
    bool IsCrawlingResultAvailable();
    CrawlingResult GetCrawlingResult();
};
#endif