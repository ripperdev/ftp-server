//
// Created by ripper on 5/22/19.
//

#ifndef THREADPOOL_XTHREADPOOL_H
#define THREADPOOL_XTHREADPOOL_H

#include <vector>
#include <thread>
#include <iostream>
#include "XThread.h"
#include "XTask.h"

class XThreadPool {
public:
    static XThreadPool *GetInstance() {
        static XThreadPool threadPool;
        return &threadPool;
    }

    ~XThreadPool() = default;

    void Init(int num);

    void Dispatch(XTask *task);

private:
    XThreadPool() = default;

    int threadCount = 0;
    int lastThread = -1;
    std::vector<XThread *> threads;
};


#endif //THREADPOOL_XTHREADPOOL_H
