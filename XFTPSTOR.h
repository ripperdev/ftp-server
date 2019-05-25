//
// Created by ripper on 5/25/19.
//

#ifndef EV_FTP_SERVER_XFTPSTOR_H
#define EV_FTP_SERVER_XFTPSTOR_H

#include "XFTPTask.h"

class XFTPSTOR : public XFTPTask {
    void Parse(const std::string &type, const std::string &msg) override;

    void Read(bufferevent *bev) override;

    void Event(bufferevent *bev, short what) override;

private:
    FILE *fp = nullptr;
    char buf[1024] = {0};
};

#endif //EV_FTP_SERVER_XFTPSTOR_H
