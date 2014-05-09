
#include "SpiderFor163.h"
#include "SpiderForQQ.h"
#include "SpiderForSina.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

int test_gumbo_spiders(int argc, char const *argv[])
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

    spider = new SpiderForSina;
    spider->StartCrawling();
    print_func();
    delete spider;

    spider = new SpiderFor163;
    spider->StartCrawling();
    print_func();
    delete spider;
    
    spider = new SpiderForQQ;
    spider->StartCrawling();
    print_func();
    delete spider;

    return 0;
}