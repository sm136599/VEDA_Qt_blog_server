#ifndef THREAD_H
#define THREAD_H

#include "libs/httplib.h"

// 클라이언트 요청을 처리하는 함수
void handleClient(const httplib::Request &req, httplib::Response &res);

#endif // THREAD_H
