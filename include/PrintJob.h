#ifndef PRINT_JOB_H
#define PRINT_JOB_H

#include <string>

struct PrintJob {

    int job_id;

    std::string content;

    PrintJob(int id, const std::string& data)
        : job_id(id), content(data) {
    }
};

#endif