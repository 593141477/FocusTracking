#include "SpiderFor163Roll.h"
#include "common.h"
#include <string>
#include <iostream>
using std::string;
using std::endl;
using std::cout;

void SpiderFor163Roll::doCrawling()
{
    result.clear();
    string content = downloadWebPage("http://news.163.com/special/0001220O/news_json.js");

    auto news_categories = JSON::Array(parseJSON(filterJSCode(content))["news"]);

    for (auto i = news_categories.begin(); i != news_categories.end(); ++i) {
        const JSON::Array &news = JSON::Array(*i);
        for (auto j = news.begin(); j != news.end(); ++j) {
            CrawlingResultItem out;
            JSON::Value item = *j;

            out.title = (string)item["t"];
            out.url = (string)item["l"];
            out.date = datetime2time_t((string)item["p"]);
            result.push_back(out);
        }
    }

}

time_t SpiderFor163Roll::datetime2time_t(const std::string &str)const
{
    tm t;
    int y, m, d, h, min, s;
    sscanf(str.c_str(), "%d-%d-%d%d:%d:%d", &y, &m, &d, &h, &min, &s);
    t.tm_year = y - 1900;
    t.tm_mon = m - 1;
    t.tm_mday = d;
    t.tm_hour = h;
    t.tm_min = min;
    t.tm_sec = s;
    t.tm_isdst = 0;

    return mktime(&t);
}
