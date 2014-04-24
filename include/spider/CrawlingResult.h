#ifndef CRAWLING_RESULT_H__
#define CRAWLING_RESULT_H__

#include <vector>
#include <string>
#include <ctime>

typedef struct {
    std::string  title;
    std::string  url;
    std::time_t  date;
} CrawlingResultItem;

class CrawlingResult : public std::vector<CrawlingResultItem>
{


};
#endif