//
// Created by ripper on 5/24/19.
//

#include "XFTPFactory.h"

XFTPFactory *XFTPFactory::GetInstance() {
    static XFTPFactory factory;
    return &factory;
}

XTask *XFTPFactory::CreateTask() {
    auto cmd = new XFTPServerCMD();
    ///注册ftp消息处理函数
    cmd->Reg("USER", new XFTPUSER());
    auto list = new XFTPLIST();
    cmd->Reg("PWD", list);
    cmd->Reg("LIST", list);
    cmd->Reg("CWD", list);
    cmd->Reg("CDUP", list);
    cmd->Reg("PORT", new XFTPPORT());
    cmd->Reg("RETR", new XFTPRETR());
    cmd->Reg("STOR", new XFTPSTOR());

    return cmd;
}
