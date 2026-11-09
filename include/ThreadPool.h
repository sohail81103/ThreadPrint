#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <mutex>

#include "JobQueue.h"

class ThreadPool {

private:

    JobQueue& job_queue;

    std::vector<std::thread> workers;

    std::mutex output_mutex;

    void workerLoop(int worker_id);

    void processJob(const PrintJob& job, int worker_id);

public:

    ThreadPool(JobQueue& queue, int worker_count);

    ~ThreadPool();

    void stop();

};

#endif