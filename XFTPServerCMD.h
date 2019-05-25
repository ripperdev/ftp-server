//
// Created by ripper on 5/23/19.
//

#ifndef THREADPOOL_XFTPSERVERCMD_H
#define THREADPOOL_XFTPSERVERCMD_H

#include "XFTPTask.h"
#include <map>

class XFTPServerCMD : public XFTPTask {
public:
    bool Init() override;

    void Read(bufferevent *bev) override;

    void Event(bufferevent *bev, short what) override;

    /// 注册命令处理对象，不需要考虑线程安全，调用时还未分发到线程
    /// \param cmd
    /// \param call
    void Reg(const std::string &cmd, XFTPTask *call);

    XFTPServerCMD() = default;

    ~XFTPServerCMD();

private:
    std::map<std::string, XFTPTask *> calls;
    std::map<XFTPTask *, int> calls_del;
};

#endif //THREADPOOL_XFTPSERVERCMD_H
