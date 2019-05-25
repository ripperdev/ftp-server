//
// Created by ripper on 5/24/19.
//

#include "XFTPTask.h"

void XFTPTask::SetCallback(bufferevent *bev) {
    bufferevent_setcb(bev, ReadCB, WriteCB, EventCB, this);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void XFTPTask::EventCB(bufferevent *bev, short what, void *arg) {
    auto task = reinterpret_cast<XFTPTask *>(arg);
    task->Event(bev, what);
}

void XFTPTask::ReadCB(bufferevent *bev, void *arg) {
    auto task = reinterpret_cast<XFTPTask *>(arg);
    task->Read(bev);
}

void XFTPTask::WriteCB(bufferevent *bev, void *arg) {
    auto task = reinterpret_cast<XFTPTask *>(arg);
    task->Write(bev);
}

bool XFTPTask::Init() {
    return true;
}

void XFTPTask::ResponseCMD(const std::string &msg) {
    if (!cmdTask || !cmdTask->cmdbev)
        return;
    std::cout << "ResponseCMD: " << msg << std::endl;
    bufferevent_write(cmdTask->cmdbev, msg.c_str(), msg.size());
}

void XFTPTask::ConnectPORT() {
    if (ip.empty() || port <= 0 || !base) {
        std::cout << "ConnectPORT failed" << std::endl;
        return;
    }
    Close();
    cmdbev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sock;
    memset(&sock, 0, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    evutil_inet_pton(AF_INET, ip.c_str(), &sock.sin_addr.s_addr);
    SetCallback(cmdbev);
    ///添加超时
    timeval readtime = {120, 0};
    bufferevent_set_timeouts(cmdbev, &readtime, nullptr);
    bufferevent_socket_connect(cmdbev, reinterpret_cast<sockaddr *>(&sock), sizeof(sock));
}

void XFTPTask::Send(const std::string &data) {
    Send(data.c_str(), data.size());
}

void XFTPTask::Send(const char *data, int len) {
    if (!cmdbev)
        return;
    bufferevent_write(cmdbev, data, len);
}

void XFTPTask::Close() {
    if (cmdbev) {
        bufferevent_free(cmdbev);
        this->cmdbev = nullptr;
    }
    if (fp) {
        fclose(fp);
        fp = nullptr;
    }
}
