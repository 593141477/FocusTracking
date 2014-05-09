
#include "SqliteDatabaseStorage.h"
#include <iostream>

int test_sqlite_query(int argc, char const *argv[])
{
	using std::endl;
	using std::cout;
    using namespace std::chrono;

    SqliteDatabaseStorage *storage;
    storage = new SqliteDatabaseStorage("crawling");

    auto result = storage->FindResultByDate(system_clock::time_point(), system_clock::now());

    for(const CrawlingResultItem& item : result) {
    	cout << item.title << endl << item.url << endl << endl;
    }

    delete storage;

    return 0;
}