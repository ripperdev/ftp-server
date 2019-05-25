//
// Created by ripper on 5/24/19.
//

#include "XFTPUSER.h"

void XFTPUSER::Parse(const std::string &type, const std::string &msg) {
    std::cout << "XFTPUSER::Parse " << type << " " << msg << std::endl;
    ResponseCMD("230 Login successful.\r\n");
}
