//
// Created by ripper on 5/22/19.
//

#include "XThread.h"

using namespace std;

static void NotifyCB(evutil_socket_t fd, short events, void *arg);

void XThread::Start() {
    Setup();
    ///启动线程
    thread th(&XThread::Main, this);
    th.detach();
}

void XThread::Main() {
    cout << id << " XThread::Main() Begin" << endl;

    event_base_dispatch(base);
    event_base_free(base);

    cout << id << " XThread::Main() End" << endl;
}

bool XThread::Setup() {
#ifdef _WIN32
    ///创建一个socket pair,可以互相通信,fds[0]读,fds[1]写
    evutil_socket_t fds[2];
    if (evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0){
        cout << "evutil_socketpair failed" << endl;
        return false;
    }
    evutil_make_socket_nonblocking(fds[0]);
    evutil_make_socket_nonblocking(fds[1]);
#else
    ///创建管道,不能用send recv读取写入 用read write
    int fds[2];
    if (pipe(fds)) {
        cerr << "pipe failed" << endl;
        return false;
    }
#endif
    ///读取绑定到event事件中
    notify_send_fd = fds[1];

    ///创建libevent上下文（无锁）
    auto ev_config = event_config_new();
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK);
    this->base = event_base_new_with_config(ev_config);
    event_config_free(ev_config);
    if (!base) {
        cout << "event_base_new_with_config failed in thread " << id << endl;
        return false;
    }

    ///添加管道坚挺事件,用于激活线程执行任务
    auto *ev = event_new(base, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
    event_add(ev, nullptr);
    return true;
}

void XThread::Notify(evutil_socket_t fd, short which) {
    ///水平触发,只要没有接收完成,会再次进来
    char buf[2] = {0};
#ifdef _WIN32
    int res = recv(fd, buf, 1, 0);
#else
    int res = read(fd, buf, 1);
#endif
    if (res <= 0) {
        return;
    }
    cout << id << " thread " << buf << endl;

    XTask *task = nullptr;

    ///获取任务并初始化任务
    tasks_mutex.lock();
    if (tasks.empty()) {
        tasks_mutex.unlock();
        return;
    }
    task = tasks.front();///先进先出
    tasks.pop_front();
    tasks_mutex.unlock();
    task->Init();
}

void XThread::Activate() {
#ifdef _WIN32
    int re = send(this->notify_send_fd, "c", 1, 0);
#else
    int re = write(this->notify_send_fd, "c", 1);
#endif
    if (re <= 0) {
        cerr << "XThread::Activate() failed" << endl;
    }
}

void XThread::AddTask(XTask *task) {
    if (!task)
        return;
    task->base = this->base;
    tasks_mutex.lock();
    tasks.emplace_back(task);
    tasks_mutex.unlock();
}

static void NotifyCB(evutil_socket_t fd, short events, void *arg) {
    auto t = reinterpret_cast<XThread *>(arg);
    t->Notify(fd, events);
}
