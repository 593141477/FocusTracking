
#include "SpiderFor163Roll.h"
#include "SpiderForQQRoll.h"
#include "SpiderForSinaRoll.h"
#include <iostream>
#include <chrono>
#include <iomanip>

int test_json_spiders(int argc, char const *argv[])
{
    using std::endl;
    using std::cout;
    
    SpiderBase *spider;

    auto print_func = [&spider](){
        for(const CrawlingResultItem& item : spider->GetCrawlingResult()) {
            cout << item.title << endl
                << item.url << endl
                << std::put_time(std::localtime(&item.date), "%F %T") << endl << endl;
        }
        cout << "\n==================\n" << endl;
    };

    spider = new SpiderForSinaRoll;
    spider->StartCrawling();
    print_func();
    delete spider;

    spider = new SpiderFor163Roll;
    spider->StartCrawling();
    print_func();
    delete spider;
    
    spider = new SpiderForQQRoll;
    spider->StartCrawling();
    print_func();
    delete spider;

    return 0;
}