#ifndef JOB_QUEUE_H
#define JOB_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "PrintJob.h"

class JobQueue {

private:

    std::queue<PrintJob> jobs;

    std::mutex queue_mutex;

    std::condition_variable condition;

    bool closed = false;

public:

    bool push(const PrintJob& job);

    bool tryPop(PrintJob& job);

    bool waitAndPop(PrintJob& job);

    int size();

    void close();

};

#endif