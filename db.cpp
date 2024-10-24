#include "db.h"
#include <iostream>
#include <vector>

MYSQL *conn;

bool connectDB() {
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "root", "1234", "blogdb", 3306, NULL, 0)) {
        std::cerr << "DB connect failed: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

bool addUser(const std::string &user_id, const std::string &user_pw, const std::string &nickname) {
    connectDB();
    std::string query = "INSERT INTO userinfo (user_id, user_pw, nickname, role) VALUES ('" + user_id + "', '" + user_pw + "', '" + nickname + "', 'user')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "ID already exists: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

int loginUser(std::string user_id, std::string user_pw) {
    connectDB();
    std::string query = "SELECT * FROM userinfo WHERE user_id='" + user_id + "' AND user_pw='" + user_pw + "'";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "login fail: " << mysql_error(conn) << std::endl;
        return 1; // 로그인 실패
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        return 1; // 로그인 실패
    }
    return 0; // 로그인 성공
}

std::string findNick(const std::string &user_id) {
    connectDB();
    std::string query = "SELECT nickname FROM userinfo WHERE user_id='" + user_id + "'";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to get nickname: " << mysql_error(conn) << std::endl;
        return 0;
    }
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    std::string nickname = row[0];
    return nickname;
}

bool addPost(const std::string &user_id, const std::string &title, const std::string &post_des) {
    connectDB();
    std::string query = "INSERT INTO posts (user_id, title, post_des) VALUES ('" + user_id + "', '" + title + "', '" + post_des + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to add post: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

bool addComment(int post_num, const std::string &user_id, const std::string &comment_des) {
    connectDB();
    std::string query = "INSERT INTO comments (post_num, user_id, comment_des) VALUES (" + std::to_string(post_num) + ", '" + user_id + "', '" + comment_des + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to add comment: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

std::vector<Json::Value> getAllPosts() {
    std::vector<Json::Value> posts;
    connectDB();
    if (mysql_query(conn, "SELECT post_num, title, user_id FROM posts")) {
        std::cerr << "Failed to load post: " << mysql_error(conn) << std::endl;
        return posts;
    }
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        Json::Value post;
        post["postNumber"] = std::stoi(row[0]);  // Convert post_num to int
        post["subject"] = row[1];              // Title as string
        post["writer"] = row[2];             // Writer as string
        posts.push_back(post);
    }
    mysql_free_result(result);
    return posts;
}

std::string getPostWithComments(int post_num, std::string &post, std::vector<std::string> &comments) {
    connectDB();
    std::string query = "SELECT * FROM comments WHERE post_num=" + std::to_string(post_num);
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to load comment: " << mysql_error(conn) << std::endl;
        return 0;
    }
    
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row;

    Json::Value commentsArray(Json::arrayValue);
    while ((row = mysql_fetch_row(result))) { //comments 배열생성
        //std::vector<Json::Value> comments;
        Json::Value comment;
        comment["commentNumber"] = std::stoi(row[0]);  
        comment["writer"] = row[1]; 
        comment["description"] = row[2];  
        commentsArray.append(comment);
    }
    
    //post 전체 문자열 생성
    query = "SELECT * FROM posts WHERE post_num=" + std::to_string(post_num);
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to load post: " << mysql_error(conn) << std::endl;
        return 0;
    }
    result = mysql_store_result(conn);
    row = mysql_fetch_row(result);
    //post = "postNumber: " + std::string(row[0]) + ", subject: " + std::string(row[1]) + ", writer: " + std::string(row[2]) + ", description: " + std::string(row[3]);
    Json::Value root;
    root["postNumber"] = std::stoi(row[0]);  
    root["subject"] = row[2];
    root["writer"] = row[1]; 
    root["description"] = row[3];
    root["comments"] =  commentsArray; 
    
    Json::StreamWriterBuilder writer;
    std::cout << Json::writeString(writer, root) << std::endl;
    return Json::writeString(writer, root);
}

bool editPost(int post_id, const std::string &title, const std::string &post_des) {
    connectDB();
    std::string query = "UPDATE posts SET title = '" + title + "', post_des = '" + post_des + "' WHERE post_num = " + std::to_string(post_id);
    std::cout << query << std::endl;
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to edit post: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

bool editComment(int comment_id, const std::string &comment_des) {
    connectDB();
    std::string query = "UPDATE comments SET comment_des = '" + comment_des + "' WHERE comment_num = " + std::to_string(comment_id);
    
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Failed to edit comment: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

