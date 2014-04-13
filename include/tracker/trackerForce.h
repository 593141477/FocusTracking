#ifndef __trackerForce_h__
#define __trackerForce_h__

#include "tracker.h"

class trackerForce : public tracker {
    double limit;
public:
    trackerForce();
    ~trackerForce();
    trackerForce(double);
    static bool titlebundleCmp(const titlebundle&, const titlebundle&);
    double evaluate(const titlebundle&, const title&);
    virtual std::vector<titlebundle> trackFocus(const std::vector<title>&);
};


#endif
