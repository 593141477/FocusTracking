#ifndef GUMBO_BASED_SPIDER_H__
#define GUMBO_BASED_SPIDER_H__

#include "common.h"
#include "CrawlingResult.h"
#include "SpiderBase.h"
#include "gumbo.h"
#include <vector>
#include <string>

class GumboBasedSpider
{
protected:
    const char* original;
    std::vector<GumboNode*> storedNodes;

    void parsePage(const std::string &content);

    std::string childrenToText(GumboNode* node) const;

private:
    void searchNode(GumboNode *node, int level);
    void convertResult();
protected:
    virtual bool searchNodeCallback(GumboNode *node, int &level) = 0;
    virtual void resultCallback(GumboNode *node) = 0;
};
#endif