#include "title.h"
#include "tracker.h"
#include "trackerForce.h"
#include "SpiderFor163.h"
#include "SpiderForQQ.h"
#include "SpiderForSina.h"
#include "SqliteDatabaseStorage.h"
#include <iostream>
#include <codecvt>

std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;

void appendTitleSet(std::vector<title> &s, const CrawlingResult &res)
{
    for(auto &x : res){
        title tmp;
        tmp.name = Utility::utf8string_to_u32(x.title.c_str());
        tmp.url = x.url;
        s.push_back(tmp);
    }
}
int main() {
    SqliteDatabaseStorage *storage;
    storage = new SqliteDatabaseStorage("crawling");

    std::vector<title> titleSet;
    SpiderBase *spider;

    spider = new SpiderForSina;
    spider->StartCrawling();
    appendTitleSet(titleSet, spider->GetCrawlingResult());
    storage->InsertCrawlingResult("sina", spider->GetCrawlingResult());
    delete spider;

    spider = new SpiderFor163;
    spider->StartCrawling();
    appendTitleSet(titleSet, spider->GetCrawlingResult());
    storage->InsertCrawlingResult("163", spider->GetCrawlingResult());
    delete spider;
    
    spider = new SpiderForQQ;
    spider->StartCrawling();
    appendTitleSet(titleSet, spider->GetCrawlingResult());
    storage->InsertCrawlingResult("qq", spider->GetCrawlingResult());
    delete spider;

    delete storage;

    tracker *mytracker = new trackerForce;
    
    std::cout << "\n********************\n";
    
    std::vector<titlebundle> bundle = mytracker->trackFocus(titleSet);
    
    for (std::vector<titlebundle>::iterator ii = bundle.begin(); ii != bundle.end();ii++) {
        const std::vector<title> titles = ii->getTitles();
        for (std::vector<title>::const_iterator jj = titles.cbegin(); jj != titles.cend(); jj++) {
            std::cout << cv.to_bytes(jj->name) << std::endl;;
        }
        std::cout << "===================" << std::endl;
    }
    
    delete mytracker;
    mytracker = NULL;
}