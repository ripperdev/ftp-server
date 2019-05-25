cmake_minimum_required(VERSION 3.14)
project(ev_ftp_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(ev_ftp_server
        main.cpp
        XTask.h
        XThread.h XThread.cpp
        XThreadPool.h XThreadPool.cpp
        XFTPServerCMD.h XFTPServerCMD.cpp
        XFTPTask.cpp XFTPTask.h
        XFTPFactory.cpp XFTPFactory.h XFTPUSER.cpp XFTPUSER.h XFTPLIST.cpp XFTPLIST.h XFTPPORT.cpp XFTPPORT.h XFTPRETR.cpp XFTPRETR.h XFTPSTOR.cpp XFTPSTOR.h)

target_link_libraries(ev_ftp_server event pthread stdc++fs)
