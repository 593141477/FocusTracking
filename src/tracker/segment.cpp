#include "segment.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void segment::init(std::ifstream &model) {
    std::string line;
    while (getline(model, line)) {
        std::istringstream tokenizer(line);
        std::string word;
        tokenizer >> word;
        std::u32string word32 = converter.from_bytes(word);
        dict.insert(word32);
        std::reverse(word32.begin(), word32.end());
        inverseDict.insert(word32);
    }
}

std::vector<std::u32string> segment::getSegment(const std::u32string& sentence) {
    std::vector<int> forward = forwardMatching(sentence);
    //std::cout << "=======" << std::endl;
    std::vector<int> backward = backwardMatching(sentence);
    //std::cout << "forward*****" << std::endl;
    //print(transFormat(sentence, forward));
    //std::cout << "backward*****" << std::endl;
    //print(transFormat(sentence, backward));
    return transFormat(sentence, evaluate(sentence, forward, backward));
}

bool segment::isMark(char32_t ch) {
    if (0x4E00 <= (int)ch && (int)ch < 0xA000) return 0;
    if ('a' <= ch && ch <= 'z') return 0;
    if ('A' <= ch && ch <= 'Z') return 0;
    if ('0' <= ch && ch <= '9') return 0;
    return 1;
}

std::vector<int> segment::matching(const std::u32string &sentence, trie<std::u32string> &dict) {
    int start = 0;
    bool unknow = 0;
    std::vector<int> ret;
    while (start != sentence.size()) {
        int mat = dict.maxMatching(sentence.substr(start));
        //std::cout << mat << std::endl;
        if (mat) {
            ret.push_back(mat);
            unknow = 0;
            start += mat;
        } else {
            if (isMark(sentence[start])) {
                ret.push_back(1);
                unknow = 0;
                start++;
            } else {
                if (unknow) {
                    (*ret.rbegin())++;
                    start++;
                } else {
                    unknow = 1;
                    ret.push_back(1);
                    start++;
                }
            }
        }
    }
    return ret;
}

std::vector<int> segment::forwardMatching(const std::u32string& sentence) {
    return matching(sentence, dict);
}

std::vector<int> segment::backwardMatching(const std::u32string& sentence) {
    std::u32string inverseSentence = sentence;
    std::reverse(inverseSentence.begin(), inverseSentence.end());
    std::vector<int> ret = matching(inverseSentence, inverseDict);
    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::vector<int> segment::evaluate(const std::u32string &sentence, const std::vector<int> &forward, const std::vector<int> &backward) {
    if (equal(forward, backward)) return forward;
    int unknowForward = 0, unknowBackward = 0, start = 0;
    for (auto ii = forward.begin(); ii != forward.end(); start += *(ii++))
        if (!dict.find(sentence.substr(start, *ii))) unknowForward++;
    start = 0;
    for (auto ii = backward.begin(); ii != backward.end(); start += *(ii++))
        if (!dict.find(sentence.substr(start, *ii))) unknowBackward++;
    if (unknowForward < unknowBackward) return forward;
    if (unknowForward > unknowBackward) return backward;
    if (forward.size() < backward.size()) return forward;
    return backward;
}

bool segment::equal(const std::vector<int> &forward, const std::vector<int> &backward) {
    if (forward.size() != backward.size()) return 0;
    int n = forward.size();
    for (int i = 0; i < n; i++)
        if (forward[i] != backward[i]) return 0;
    return 1;
}

std::vector<std::u32string> segment::transFormat(const std::u32string &sentence, const std::vector<int> &answer) {
    std::vector<std::u32string> ret;
    int start = 0;
    for (auto ii = answer.begin(); ii != answer.end(); start += *(ii++))
        ret.push_back(sentence.substr(start, *ii));
    return ret;
}

void segment::print(const std::vector<std::u32string> &out) {
    for (auto ii = out.begin(); ii != out.end(); ii++)
        std::cout << converter.to_bytes(*ii) << std::endl;
    std::cout << "****************" << std::endl;
}