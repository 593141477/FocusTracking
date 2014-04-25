#ifndef SQLITE_STORAGE_H__
#define SQLITE_STORAGE_H__ 
#include "StorageBase.h"
#include <sqlite3.h>

class SqliteDatabaseStorage : public StorageBase
{
	sqlite3 *db;
public:
	SqliteDatabaseStorage(std::string storage);
	~SqliteDatabaseStorage();
	bool InsertCrawlingResult(std::string site_name, const CrawlingResult &result);
	CrawlingResult FindResultByDate(
		std::chrono::system_clock::time_point start,
		std::chrono::system_clock::time_point end);
};

#endif
