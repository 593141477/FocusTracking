#include "JsonBasedSpider.h"
using std::string;
string JsonBasedSpider::filterJSCode(const string &js)const
{
    auto b = js.find('{');
    auto e = js.rfind('}');

    if (b == string::npos || e == string::npos || b >= e)
        return js;

    return js.substr(b, e - b + 1);
}

JSON::Value JsonBasedSpider::parseJSON(const std::string &json) const
{
	return parse_string(json);
}
