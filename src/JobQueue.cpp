#include "JobQueue.h"

bool JobQueue::push(const PrintJob& job) {

    {
        std::lock_guard<std::mutex> lock(queue_mutex);

        if (closed) {
            return false;
        }

        jobs.push(job);
    }

    // Notify one waiting worker
    condition.notify_one();

    return true;
}

bool JobQueue::tryPop(PrintJob& job) {

    std::lock_guard<std::mutex> lock(queue_mutex);

    if (jobs.empty()) {
        return false;
    }

    job = jobs.front();

    jobs.pop();

    return true;
}

bool JobQueue::waitAndPop(PrintJob& job) {

    std::unique_lock<std::mutex> lock(queue_mutex);

    // Wait until a job arrives or the queue closes
    condition.wait(lock, [this] {
        return !jobs.empty() || closed;
    });

    // No jobs remain and queue is closed
    if (jobs.empty()) {
        return false;
    }

    job = jobs.front();

    jobs.pop();

    return true;
}

int JobQueue::size() {

    std::lock_guard<std::mutex> lock(queue_mutex);

    return static_cast<int>(jobs.size());
}

void JobQueue::close() {

    {
        std::lock_guard<std::mutex> lock(queue_mutex);

        closed = true;
    }

    // Wake all waiting workers
    condition.notify_all();
}