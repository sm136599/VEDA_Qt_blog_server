#include <iostream>
#include "readjson.h"
#include "writejson.h"
#include "db.h"

bool parseRequest(const std::string &json_str, Json::Value &root) {
    Json::CharReaderBuilder readerBuilder;
    std::string errs;
    std::istringstream ss(json_str);
    return Json::parseFromStream(readerBuilder, ss, &root, &errs);
}

bool parseJoinRequest(const std::string &json_str, std::string &loginId, std::string &password, std::string &passwordCheck, std::string &name) {
    Json::Value root;
    if (!parseRequest(json_str, root)) return false;
    
    loginId = root["loginId"].asString();
    password = root["password"].asString();
    passwordCheck = root["passwordCheck"].asString();
    name = root["name"].asString();
    std::cout << "ID: " << loginId << std::endl;
    std::cout << "PW: " << password << std::endl;
    std::cout << "PW Check: " << passwordCheck << std::endl;
    std::cout << "name: " << name << std::endl;

    return true;
}

bool parseLoginRequest(const std::string &json_str, std::string &loginId, std::string &password) {
    Json::Value root;
    if (!parseRequest(json_str, root)) return false;
    
    loginId = root["loginId"].asString();
    password = root["password"].asString();
    std::cout << "Login ID: " << loginId << std::endl;
    std::cout << "Password: " << password << std::endl;
    return true;
}

bool parsePostRequest(const std::string &json_str, std::string &title, std::string &writer, std::string &description) {
    Json::Value root;
    if (!parseRequest(json_str, root)) return false;
    
    title = root["subject"].asString();
    writer = root["writer"].asString();    
    description = root["description"].asString();
    std::cout << "title: " << title << std::endl;
    std::cout << "writer: " << writer << std::endl;
    std::cout << "description: " << description << std::endl; 
    return true;
}

bool parseCommentRequest(const std::string &json_str, int &postNumber, std::string &writer, std::string &description) {
    Json::Value root;
    if (!parseRequest(json_str, root)) return false;

    postNumber = root["postNumber"].asInt();
    writer = root["writer"].asString();
    description = root["description"].asString();
    std::cout << "postNum: " << postNumber << std::endl;
    std::cout << "writer: " << writer << std::endl;
    std::cout << "description: " << description << std::endl;
    return true;
}
