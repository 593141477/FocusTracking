#ifndef STORAGE_BASH_H__
#define STORAGE_BASH_H__

#include <string>
#include <chrono>
#include "CrawlingResult.h"

class StorageBase
{
	std::string storageName;
public:
	StorageBase(std::string _storage):storageName(_storage){}
	virtual ~StorageBase(){}
	const char * GetStroageName(){
		return storageName.c_str();
	}
	virtual bool InsertCrawlingResult(std::string site_name, const CrawlingResult &result) = 0;

	virtual CrawlingResult FindResultByDate(
		std::chrono::system_clock::time_point start,
		std::chrono::system_clock::time_point end) = 0;

	CrawlingResult FindResultByDate(
		std::chrono::system_clock::time_point start,
		std::chrono::system_clock::duration dur) {

		return FindResultByDate(start, start + dur);
	}

};

#endif

