#include "writejson.h"
#include <iostream>

std::string makeResponse(const std::string &status, const std::string &message) {
    Json::Value root;
    root["status"] = status;
    root["message"] = message;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

std::string successResponse() {
    std::cout << "send message\n";
    return makeResponse("200", "success");
}

std::string errorResponse(const std::string &msg) {
    return makeResponse("400", msg);
}

std::string writePostsResponse(const Json::Value &posts) {
    Json::Value root;
    root["posts"] = posts;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

std::string writePostDetailResponse(const Json::Value &post, const Json::Value &comments) {
    Json::Value root;
    root["post"] = post;
    root["comments"] = comments;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}
