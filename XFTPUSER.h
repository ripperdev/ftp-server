//
// Created by ripper on 5/24/19.
//

#ifndef EV_FTP_SERVER_XFTPUSER_H
#define EV_FTP_SERVER_XFTPUSER_H

#include "XFTPTask.h"

class XFTPUSER : public XFTPTask {
public:
    void Parse(const std::string &type, const std::string &msg) override;

    XFTPUSER() = default;

    ~XFTPUSER() = default;
};


#endif //EV_FTP_SERVER_XFTPUSER_H
