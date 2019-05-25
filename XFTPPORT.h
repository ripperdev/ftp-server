//
// Created by ripper on 5/24/19.
//

#ifndef EV_FTP_SERVER_XFTPPORT_H
#define EV_FTP_SERVER_XFTPPORT_H

#include "XFTPTask.h"
#include <vector>

class XFTPPORT : public XFTPTask {
    void Parse(const std::string &type, const std::string &msg) override;
};


#endif //EV_FTP_SERVER_XFTPPORT_H
