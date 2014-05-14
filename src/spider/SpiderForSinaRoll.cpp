#include "SpiderForSinaRoll.h"
#include "common.h"
#include <string>
#include <iostream>
using std::string;
using JSON::Value;
using JSON::Array;

void SpiderForSinaRoll::doCrawling()
{
    string content = downloadWebPage("http://roll.news.sina.com.cn/interface/rollnews_ch_out_interface.php?col=89&ch=01&offset_page=0&offset_num=0&num=60&page=1", "GBK");
    result.clear();
    Array list = (Array)(parseJSON(filterJSCode(content))["list"]);

    for (auto it = list.begin(); it != list.end(); ++it)
    {
        CrawlingResultItem out;
        Value item = *it;

        out.title = (string)item["title"];
        out.url = (string)item["url"];
        out.date = (long long)item["time"];
        result.push_back(out);
    }
}
