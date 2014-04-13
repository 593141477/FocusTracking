#ifndef __title_h__
#define __title_h__

#include <string>
#include <vector>

class title {
public:
    std::u32string name;
    std::string url;
    int rank;
};

class titlebundle {
private:
    std::vector< std::u32string > tags;
    std::vector< title > titles;
public:
    int getSize() const {
        return titles.size();
    }
    void addTitle(const title &_t) {
        titles.push_back(_t);
    }
    void addTags(const std::u32string &_t) {
        tags.push_back(_t);
    }
    const std::vector< std::u32string >& getTags() const {
        return tags;
    }
    const std::vector< title >& getTitles() const {
        return titles;
    }
};

#endif
