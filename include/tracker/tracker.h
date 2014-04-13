#ifndef __tracker_h__
#define __tracker_h__

#include "title.h"
#include <vector>

class tracker {
public:
    virtual std::vector<titlebundle> trackFocus(const std::vector<title>&) = 0;
    virtual ~tracker(){};
};

#endif
