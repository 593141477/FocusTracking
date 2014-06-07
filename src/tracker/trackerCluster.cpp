#include "trackerCluster.h"
#include <cmath>
#include <fstream>
#include <iostream>

trackerCluster::trackerCluster() {
    clusterNum = 100;
    loopNum = 10;
    std::ifstream model;
    model.open("segmentModel.txt");
    if (!model.is_open()) {
        std::cout << "MODEL FILE FAILED" << std::endl;
        exit(0);
    }
    segmenter.init(model);
    model.close();
}

trackerCluster::trackerCluster(int c, int l) {
    clusterNum = c;
    loopNum = l;
    std::ifstream model;
    model.open("segmentModel.txt");
    if (!model.is_open()) {
        std::cout << "MODEL FILE FAILED" << std::endl;
        exit(0);
    }
    segmenter.init(model);
    model.close();
}

trackerCluster::~trackerCluster() {}

std::vector<titlebundle> trackerCluster::trackFocus(const std::vector<title> &titles) {
    int n = titles.size();
    std::vector< std::vector<std::u32string> > titlesSegment(n);
    trie<std::u32string> dict;
    std::vector<int> trieId2Df;
    std::vector< std::vector<double> > tfidf(n);
    
    for (int i = 0; i < n; i++) {
        titlesSegment[i] = segmenter.getSegment(titles[i].name);
        //if (titlesSegment[i].size() <= 1) {
        //    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> cv;
        //    std::cout << "ERROR" << cv.to_bytes(titles[i].name) << std::endl;
        //}
        int countErase = 0, m = titlesSegment[i].size();
        for (int j = 0; j < m; j++)
            if (hasMark(titlesSegment[i][j - countErase]))
                titlesSegment[i].erase(titlesSegment[i].begin() + j - countErase++);
        std::sort(titlesSegment[i].begin(), titlesSegment[i].end());
        for (auto ii = titlesSegment[i].begin(); ii != titlesSegment[i].end(); ii++) {
            if (ii != titlesSegment[i].begin() && *ii == *(ii - 1)) continue;
            int trieId = dict.insert(*ii);
            if (trieId + 1 > trieId2Df.size()) trieId2Df.resize(trieId + 1);
            trieId2Df[trieId]++;
        }
    }
    
    for (int i = 0; i < n; i++) {
        tfidf[i].resize(titlesSegment[i].size());
        for (int j = 0; j < titlesSegment[i].size(); j++)
            tfidf[i][j] = std::log((double)n / (double)trieId2Df[dict.find(titlesSegment[i][j])]);
    }
    
    if (clusterNum > n) clusterNum = n;
    std::vector< std::vector<int> > cluster(clusterNum);
    std::vector<bool> isClustered;
    for (int t = 0; t < loopNum; t++) {
        isClustered.clear();
        isClustered.resize(n, false);
        if (t == 0)
            for (int i = 0; i < clusterNum; i++) {
                cluster[i].push_back(i);
                isClustered[i] = 1;
            }
        else
            for (int i = 0; i < clusterNum; i++) {
                int center = getCenter(cluster[i], titlesSegment, tfidf);
                cluster[i].clear();
                cluster[i].push_back(center);
                isClustered[center] = 1;
            }
        for (int i = 0; i < n; i++) {
            if (isClustered[i]) continue;
            double best = -1;
            int bestId = -1;
            for (int j = 0; j < clusterNum; j++) {
                double now = cosSimilarity(titlesSegment[ cluster[j][0] ], tfidf[ cluster[j][0] ],
                                           titlesSegment[i], tfidf[i]);
                if (now > best) {
                    best = now;
                    bestId = j;
                }
            }
            if (bestId == -1) bestId = 0;
            cluster[bestId].push_back(i);
        }
    }
    
    std::vector<titlebundle> ret(clusterNum);
    for (int i = 0; i < clusterNum; i++) {
        std::map<std::u32string, double> wordCount;
        for (int j = 0; j < cluster[i].size(); j++) {
            ret[i].addTitle(titles[cluster[i][j]]);
            int now = cluster[i][j];
            for (int p = 0; p < titlesSegment[now].size(); p++) {
                if (wordCount.find(titlesSegment[now][p]) != wordCount.end())
                    wordCount[titlesSegment[now][p]] = tfidf[now][p] * titlesSegment[now][p].size();
                else
                    wordCount[titlesSegment[now][p]] += tfidf[now][p] * titlesSegment[now][p].size();
            }
        }
        std::vector< std::pair<double, std::u32string> > countSort;
        for (auto ii = wordCount.begin(); ii != wordCount.end(); ii++)
            countSort.push_back(std::make_pair(ii->second, ii->first));
        std::sort(countSort.rbegin(), countSort.rend());
        for (int t = 0; t < countSort.size(); t++)
            ret[i].addTags(countSort[t].second);
    }
    std::sort(ret.rbegin(), ret.rend(),
              [](titlebundle &a, titlebundle &b) {return a.getTitles().size() < b.getTitles().size();});
    return ret;
}

bool trackerCluster::isMark(char32_t ch) {
    if (0x4E00 <= (int)ch && (int)ch < 0xA000) return 0;
    if ('a' <= ch && ch <= 'z') return 0;
    if ('A' <= ch && ch <= 'Z') return 0;
    if ('0' <= ch && ch <= '9') return 0;
    return 1;
}

bool trackerCluster::hasMark(const std::u32string &word) {
    for (int i = 0; i < word.size(); i++)
        if (isMark(word[i]))
            return 1;
    return 0;
}

double trackerCluster::cosSimilarity(const std::vector<std::u32string> &seg1,
                                     const std::vector<double> &tfidf1,
                                     const std::vector<std::u32string> &seg2,
                                     const std::vector<double> &tfidf2) {
    double sim = 0, len1 = 0, len2 = 0;
    for (auto ii = tfidf1.begin(); ii != tfidf1.end(); ii++)
        len1 += (*ii) * (*ii);
    for (auto ii = tfidf2.begin(); ii != tfidf2.end(); ii++)
        len2 += (*ii) * (*ii);
    for (int i = 0, j = 0; i < seg1.size() && j < seg2.size();)
        if (seg1[i] == seg2[j])
            sim += tfidf1[i++] * tfidf2[j++];
        else if (seg1[i] < seg2[j])
            i++;
        else
            j++;
    return sim / std::sqrt(len1 * len2);
}

int trackerCluster::getCenter(const std::vector<int> &category,
              const std::vector< std::vector<std::u32string> > &segment,
              const std::vector< std::vector<double> > &tfidf) {
    double best = 0;
    int bestId = -1;
    for (auto i = category.cbegin(); i != category.cend(); i++) {
        double now = 0;
        for (auto j = category.cbegin(); j != category.cend(); j++)
            if (i != j)
                now += cosSimilarity(segment[*i], tfidf[*i], segment[*j], tfidf[*j]);
        if (now > best) {
            best = now;
            bestId = *i;
        }
    }
    if (bestId == -1) bestId = category[0];
    return bestId;
}

