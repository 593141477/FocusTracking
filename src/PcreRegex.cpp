#include "PcreRegex.h"
#include <iostream>

PcreRegex::PcreRegex(const char *pattern)
{
    const char *error;
    int erroffset;
    re = pcre_compile(pattern, PCRE_CASELESS, &error, &erroffset, NULL);
    if (!re) {
        std::cerr << error;
        throw 1;
    }
}
PcreRegex::~PcreRegex()
{
    pcre_free(re);
}
bool PcreRegex::MatchFirstSubstring(const char *src, size_t len, std::string &out)
{
    const int OVECCOUNT = 2*3;
    int   ovector[OVECCOUNT];

    int rc = pcre_exec (
                 re,                   /* the compiled pattern */
                 0,                    /* no extra data - pattern was not studied */
                 src,                  /* the string to match */
                 len,                  /* the length of the string */
                 0,                    /* start at offset 0 in the subject */
                 0,                    /* default options */
                 ovector,              /* output vector for substring information */
                 OVECCOUNT);           /* number of elements in the output vector */
    if(rc < 0) {
    	return false;
    }
    out.assign(src + ovector[2], ovector[3] - ovector[2]);
    return true;
}