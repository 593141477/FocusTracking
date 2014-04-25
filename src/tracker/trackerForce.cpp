#include "trackerForce.h"
#include <set>
#include <cmath>

trackerForce::trackerForce() {
    //limit = 1.0 - (std::sqrt(5.0) - 1.0) / 2.0;
    limit = 0.2;
}

trackerForce::~trackerForce() {
    
}

bool trackerForce::isCJK(char32_t ch) {
    return 0x4E00 <= (int)ch && (int)ch < 0xA000;
}

bool trackerForce::titlebundleCmp(const titlebundle &a, const titlebundle &b) {
    return a.getSize() > b.getSize();
}

double trackerForce::evaluate(const titlebundle& bundle, const title &tt) {
    std::u32string name = tt.name;
    std::set<char32_t> charset;
    for (int i = 0; i < name.size(); i++)
        if (isCJK(name[i]))
            charset.insert(tt.name[i]);

    const std::vector<title> titles = bundle.getTitles();
    double totalScore = 0;
    for (std::vector<title>::const_iterator ii = titles.cbegin(); ii != titles.cend(); ii++) {
        int matchChar = 0, length = 0;
        for (int i = 0; i < ii->name.size(); i++) {
            if (isCJK(ii->name[i]))
                length++;
            else
                continue;
            if (charset.find(ii->name[i]) != charset.end())
                matchChar++;
        }
        totalScore += (double)matchChar / (double)ii->name.size();
    } 
    return totalScore / titles.size();
}

std::vector<titlebundle> trackerForce::trackFocus(const std::vector<title> &titles) {
    std::vector<titlebundle> ret;
    for (std::vector<title>::const_iterator ii = titles.cbegin(); ii != titles.cend(); ii++) {
        bool flag = 0;
        for (std::vector<titlebundle>::iterator jj = ret.begin(); jj != ret.end(); jj++)
            if (evaluate(*jj, *ii) > limit) {
                jj->addTitle(*ii);
                flag = 1;
            }

        if (!flag) {
            titlebundle newbundle;
            newbundle.addTitle(*ii);
            ret.push_back(newbundle);
        }
    }
    sort(ret.begin(), ret.end(), titlebundleCmp);
    return ret;
}
