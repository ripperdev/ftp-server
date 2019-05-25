//
// Created by ripper on 5/24/19.
//

#ifndef EV_FTP_SERVER_XFTPLIST_H
#define EV_FTP_SERVER_XFTPLIST_H

#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "XFTPTask.h"

namespace fs = std::filesystem;

class XFTPLIST : public XFTPTask {
public:
    void Parse(const std::string &type, const std::string &msg) override;

    void Write(bufferevent *bev) override;

    void Event(bufferevent *bev, short what) override;

private:
    static std::string GetDirData(const std::string &path);

    static std::string getFilePermissions(const fs::directory_entry &f);

    static std::string getFileTime(const fs::directory_entry &f);

    static int getDirectoryCount(const fs::path &fp);
};

#endif //EV_FTP_SERVER_XFTPLIST_H
