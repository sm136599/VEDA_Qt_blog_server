#ifndef WRITEJSON_H
#define WRITEJSON_H

#include <string>
#include "json/json.h"

// JSON 응답
std::string makeResponse(const std::string &status, const std::string &message);

//회원가입 성공
std::string successJoin(const std::string &message);

//회원가입 실패
std::string errorJoin(const std::string &message);

//로그인 성공
std::string successLogin(const std::string &nickname);

//로그인 실패
std::string failedLogin();

// 성공 응답
std::string successResponse();

// 실패 응답
std::string errorResponse(const std::string &msg);

// 게시물 리스트
std::string writePostsResponse(const std::vector<Json::Value> &posts);

// 댓글 리스트
std::string writePostDetailResponse(const Json::Value &post, const Json::Value &comments);

#endif // WRITEJSON_H
