#include "XFTPRETR.h"

void XFTPRETR::Parse(const std::string &type, const std::string &msg) {
    int pos = msg.rfind(' ') + 1;
    std::string filename = msg.substr(pos, msg.size() - pos - 2);
    std::string path = cmdTask->rootDir;
    path += cmdTask->curDir;
    path += filename;
    fp = fopen(path.c_str(), "rb");
    if (fp != nullptr) {
        ConnectPORT();
        ResponseCMD("150 File OK\r\n");
        bufferevent_trigger(cmdbev, EV_WRITE, 0);
    } else {
        ResponseCMD("450 file open failed!\r\n");
    }
}

void XFTPRETR::Write(bufferevent *bev) {
    if (fp == nullptr) {
        return;
    }
    int len = fread(buf, 1, sizeof(buf), fp);
    if (len <= 0) {
        ResponseCMD("226 Transfer complete\r\n");
        Close();
        return;
    }
    Send(buf, len);
}

void XFTPRETR::Event(bufferevent *bev, short what) {
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        std::cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT" << std::endl;
        Close();
    } else if (what & BEV_EVENT_CONNECTED) {
        std::cout << "BEV_EVENT_CONNECTED" << std::endl;
    }
}
