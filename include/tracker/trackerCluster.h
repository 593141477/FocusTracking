#ifndef __trackerCluster_h__
#define __trackerCluster_h__

#include "tracker.h"
#include "segment.h"

class trackerCluster : public tracker {
public:
    trackerCluster();
    trackerCluster(int, int);
    ~trackerCluster();
    virtual std::vector<titlebundle> trackFocus(const std::vector<title>&);
private:
    int clusterNum, loopNum;
    segment segmenter;
    bool hasMark(const std::u32string&);
    bool isMark(char32_t);
    double cosSimilarity(const std::vector<std::u32string>&,
                         const std::vector<double>&,
                         const std::vector<std::u32string>&,
                         const std::vector<double>&);
    int getCenter(const std::vector<int>&,
                  const std::vector< std::vector<std::u32string> >&,
                  const std::vector< std::vector<double> >&);
};

#endif
/*
#include "title.h"
#include "tracker.h"
#include "trackerForce.h"
#include "trackerCluster.h"
#include <iostream>
#include <codecvt>
using namespace std;

std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;

int main() {
    std::string s;
    vector<title> titles;
    
    while (getline(cin, s)) {
        u32string s32 = cv.from_bytes(s);
        title now;
        now.name = s32;
        titles.push_back(now);
    }
    
    trackerCluster T;
    vector<titlebundle> ans = T.trackFocus(titles);
    
    std::cout << "Algorithm end" << std::endl;
    for (auto ii = ans.begin(); ii != ans.end(); ii++) {
        cout << "==========================" << endl;
        const vector<title> tt = ii->getTitles();
        for (auto jj = tt.begin(); jj != tt.end(); jj++)
            cout << cv.to_bytes(jj->name) << endl;
    }
    
    return 0;
}
*/