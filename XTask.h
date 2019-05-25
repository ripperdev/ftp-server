//
// Created by ripper on 5/23/19.
//

#ifndef THREADPOOL_XTASK_H
#define THREADPOOL_XTASK_H

#include <iostream>
#include <string>
#include <event.h>

class XTask {
public:
    event_base *base = nullptr;
    int sock = 0;
    int thread_id = 0;

    /// 初始化任务
    /// \return
    virtual bool Init() = 0;
};


#endif //THREADPOOL_XTASK_H
