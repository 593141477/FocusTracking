#include "SpiderBase.h"
#include <curl/curl.h>
#include <iconv.h>
#include <string>
#include <cctype>
#include "PcreRegex.h"

using std::string;
using std::make_pair;

SpiderBase::SpiderBase()
{
    state = SPIDER_FREE;
}
SpiderBase::~SpiderBase()
{
}

RetType SpiderBase::StartCrawling()
{
    try{
        doCrawling();
        state = SPIDER_HAS_DATA;
    }catch(...){
        log_err("Exception occurred in %s", __func__);
    }
    return RET_OK;
}
RetType SpiderBase::StopCrawling()
{
    return RET_OK;
}
bool SpiderBase::IsCrawlingResultAvailable()
{
    return state == SPIDER_HAS_DATA;
}
CrawlingResult SpiderBase::GetCrawlingResult()
{
    if (!IsCrawlingResultAvailable())
        return CrawlingResult();

    return result;
}

static long curl_custom_writer(void *data, int size, int nmemb, string &content)
{
    long sizes = size * nmemb;
    content.append((char *)data, sizes);
    return sizes;
}

std::pair<std::string, std::string> SpiderBase::httpGet(string url, string charset) const
{
    CURLcode code;
    CURL *curl = NULL;
    char error[CURL_ERROR_SIZE];
    char *ct;
    string  content;

    const char *URL = url.c_str();

    curl = curl_easy_init();
    if (curl == NULL) {
        log_err("Failed to create CURL connection");
        throw 1;
    }
    code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
    if (code != CURLE_OK) {
        log_err("Failed to set error buffer [%d]", code );
        throw 1;
    }
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    code = curl_easy_setopt(curl, CURLOPT_URL, URL);
    if (code != CURLE_OK) {
        log_err("Failed to set URL [%s]", error);
        throw 1;
    }
    code = curl_easy_setopt(curl, CURLOPT_REFERER, URL);
    if (code != CURLE_OK) {
        log_err("Failed to set refer page [%s]", error);
        throw 1;
    }
    code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    if (code != CURLE_OK) {
        log_err("Failed to set redirect option [%s]", error );
        throw 1;
    }
    code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_custom_writer);
    if (code != CURLE_OK) {
        log_err("Failed to set writer [%s]", error);
        throw 1;
    }
    code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    if (code != CURLE_OK) {
        log_err("Failed to set write data [%s]", error );
        throw 1;
    }
    code = curl_easy_perform(curl);
    if (code != CURLE_OK) {
        log_err("Failed to get '%s' [%s]", URL, error);
        throw 1;
    }
    if(charset.empty()){
        /* ask for the content-type */
        code = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
        charset = "UTF-8";
        if ((CURLE_OK == code) && ct && extractCharsetFromHeader(ct, charset)) {
            printf("We received Content-Type: %s\n", ct);
        } else {
            log_warn("Failed to get charset from Content-Type");
        }
        if (extractCharsetFromMetaTag(content, charset)) {
            log_info("Use charset definition in <meta> tag.");        
        }
    }
    printf("Charset is: [%s]\n", charset.c_str());
    curl_easy_cleanup(curl);
    return make_pair(content, charset);
}

bool SpiderBase::extractCharsetFromHeader(const char *field, std::string &outCharset) const
{
    string word;
    const char *st = strstr(field, "charset=");
    if (!st)
        return false;
    st += 8;
    while (*st == '-' || isalnum(*st)) {
        word += *st;
        st++;
    }
    outCharset = word;
    return true;
}

bool SpiderBase::extractCharsetFromMetaTag(const string &content, std::string &outCharset) const
{
    static PcreRegex re("<meta[^>]+charset=([\\-\\w]+)[^>]*>");

    return re.MatchFirstSubstring(content.c_str(), content.size(), outCharset);
}

/*
static int print_one (unsigned int namescount, const char *const *names,
                      void *data)
{
    unsigned int i;
    (void)data;
    for (i = 0; i < namescount; i++) {
        if (i > 0)
            putc(' ', stdout);
        fputs(names[i], stdout);
    }
    putc('\n', stdout);
    return 0;
}
*/

string SpiderBase::charsetConv(string source, string charset) const
{
    size_t in_size = source.size();
    size_t out_size = in_size * 3;
    size_t ret;

    // iconvlist(print_one, NULL);
    
    iconv_t hdl = iconv_open ("UTF-8", charset.c_str());
    if (hdl == (iconv_t)(-1)) {
        perror("iconv_open");
        throw 1;
    }

    const static int option_val = 1;
    iconvctl(hdl, ICONV_SET_DISCARD_ILSEQ, (void*)&(option_val));

    string outbuf;
    outbuf.resize(out_size);
    char *in_buf = (char *)source.c_str();
    char *out_buf = (char *)outbuf.c_str();

    ret = iconv(hdl, &in_buf, &in_size, &out_buf, &out_size);
    if (ret == (size_t)(-1)) {
        iconv_close(hdl);
        perror("iconv");
        throw 1;
    }
    iconv_close(hdl);

    outbuf.resize(out_size);

    return outbuf;
}

string SpiderBase::downloadWebPage(string url, std::string charset) const
{
    std::pair<std::string, std::string> res;
    std::string str;

    res = httpGet(url, charset);
    // printf("%s\n", "httpGet returned");
    str = charsetConv(res.first, res.second);
    return str;
}

