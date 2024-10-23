#include "thread.h"
#include "readjson.h"
#include "writejson.h"
#include "db.h"
#include <iostream>
#include <vector>

void handleClient(const httplib::Request &req, httplib::Response &res) {
    if (req.method == "POST" && req.path == "/join") {
        std::string loginId, password, passwordCheck, name;
        if (parseJoinRequest(req.body, loginId, password, passwordCheck, name)) {
            if (addUser(loginId, password, name)) {
                res.set_content(successResponse(), "application/json");
            } else {
                res.set_content(errorResponse("회원가입 실패"), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/login") {
        std::string loginId, password;
        if (parseLoginRequest(req.body, loginId, password)) {
            std::cout << "debug\n";
            int result = loginUser(loginId, password);
            std::cout << result << " login\n";
            if (result == 0) {
                res.set_content(successResponse(), "application/json");
            } else {
                res.set_content(errorResponse("로그인 실패"), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/upload-post") {
        std::string title, writer, description;
        if (parsePostRequest(req.body, title, writer, description)) {
            if (addPost(writer, title, description)) {
                res.set_content(successResponse(), "application/json");
            } else {
                res.set_content(errorResponse("글쓰기 실패"), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/upload-comment") {
        int postNumber;
        std::string writer, description;
        if (parseCommentRequest(req.body, postNumber, writer, description)) {
            if (addComment(postNumber, writer, description)) {
                res.set_content(successResponse(), "application/json");
            } else {
                res.set_content(errorResponse("댓글 작성 실패"), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/delete-post") {
        Json::Value root;
        if (parseRequest(req.body, root)) {
            int postNumber = root["postNumber"].asInt();
            std::string query = "DELETE FROM posts WHERE post_num = " + std::to_string(postNumber);
            if (mysql_query(conn, query.c_str())) {
                res.set_content(errorResponse("글 삭제 실패"), "application/json");
            } else {
                res.set_content(successResponse(), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/delete-comment") {
        Json::Value root;
        if (parseRequest(req.body, root)) {
            int commentNumber = root["commentNumber"].asInt();
            std::string query = "DELETE FROM comments WHERE comment_num = " + std::to_string(commentNumber);
            if (mysql_query(conn, query.c_str())) {
                res.set_content(errorResponse("댓글 삭제 실패"), "application/json");
            } else {
                res.set_content(successResponse(), "application/json");
            }
        }
    } else if (req.method == "GET" && req.path == "/get-all-post") {
        std::vector<std::string> posts = getAllPosts();
        std::cout << posts[0] << std::endl;
        Json::Value postArray(Json::arrayValue);
        for (const auto &post : posts) {
            postArray.append(post);
        }
        Json::Value response;
        response["posts"] = postArray;
        res.set_content(response.toStyledString(), "application/json");
    } else if (req.method == "GET" && req.path.find("/get-post/") == 0) {
        std::string postIdStr = req.path.substr(10); // /get-post/{post-id}에서 post-id 추출
        int postId = std::stoi(postIdStr);
        std::string post;
        std::vector<std::string> comments;
        if (getPostWithComments(postId, post, comments)) {
            Json::Value postJson;
            postJson["post"] = post;
            Json::Value commentArray(Json::arrayValue);
            for (const auto &comment : comments) {
                commentArray.append(comment);
            }
            postJson["comments"] = commentArray;
            res.set_content(postJson.toStyledString(), "application/json");
        } else {
            res.set_content(errorResponse("게시글 조회 실패"), "application/json");
        }
    } else if (req.method == "POST" && req.path == "/delete-user") {
        Json::Value root;
        if (parseRequest(req.body, root)) {
            std::string user = root["user"].asString();
            std::string query = "DELETE FROM userinfo WHERE nickname = '" + user + "'";
            if (mysql_query(conn, query.c_str())) {
                res.set_content(errorResponse("유저 삭제 실패"), "application/json");
            } else {
                res.set_content(successResponse(), "application/json");
            }
        }
    } else {
        res.set_content(errorResponse("잘못된 요청"), "application/json");
    } 
    // 나머지 API 구현
}

int main() {
    httplib::Server svr;

    svr.Post("/join", handleClient);
    svr.Post("/login", handleClient);
    svr.Post("/upload-post", handleClient);
    svr.Post("/upload-comment", handleClient);
    svr.Post("/delete-post", handleClient);
    svr.Post("/delete-comment", handleClient);
    svr.Post("/delete-user", handleClient);
    svr.Get("/get-all-post", handleClient);
    svr.Get("/get-post/(.*)", handleClient); // "/get-post/{post-id}" 경로 처리
    
    std::cout << "Server started at http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080); // 모든 외부 접속 허용
}

