#include "SqliteDatabaseStorage.h"
#include <cstdio>

using std::string;

SqliteDatabaseStorage::SqliteDatabaseStorage(std::string storage):StorageBase(storage)
{
	if(sqlite3_open((storage+".db").c_str(), &db)){
		sqlite3_close(db);
		db = NULL;
		throw 0;
	}
}

SqliteDatabaseStorage::~SqliteDatabaseStorage()
{
	if(db) {
		sqlite3_close(db);
	}
}


bool SqliteDatabaseStorage::InsertCrawlingResult(string site_name, const CrawlingResult &result)
{
	for(auto &item : result){
		sqlite3_stmt *stmt;
		int ret;

		if(SQLITE_OK != sqlite3_prepare_v2(db,
			"INSERT INTO CrawlingResult(title,url,date) VALUES(?,?,?)", -1, &stmt, NULL) || !stmt) {

			fprintf(stderr, "sqlite3_prepare_v2 failed\n");
			return false;
		}
		if(SQLITE_OK != sqlite3_bind_text(stmt, 1, item.title.c_str(), -1, SQLITE_STATIC) ||
			SQLITE_OK != sqlite3_bind_text(stmt, 2, item.url.c_str(), -1, SQLITE_STATIC) ||
			SQLITE_OK != sqlite3_bind_int64(stmt, 3, item.date) ) {

			fprintf(stderr, "sqlite3_bind_* failed\n");
			return false;
		}

		ret = sqlite3_step(stmt);
		if(ret != SQLITE_DONE) {
			fprintf(stderr, "sqlite3_step returned %d\n", ret);
		}

		sqlite3_finalize(stmt);
	}
	return true;
}

CrawlingResult SqliteDatabaseStorage::FindResultByDate(
		std::chrono::system_clock::time_point start,
		std::chrono::system_clock::time_point end)
{
	
}