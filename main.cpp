#include <iostream>
#include <csignal>
#include <cstring>
#include <event.h>
#include <event2/listener.h>
#include "XThreadPool.h"
#include "XFTPFactory.h"

using namespace std;

#define PORT 5001

void listen_cb(evconnlistener *e, evutil_socket_t s, sockaddr *sock, int socklen, void *arg) {
    cout << "listen_cb" << endl;
    XTask *task = XFTPFactory::GetInstance()->CreateTask();
    task->sock = s;
    XThreadPool::GetInstance()->Dispatch(task);
}

int main() {
    if (signal(SIGPIPE, SIG_IGN) == SIG_IGN) {
        return 1;
    }

    /// 1.初始化线程池
    XThreadPool::GetInstance()->Init(10);

    auto base = event_base_new();
    if (base) {
        cout << "event_base_new success" << endl;
    }
    sockaddr_in sock;
    memset(&sock, 0, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT);

    auto ev = evconnlistener_new_bind(
            base,
            listen_cb,
            base,
            LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
            10,
            reinterpret_cast<sockaddr *>(&sock),
            sizeof(sock)
    );
    ///事件分发处理
    if (base) {
        event_base_dispatch(base);
    }

    if (ev) {
        evconnlistener_free(ev);
    }
    if (base) {
        event_base_free(base);
    }
    return 0;
}
