#ifndef CRAWLING_RESULT_H__
#define CRAWLING_RESULT_H__

#include <vector>
#include <string>

class CrawlingResult
{
public:
	typedef struct
	{
		std::string title;
	}ResultItem;
	CrawlingResult();
	~CrawlingResult();
private:
	std::vector<ResultItem> mList;
	
};
#endif