#ifndef __title_h__
#define __title_h__

#include <string>
#include <vector>

struct title {
	std::wstring name;
	std::string url;
	int rank;
};

struct titlebundle {
	std::vector< std::wstring > tags;
	std::vector< title > titles;
	int size() {
		return titles.size();
	}
};

#endif
