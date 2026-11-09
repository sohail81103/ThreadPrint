#include "ThreadPool.h"

#include <iostream>
#include <chrono>

ThreadPool::ThreadPool(JobQueue& queue, int worker_count)
    : job_queue(queue) {

    for (int i = 0; i < worker_count; i++) {

        workers.emplace_back(
            &ThreadPool::workerLoop,
            this,
            i + 1
        );
    }

    std::cout << worker_count
              << " worker threads started.\n";
}

ThreadPool::~ThreadPool() {

    stop();

}

void ThreadPool::workerLoop(int worker_id) {

    while (true) {

        PrintJob job(0, "");

        // Wait for the next available job
        if (!job_queue.waitAndPop(job)) {
            break;
        }

        processJob(job, worker_id);
    }

    {
        std::lock_guard<std::mutex> lock(output_mutex);

        std::cout << "Worker " << worker_id
                  << " shutting down.\n";
    }
}

void ThreadPool::processJob(
    const PrintJob& job,
    int worker_id
) {

    {
        std::lock_guard<std::mutex> lock(output_mutex);

        std::cout << "Worker " << worker_id
                  << " started processing Job "
                  << job.job_id << '\n';

        std::cout << "Document: "
                  << job.content << '\n';
    }

    // Simulate printing for 3 seconds
    std::this_thread::sleep_for(
        std::chrono::seconds(3)
    );

    {
        std::lock_guard<std::mutex> lock(output_mutex);

        std::cout << "Worker " << worker_id
                  << " completed Job "
                  << job.job_id << '\n';
    }
}

void ThreadPool::stop() {

    // Closing the queue wakes all waiting workers
    job_queue.close();

    for (std::thread& worker : workers) {

        if (worker.joinable()) {
            worker.join();
        }
    }

    workers.clear();
}