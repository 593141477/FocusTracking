#include "common.h"
#include <sstream>
#include <locale>
#include <codecvt>

std::vector<std::string> &Utility::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> Utility::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::u32string Utility::utf8string_to_u32(const char* src)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;
	return cv.from_bytes(src);
}

std::string Utility::u32string_to_utf8(const std::u32string &src)
{
	std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;
	return cv.to_bytes(src);
}
