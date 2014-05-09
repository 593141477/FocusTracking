#include "SqliteDatabaseStorage.h"
#include "common.h"
#include <cstdio>

using std::string;

SqliteDatabaseStorage::SqliteDatabaseStorage(std::string storage): StorageBase(storage)
{
    if (sqlite3_open((storage + ".db").c_str(), &db)) {
        sqlite3_close(db);
        db = NULL;
        throw 0;
    }
}

SqliteDatabaseStorage::~SqliteDatabaseStorage()
{
    if (db) {
        sqlite3_close(db);
    }
}


bool SqliteDatabaseStorage::InsertCrawlingResult(string site_name, const CrawlingResult &result)
{
    const char *site = site_name.c_str();
    for (auto &item : result) {
        sqlite3_stmt *stmt;
        int ret;

        if (SQLITE_OK != sqlite3_prepare_v2(db,
                                            "INSERT INTO CrawlingResult(site,title,url,date) VALUES(?,?,?,?)", -1, &stmt, NULL) || !stmt) {

            log_err("sqlite3_prepare_v2 failed\n");
            return false;
        }
        if (SQLITE_OK != sqlite3_bind_text(stmt, 1, site, -1, SQLITE_STATIC) ||
                SQLITE_OK != sqlite3_bind_text(stmt, 2, item.title.c_str(), -1, SQLITE_STATIC) ||
                SQLITE_OK != sqlite3_bind_text(stmt, 3, item.url.c_str(), -1, SQLITE_STATIC) ||
                SQLITE_OK != sqlite3_bind_int64(stmt, 4, item.date) ) {

            log_err("sqlite3_bind_* failed\n");
            sqlite3_finalize(stmt);
            return false;
        }

        ret = sqlite3_step(stmt);
        if (ret != SQLITE_DONE) {
            // log_err("sqlite3_step returned %d\n", ret);
        }

        sqlite3_finalize(stmt);
    }
    return true;
}

CrawlingResult SqliteDatabaseStorage::FindResultByDate(
    std::chrono::system_clock::time_point start,
    std::chrono::system_clock::time_point end)
{
    CrawlingResult result;
    sqlite3_stmt *stmt;

    if (SQLITE_OK != sqlite3_prepare_v2(db,
                                        "SELECT title,url,date FROM CrawlingResult WHERE date BETWEEN ? AND ?", -1, &stmt, NULL) || !stmt) {

        log_err("sqlite3_prepare_v2 failed\n");
        throw 0;
    }
    if (SQLITE_OK != sqlite3_bind_int64(stmt, 1, std::chrono::system_clock::to_time_t(start)) ||
            SQLITE_OK != sqlite3_bind_int64(stmt, 2, std::chrono::system_clock::to_time_t(end)) ) {

        log_err("sqlite3_bind_* failed\n");
        sqlite3_finalize(stmt);
        throw 0;
    }

    for (;;) {
        int ret;
        ret = sqlite3_step(stmt);

        if (ret == SQLITE_ROW) {
            CrawlingResultItem item;
            const char *text;

            text = (const char *) sqlite3_column_text(stmt, 0);
            item.title.assign(text);

            text = (const char *) sqlite3_column_text(stmt, 1);
            item.url.assign(text);

            item.date = sqlite3_column_int64(stmt, 2);

            result.push_back(item);
        } else if (ret == SQLITE_DONE) {
            break;
        } else {
            log_err("%s: sqlite3_step returned %d\n", __func__, ret);
        }
    }

    sqlite3_finalize(stmt);

    return result;
}
