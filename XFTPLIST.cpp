//
// Created by ripper on 5/24/19.
//

#include "XFTPLIST.h"

void XFTPLIST::Parse(const std::string &type, const std::string &msg) {
    if (type == "PWD") {
        std::string responseMsg = "257 \"";
        responseMsg += cmdTask->curDir;
        responseMsg += "\" is current dir.\r\n";
        ResponseCMD(responseMsg);
    } else if (type == "LIST") {
        ///1.连接数据通道 2.回应150状态码 3.发送目录数据通道 4.回应226 5.关闭连接

        ///命令通道回复消息
        ///数据通道发送目录
        //std::string dirData = "-rwxrwxrwx 1 root root 64463 Mar 14 09:53 101.jpg\r\n";
        std::string dirData = GetDirData(cmdTask->rootDir + cmdTask->curDir);
        ///1.连接数据通道
        ConnectPORT();
        ///2.回应150
        ResponseCMD("150 Here comes the directory listing.\r\n");
        ///3.数据通道发送数据
        Send(dirData);
    } else if (type == "CWD") {
        int pos = msg.rfind(' ') + 1;
        std::string path = msg.substr(pos, msg.size() - pos - 2);
        if (path[0] == '/') {
            cmdTask->curDir = path;
        } else {
            if (cmdTask->curDir[cmdTask->curDir.size() - 1] != '/') {
                cmdTask += '/';
            }
            cmdTask->curDir += path + "/";
        }
        ResponseCMD("250 Directory success changed.\r\n");
    } else if (type == "CDUP") {
        std::string path = cmdTask->curDir;
        if (path[path.size() - 1] == '/') {
            path = path.substr(0, path.size() - 1);
        }
        int pos = path.rfind('/');
        path = path.substr(0, pos);
        cmdTask->curDir = path;
        ResponseCMD("250 Directory success changed.\r\n");
    }
}

void XFTPLIST::Write(bufferevent *bev) {
    ///4.发送完成 回应226
    ResponseCMD("226 Transfer complete\r\n");
    ///5.关闭连接
    Close();
}

void XFTPLIST::Event(bufferevent *bev, short what) {
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        std::cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT" << std::endl;
        Close();
    } else if (what & BEV_EVENT_CONNECTED) {
        std::cout << "BEV_EVENT_CONNECTED" << std::endl;
    }
}

std::string XFTPLIST::GetDirData(const std::string &path) {
    std::cout << path << " " << fs::absolute(path) << std::endl;
    std::string data;
    for (auto &f:fs::directory_iterator(path)) {
        auto &fp = f.path();
        data += getFilePermissions(f) + " ";
        if (fs::is_directory(f)) {
            data += std::to_string(getDirectoryCount(fp)) + " " +
                    std::to_string(4096) + " ";
        } else {
            data += std::to_string(1) + " " +
                    std::to_string(f.file_size()) + " ";
        }
        data += getFileTime(f) + " ";
        data += fp.filename();
        data += "\r\n";
    }
    return data;
}

std::string XFTPLIST::getFilePermissions(const fs::directory_entry &f) {
    fs::perms permission = f.status().permissions();
    std::string permissions;
    ///directory
    permissions += f.is_directory() ? "d" : "-";
    ///owner
    permissions += (permission & fs::perms::owner_read) != fs::perms::none ? "r" : "-";
    permissions += (permission & fs::perms::owner_write) != fs::perms::none ? "w" : "-";
    permissions += (permission & fs::perms::owner_exec) != fs::perms::none ? "x" : "-";
    ///group
    permissions += (permission & fs::perms::group_read) != fs::perms::none ? "r" : "-";
    permissions += (permission & fs::perms::group_write) != fs::perms::none ? "w" : "-";
    permissions += (permission & fs::perms::group_exec) != fs::perms::none ? "x" : "-";
    ///others
    permissions += (permission & fs::perms::others_read) != fs::perms::none ? "r" : "-";
    permissions += (permission & fs::perms::others_write) != fs::perms::none ? "w" : "-";
    permissions += (permission & fs::perms::others_exec) != fs::perms::none ? "x" : "-";

    return permissions;
}

std::string XFTPLIST::getFileTime(const fs::directory_entry &f) {
    auto ftime = f.last_write_time();
    std::time_t fileTime = std::chrono::system_clock::to_time_t(ftime);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&fileTime), "%b %d %H:%M");
    return ss.str();
}

int XFTPLIST::getDirectoryCount(const fs::path &fp) {
    int ans = 2;
    for (auto &f:fs::directory_iterator(fp)) {
        if (fs::is_directory(f)) {
            ans++;
        }
    }
    return ans;
}
