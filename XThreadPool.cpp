//
// Created by ripper on 5/22/19.
//

#include "XThreadPool.h"

void XThreadPool::Init(int num) {
    this->threadCount = num;
    this->lastThread = -1;
    for (int i = 0; i < num; i++) {
        auto t = new XThread();
        std::cout << "Create thread " << i + 1 << std::endl;
        t->id = i + 1;
        t->Start();
        threads.emplace_back(t);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}

void XThreadPool::Dispatch(XTask *task) {
    ///采用轮询机制
    if (!task)
        return;
    int tid = (lastThread + 1) % threadCount;
    lastThread = tid;
    XThread *t = threads[tid];
    t->AddTask(task);
    t->Activate();
}
