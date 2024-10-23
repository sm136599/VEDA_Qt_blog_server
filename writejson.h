#ifndef WRITEJSON_H
#define WRITEJSON_H

#include <string>
#include "json/json.h"

// JSON 응답 작성 함수
std::string makeResponse(const std::string &status, const std::string &message);

// 성공 응답을 만드는 함수
std::string successResponse();

// 실패 응답을 만드는 함수
std::string errorResponse(const std::string &msg);

// 게시물 리스트를 JSON으로 작성하는 함수
std::string writePostsResponse(const Json::Value &posts);

// 댓글 리스트를 JSON으로 작성하는 함수
std::string writePostDetailResponse(const Json::Value &post, const Json::Value &comments);

#endif // WRITEJSON_H
