#ifndef READJSON_H
#define READJSON_H

#include <string>
#include "json/json.h"

// JSON 요청 파싱 함수
bool parseRequest(const std::string &json_str, Json::Value &root);

// 회원가입 요청을 파싱하는 함수
bool parseJoinRequest(const std::string &json_str, std::string &loginId, std::string &password, std::string &name);

// 로그인 요청을 파싱하는 함수
bool parseLoginRequest(const std::string &json_str, std::string &loginId, std::string &password);

// 글쓰기 요청을 파싱하는 함수
bool parsePostRequest(const std::string &json_str, std::string &title, std::string &writer, std::string &description);

// 댓글 작성 요청을 파싱하는 함수
bool parseCommentRequest(const std::string &json_str, int &postNumber, std::string &writer, std::string &description);

#endif // READJSON_H
