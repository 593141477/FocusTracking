#include "trackerForce.h"

#include <set>

trackerForce::trackerForce() {
    limit = 0.2;
}

trackerForce::~trackerForce() {
    
}

bool trackerForce::titlebundleCmp(const titlebundle &a, const titlebundle &b) {
    return a.getSize() > b.getSize();
}

double trackerForce::evaluate(const titlebundle& bundle, const title &tt) {
    std::u32string name = tt.name;
    std::set<char32_t> charset;
    for (int i = 0; i < name.size(); i++)
        charset.insert(tt.name[i]);

    const std::vector<title> titles = bundle.getTitles();
    int totalLength = 0, intersection = 0;
    for (std::vector<title>::const_iterator ii = titles.cbegin(); ii != titles.cend(); ii++) {
        totalLength += ii->name.size();
        for (int i = 0; i < ii->name.size(); i++) 
            if (charset.find(ii->name[i]) != charset.end())
                intersection++;
    } 
    return (double)intersection / (double)totalLength;
}

std::vector<titlebundle> trackerForce::trackFocus(const std::vector<title> &titles) {
    std::vector<titlebundle> ret;
    for (std::vector<title>::const_iterator ii = titles.cbegin(); ii != titles.cend(); ii++) {
        int best = -1;
        double bestValue = -1;
        
        for (std::vector<titlebundle>::iterator jj = ret.begin(); jj != ret.end(); jj++) {
            double nowValue = evaluate(*jj, *ii);
            if (nowValue > bestValue) {
                bestValue = nowValue;
                best = jj - ret.begin();
            }
        }

        if (best == -1 || bestValue < limit) {
            titlebundle newbundle;
            newbundle.addTitle(*ii);
            ret.push_back(newbundle);
        } else {
            ret[best].addTitle(*ii);
        }
    }
    sort(ret.begin(), ret.end(), titlebundleCmp);
    return ret;
}
