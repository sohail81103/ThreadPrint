# ThreadPrint

**A concurrent C++ TCP print-job management server**

ThreadPrint is a multithreaded network print-server application built using C++17 and POSIX sockets. It accepts print-job submissions from TCP clients, stores jobs in a thread-safe FIFO queue, and processes them using a fixed-size worker thread pool.

The project focuses on systems programming concepts including network communication, concurrency, thread synchronization, producer-consumer architecture, and resource management.

Physical printer hardware is not required. Printing is simulated to demonstrate concurrent job processing.

## Current Features

- TCP server using POSIX sockets.
- TCP client for submitting print jobs.
- Unique job IDs for submitted jobs.
- FIFO job scheduling using `std::queue`.
- Thread-safe queue operations using `std::mutex`.
- RAII-based mutex management using `std::lock_guard`.
- Worker notification using `std::condition_variable`.
- Fixed-size worker thread pool implemented using `std::thread`.
- Concurrent processing of independent print jobs.
- Simulated printing with configurable processing delay.
- Queue closure and worker-thread joining.
- GoogleTest-based unit tests.

## Architecture

```text
          TCP Client 1
               |
          TCP Client 2
               |
          TCP Client 3
               |
               v
        +---------------+
        |   TCP Server  |
        | POSIX Sockets |
        +---------------+
               |
               v
        +---------------+
        |   Job Queue   |
        |   FIFO        |
        |   std::mutex  |
        +---------------+
               |
       Condition Variable
               |
       +-------+-------+
       |       |       |
       v       v       v
    Worker 1 Worker 2 Worker 3
       |       |       |
       v       v       v
    Simulated Print Processing
```

The server acts as the producer of print jobs, while worker threads act as consumers.

Workers wait for new jobs using a condition variable rather than continuously polling the queue.

The queue mutex protects shared queue operations, ensuring that multiple workers can safely retrieve jobs.

## Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Core implementation |
| POSIX Sockets | TCP communication |
| std::thread | Worker threads |
| std::mutex | Shared queue synchronization |
| std::condition_variable | Worker notification |
| CMake | Build system |
| GoogleTest | Unit testing |

## Project Structure

```text
ThreadPrint/
├── client/
│   └── client.cpp
├── include/
│   ├── PrintJob.h
│   ├── JobQueue.h
│   └── ThreadPool.h
├── src/
│   ├── main.cpp
│   ├── JobQueue.cpp
│   └── ThreadPool.cpp
├── tests/
│   └── test_job_queue.cpp
├── docs/
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Requirements

- C++17-compatible compiler
- CMake 3.20 or later
- POSIX-compatible operating system
- GoogleTest
- Git

The current socket implementation targets macOS and other POSIX-compatible systems.

## Installation and Build

### 1. Clone the repository

```bash
git clone https://github.com/YOUR_USERNAME/ThreadPrint.git
cd ThreadPrint
```

### 2. Install dependencies (macOS)

```bash
brew install cmake googletest
```

### 3. Configure the project

```bash
cmake -S . -B build
```

### 4. Build

```bash
cmake --build build
```

## Running the Application

### Start the server

```bash
./build/threadprint_server
```

The server listens on TCP port 8080 and starts three worker threads.

### Submit print jobs

Open another terminal and execute:

```bash
./build/threadprint_client
```

Run the client multiple times to submit multiple jobs.

The server assigns unique job IDs and places submitted jobs in the FIFO queue.

Worker threads retrieve and process available jobs independently.

## Running Unit Tests

Execute:

```bash
ctest --test-dir build --output-on-failure
```

### Current Test Coverage

| Test | Description |
|---|---|
| MaintainsFIFOOrder | Verifies FIFO scheduling |
| HandlesEmptyQueue | Tests empty queue behaviour |
| TracksQueueSize | Verifies queue-size updates |
| HandlesConcurrentSubmissions | Tests concurrent queue submissions |

Current test result: **4/4 tests passing.**

## Current Limitations

This is an actively developed project. The following capabilities are not yet implemented or fully verified:

- Concurrent TCP client handling.
- Persistent print-job status tracking.
- Comprehensive invalid-request handling.
- Robust disconnected-client handling.
- Graceful shutdown of the complete server.
- Worker-pool unit tests.
- ThreadSanitizer and concurrency stress testing.

The current server accepts client connections sequentially, while print-job processing is concurrent.

## Planned Improvements

- Concurrent client connection handling.
- Job lifecycle management (Pending, Processing, Completed).
- Job cancellation.
- Improved error handling and server reliability.
- Worker-pool unit testing.
- ThreadSanitizer-based concurrency validation.
- Architecture and workflow documentation.

## Learning Objectives

ThreadPrint is designed to explore practical C++ systems programming concepts:

- TCP client-server communication.
- Producer-consumer architecture.
- Multithreaded programming.
- Mutexes and condition variables.
- Thread-pool design.
- RAII and resource management.
- Concurrent data structures and testing.

---

**Project Status:** Active Development

**Language:** C++17