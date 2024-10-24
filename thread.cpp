#include "thread.h"
#include "readjson.h"
#include "writejson.h"
#include "db.h"
#include <iostream>
#include <vector>
#include "json/json.h"

void handleClient(const httplib::Request &req, httplib::Response &res) {
    if (req.method == "POST" && req.path == "/join") {
        std::string loginId, password, name;
        if (parseJoinRequest(req.body, loginId, password, name)) {
            if (addUser(loginId, password, name)) {
                res.set_content(successJoin("join Completed"), "application/json");
            } else {
                res.set_content(errorJoin("dupId"), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/login") {
        std::string loginId, password;
        if (parseLoginRequest(req.body, loginId, password)) {
            int result = loginUser(loginId, password);
            if (result == 0) {
                std::string nickname = findNick(loginId);
                std::cout << "login : " << nickname << std::endl;
                res.set_content(successLogin(nickname), "application/json");
            } else {
                res.set_content(failedLogin(), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/upload-post") {
        std::string title, writer, description;
        if (parsePostRequest(req.body, title, writer, description)) {
            if (addPost(writer, title, description)) {
                res.set_content(successResponse(), "application/json");
            } else {
                res.set_content(errorResponse("upload fail"), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/upload-comment") {
        int postNumber;
        std::string writer, description;
        if (parseCommentRequest(req.body, postNumber, writer, description)) {
            if (addComment(postNumber, writer, description)) {
                res.set_content(successResponse(), "application/json");
            } else {
                res.set_content(errorResponse("upload comment fail"), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/delete-post") {
        connectDB();
        Json::Value root;
        if (parseRequest(req.body, root)) {
            int postNumber = root["postNumber"].asInt();
            std::string query = "DELETE FROM posts WHERE post_num = " + std::to_string(postNumber);
            if (mysql_query(conn, query.c_str())) {
                res.set_content(errorResponse("delete post fail"), "application/json");
            } else {
                res.set_content(successResponse(), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/delete-comment") {
        Json::Value root;
        connectDB();
        if (parseRequest(req.body, root)) {
            int commentNumber = root["commentNumber"].asInt();
            std::string query = "DELETE FROM comments WHERE comment_num = " + std::to_string(commentNumber);
            if (mysql_query(conn, query.c_str())) {
                std::cout << "delete fail!" << std::endl;
                res.set_content(errorResponse("delete comment fail"), "application/json");
            } else {
                std::cout << "comment deleted!" << std::endl;
                res.set_content(successResponse(), "application/json");
            }
        }
    } else if (req.method == "GET" && req.path == "/get-all-post") {
        std::vector<Json::Value> posts = getAllPosts();
        res.set_content(writePostsResponse(posts), "application/json");
    } else if (req.method == "GET" && req.path.find("/get-post/") == 0) {
        std::string postIdStr = req.path.substr(10); // /get-post/{post-id}에서 post-id 추출
        int postId = std::stoi(postIdStr);
        std::string post;
        std::vector<std::string> comments;
        res.set_content(getPostWithComments(postId, post, comments), "application/json");
    } else if (req.method == "POST" && req.path == "/delete-user") {
        connectDB();
        Json::Value root;
        if (parseRequest(req.body, root)) {
            std::string user = root["user"].asString();
            std::string query = "DELETE FROM userinfo WHERE nickname = '" + user + "'";
            if (mysql_query(conn, query.c_str())) {
                res.set_content(errorResponse("failed to delete user"), "application/json");
            } else {
                res.set_content(successResponse(), "application/json");
            }
        }
    } else if (req.method == "POST" && req.path == "/edit-post") {   
        int post_id;
        std::string title, description;
        Json::Value root;
        parseRequest(req.body, root);
        post_id = root["postNumber"].asInt();
        title = root["subject"].asString();    
        description = root["description"].asString();    

        if (editPost(post_id, title, description)) {
            std::cout << "edit post success!" << std::endl;
            res.set_content(successResponse(), "application/json");
        } else {
            std::cout << "edit post fail!" << std::endl;
            res.set_content(errorResponse("edit fail"), "application/json");
        }
    } else if (req.method == "POST" && req.path == "/edit-comment") {  
        int comment_id;
        std::string description;
        Json::Value root;
        parseRequest(req.body, root);
        comment_id = root["commentNumber"].asInt();   
        description = root["description"].asString();   

        if(editComment(comment_id, description)){
            std::cout << "edit comment success!" << std::endl;
            res.set_content(successResponse(), "application/json");
        } else {
            std::cout << "edit comment fail!" << std::endl;
            res.set_content(errorResponse("edit fail"), "application/json");
        }
    }
    // 추가
    else {
        res.set_content(errorResponse("잘못된 요청"), "application/json");
    } 
    
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
    svr.Post("/edit-post", handleClient);
    svr.Post("/edit-comment", handleClient);
    svr.Get("/get-all-post", handleClient);
    svr.Get("/get-post/(.*)", handleClient); // "/get-post/{post-id}" 경로 처리
    
    std::cout << "Server started at http://192.168.0.85:8000\n";
    svr.listen("0.0.0.0", 8000); // 모든 외부 접속 허용
}
