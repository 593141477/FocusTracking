#ifndef FOCUS_TRACKING_COMMON_H__
#define FOCUS_TRACKING_COMMON_H__

#include <utility>
#include <vector>
#include <string>

enum RetType{
    RET_OK
};

class Utility{
public:
    static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
    static std::vector<std::string> split(const std::string &s, char delim);
    static std::u32string utf8string_to_u32(const char *src);
    static std::string u32string_to_utf8(const std::u32string &src);
};

#endif