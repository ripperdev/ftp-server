//
// Created by ripper on 5/24/19.
//

#ifndef EV_FTP_SERVER_XFTPFACTORY_H
#define EV_FTP_SERVER_XFTPFACTORY_H

#include "XTask.h"
#include "XFTPServerCMD.h"
#include "XFTPUSER.h"
#include "XFTPLIST.h"
#include "XFTPPORT.h"
#include "XFTPRETR.h"
#include "XFTPSTOR.h"

class XFTPFactory {
public:
    static XFTPFactory *GetInstance();

    XTask *CreateTask();
private:
    XFTPFactory() = default;
};

#endif //EV_FTP_SERVER_XFTPFACTORY_H
