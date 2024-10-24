#include "writejson.h"
#include <iostream>

std::string makeResponse(const std::string &status, const std::string &message) {
    Json::Value root;
    root["status"] = status;
    root["message"] = message;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

std::string successJoin(const std::string &message) {
    std::cout << "Join Success!\n";
    Json::Value root;
    root["status"] = "200";
    root["result"] = "success";
    root["message"] = message;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

std::string errorJoin(const std::string &message) {
    std::cout << "Join Failed\n";
    Json::Value root;
    root["status"] = "400";
    root["result"] = "failed";
    root["message"] = message;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

std::string successLogin(const std::string &nickname) {
    Json::Value root;
    root["status"] = "200";
    root["user"] = nickname;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

std::string failedLogin() {
    Json::Value root;
    root["status"] = "400";
    root["user"] = "";

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

std::string writePostsResponse(const std::vector<Json::Value> &posts) {
    Json::Value root;
    Json::Value postArray(Json::arrayValue);

    for (const auto &post : posts) {
        postArray.append(post);
    }

    root["posts"] = postArray;

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
