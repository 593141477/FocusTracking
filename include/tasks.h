#ifndef TASKS_H__
#define TASKS_H__

#include <chrono>
#include <string>

void StartTasks();
std::string GetBundleTitlesByDate(
    std::chrono::system_clock::time_point start,
    std::chrono::system_clock::time_point end);


#endif