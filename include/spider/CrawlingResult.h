#ifndef CRAWLING_RESULT_H__
#define CRAWLING_RESULT_H__

#include <vector>
#include <string>

typedef struct {
    std::u32string title;
    std::string  url;
} CrawlingResultItem;

class CrawlingResult : public std::vector<CrawlingResultItem>
{


};
#endif