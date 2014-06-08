#include "title.h"
#include "SpiderFor163.h"
#include "SpiderForQQ.h"
#include "SpiderForQQRoll.h"
#include "SpiderForSina.h"
#include "SpiderForSinaRoll.h"
#include "SqliteDatabaseStorage.h"
#include "httpserver.h"
#include "tasks.h"
#include "tracker.h"
#include "trackerForce.h"
#include "trackerCluster.h"

#include <thread>
#include <chrono>
#include <functional>
#include <sstream>

#define DATABASE_NAME "crawling"
#define CRAWLING_INTERVAL (60*60*1000)

static void doCrawlingTasks()
{
    while (true) {
        log_info("Start crawling...");

        SqliteDatabaseStorage *storage;
        storage = new SqliteDatabaseStorage(DATABASE_NAME);

        SpiderBase *spider;

        spider = new SpiderForSina;
        spider->StartCrawling();
        storage->InsertCrawlingResult("sina", spider->GetCrawlingResult());
        delete spider;

        spider = new SpiderFor163;
        spider->StartCrawling();
        storage->InsertCrawlingResult("163", spider->GetCrawlingResult());
        delete spider;

        spider = new SpiderForQQ;
        spider->StartCrawling();
        storage->InsertCrawlingResult("qq", spider->GetCrawlingResult());
        delete spider;

        spider = new SpiderForQQRoll;
        spider->StartCrawling();
        storage->InsertCrawlingResult("qqroll", spider->GetCrawlingResult());
        delete spider;

        spider = new SpiderForSinaRoll;
        spider->StartCrawling();
        storage->InsertCrawlingResult("sinaroll", spider->GetCrawlingResult());
        delete spider;

        delete storage;

        log_info("Crawling finished.");

        std::chrono::milliseconds dura( CRAWLING_INTERVAL );
        std::this_thread::sleep_for( dura );
    }
}

tracker *gMytracker;
void StartTasks()
{
    log_info("Loading model");
    gMytracker = new trackerCluster;
    http_server_start();
    std::thread crawlingTask(doCrawlingTasks);

    log_info("Tasks started successfully.");

    crawlingTask.join();
}


static void appendTitleSet(std::vector<title> &s, const CrawlingResult &res)
{
    for(auto &x : res){
        title tmp;
        tmp.name = Utility::utf8string_to_u32(x.title.c_str());
        tmp.url = x.url;
        s.push_back(tmp);
    }
}

std::string GetBundleTitlesByDate(
    std::chrono::system_clock::time_point start,
    std::chrono::system_clock::time_point end)
{
    std::vector<title> titleSet;

    log_info("database started");
    SqliteDatabaseStorage *storage;
    storage = new SqliteDatabaseStorage(DATABASE_NAME);
    appendTitleSet(titleSet, storage->FindResultByDate(start, end));
    delete storage;
    log_info("database done");
    log_info("%d\n", titleSet.size());

    
    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;

    std::ostringstream os;

    log_info("tracker started");
    std::vector<titlebundle> bundle = gMytracker->trackFocus(titleSet);

    for (std::vector<titlebundle>::iterator ii = bundle.begin(); ii != bundle.end();ii++) {
        const std::vector<std::u32string> tags = ii->getTags();
        os << "TAGS: ";
        for (auto jj = tags.begin(); jj != tags.end(); jj++)
            os << cv.to_bytes(*jj) << " ";
        os << std::endl;
        const std::vector<title> titles = ii->getTitles();
        for (std::vector<title>::const_iterator jj = titles.cbegin(); jj != titles.cend(); jj++) {
            os << (jj->url) << std::endl;;
            os << cv.to_bytes(jj->name) << std::endl;;
        }
        os << "===================" << std::endl;
    }
    log_info("tracker done");
    
    return os.str();
}
