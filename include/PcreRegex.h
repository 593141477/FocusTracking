#ifndef PCRE_REGEX_H__
#define PCRE_REGEX_H__
#include <pcre.h>
#include <string>

class PcreRegex
{
	pcre* re;
public:
	PcreRegex(const char *pattern);
	~PcreRegex();
	bool MatchFirstSubstring(const char *src, size_t len, std::string &out);
};
#endif
