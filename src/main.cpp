#include "title.h"
#include "tracker.h"
#include "trackerForce.h"
#include "SpiderFor163.h"
#include "SpiderForQQ.h"
#include "SpiderForSina.h"
#include <iostream>
#include <codecvt>

std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;

int main() {

    SpiderBase *spider;

    spider = new SpiderForSina;
    spider->StartCrawling();
    delete spider;

    spider = new SpiderFor163;
    spider->StartCrawling();
    delete spider;
    
    spider = new SpiderForQQ;
    spider->StartCrawling();
    delete spider;

    freopen("../test/input.txt", "r", stdin);
    freopen("../test/output.txt", "w", stdout);
    tracker *mytracker = new trackerForce;
    
    std::vector<title> titleSet;
    
    std::u32string name;
    std::string url, _name;
    
    while (std::cin >> _name >> url) {
        name = cv.from_bytes(_name);
        title tmp;
        tmp.name = name;
        tmp.url = url;
        titleSet.push_back(tmp);
    }
    
    for (std::vector<title>::iterator ii = titleSet.begin(); ii != titleSet.end(); ii++)
        std::cout << cv.to_bytes(ii->name) << std::endl;
    
    
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