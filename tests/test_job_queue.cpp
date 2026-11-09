#include <gtest/gtest.h>
#include <vector>
#include <thread>
#include "JobQueue.h"

// Test 1: Verify FIFO behaviour
TEST(JobQueueTest, MaintainsFIFOOrder) {

    JobQueue queue;

    PrintJob job1(1, "Document A");
    PrintJob job2(2, "Document B");
    PrintJob job3(3, "Document C");

    queue.push(job1);
    queue.push(job2);
    queue.push(job3);

    PrintJob result(0, "");

    ASSERT_TRUE(queue.tryPop(result));
    EXPECT_EQ(result.job_id, 1);

    ASSERT_TRUE(queue.tryPop(result));
    EXPECT_EQ(result.job_id, 2);

    ASSERT_TRUE(queue.tryPop(result));
    EXPECT_EQ(result.job_id, 3);
}

// Test 2: Verify empty queue behaviour
TEST(JobQueueTest, HandlesEmptyQueue) {

    JobQueue queue;

    PrintJob result(0, "");

    EXPECT_FALSE(queue.tryPop(result));

    EXPECT_EQ(queue.size(), 0);
}

// Test 3: Verify queue size
TEST(JobQueueTest, TracksQueueSize) {

    JobQueue queue;

    queue.push(PrintJob(1, "Document A"));
    queue.push(PrintJob(2, "Document B"));

    EXPECT_EQ(queue.size(), 2);

    PrintJob result(0, "");

    queue.tryPop(result);

    EXPECT_EQ(queue.size(), 1);
}

// Test 4: Verify concurrent submissions
TEST(JobQueueTest, HandlesConcurrentSubmissions) {

    JobQueue queue;

    const int thread_count = 4;
    const int jobs_per_thread = 100;

    std::vector<std::thread> producers;

    for (int i = 0; i < thread_count; i++) {

        producers.emplace_back([&queue, i]() {

            for (int j = 0; j < jobs_per_thread; j++) {

                int id = i * jobs_per_thread + j;

                queue.push(PrintJob(id, "Test document"));
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }

    EXPECT_EQ(queue.size(), thread_count * jobs_per_thread);
}