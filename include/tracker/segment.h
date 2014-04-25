#ifndef __segment_h__
#define __segment_h__

#include "trie.h"
#include <string>
#include <vector>
#include <fstream>
#include <codecvt>
#include <locale>

class segment {
public:
    segment(std::ifstream&);
    std::vector<std::u32string> getSegment(const std::u32string&);
private:
    std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> converter;
    trie<std::u32string> dict;
    trie<std::u32string> inverseDict;
    std::vector<int> matching(const std::u32string&, trie<std::u32string>&);
    std::vector<int> forwardMatching(const std::u32string&);
    std::vector<int> backwardMatching(const std::u32string&);
    std::vector<int> evaluate(const std::u32string&, const std::vector<int>&, const std::vector<int>&);
    bool equal(const std::vector<int>&, const std::vector<int>&);
    bool isMark(char32_t);
    std::vector<std::u32string> transFormat(const std::u32string&, const std::vector<int>&);
    void print(const std::vector<std::u32string>&);
};

#endif
/*
 //usage sample
#include "segment.h"
#include <codecvt>
#include <string>
#include <locale>
#include <iostream>
#include <fstream>
using namespace std;

wstring_convert<codecvt_utf8<char32_t>,char32_t> converter;

int main() {
    ifstream model;
    model.open("segmentModel.txt");
    segment segmentor(model);
    model.close();
    
    string s;
    u32string s32;
    while (getline(cin, s)) {
        s32 = converter.from_bytes(s);
        vector<u32string> seg = segmentor.getSegment(s32);
        for (auto ii = seg.begin(); ii != seg.end(); ii++)
            cout << converter.to_bytes(*ii) << endl;
        cout << "============" << endl;
    }
}
*/