#ifndef JSON_BASED_SPIDER_H__
#define JSON_BASED_SPIDER_H__

#include "common.h"
#include "CrawlingResult.h"
#include "SpiderBase.h"
#include "json.hh"
#include <vector>
#include <string>

class JsonBasedSpider
{
protected:
    std::string filterJSCode(const std::string &js)const;
    JSON::Value parseJSON(const std::string &json) const;
};
#endif