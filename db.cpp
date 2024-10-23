#include "db.h"
#include <iostream>

MYSQL *conn;

bool connectDB() {
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "root", "1234", "blogdb", 3306, NULL, 0)) {
        std::cerr << "DB 연결 실패: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

bool addUser(const std::string &user_id, const std::string &user_pw, const std::string &nickname) {
    connectDB();
    std::string query = "INSERT INTO userinfo (user_id, user_pw, nickname, role) VALUES ('" + user_id + "', '" + user_pw + "', '" + nickname + "', 'user')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "유저 추가 실패: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

int loginUser(std::string user_id, std::string user_pw) {
    std::cout << "DB debug\n";
    connectDB();
    std::string query = "SELECT * FROM userinfo WHERE user_id='" + user_id + "' AND user_pw='" + user_pw + "'";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "로그인 실패: " << mysql_error(conn) << std::endl;
        return 1; // 로그인 실패
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        return 1; // 로그인 실패
    }
    return 0; // 로그인 성공
}

bool addPost(const std::string &user_id, const std::string &title, const std::string &post_des) {
    connectDB();
    std::string query = "INSERT INTO posts (user_id, title, post_des) VALUES ('" + user_id + "', '" + title + "', '" + post_des + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "글 추가 실패: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

bool addComment(int post_num, const std::string &user_id, const std::string &comment_des) {
    connectDB();
    std::string query = "INSERT INTO comments (post_num, user_id, comment_des) VALUES (" + std::to_string(post_num) + ", '" + user_id + "', '" + comment_des + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "댓글 추가 실패: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

std::vector<std::string> getAllPosts() {
    std::vector<std::string> posts;
    connectDB();
    if (mysql_query(conn, "SELECT post_num, title, user_id FROM posts")) {
        std::cerr << "글 조회 실패: " << mysql_error(conn) << std::endl;
        return posts;
    }
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        posts.push_back("PostID: " + std::string(row[0]) + ", Title: " + std::string(row[1]) + ", Writer: " + std::string(row[2]));
    }
    return posts;
}

bool getPostWithComments(int post_num, std::string &post, std::vector<std::string> &comments) {
    connectDB();
    std::string query = "SELECT * FROM posts WHERE post_num=" + std::to_string(post_num);
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "글 조회 실패: " << mysql_error(conn) << std::endl;
        return false;
    }
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    post = "PostID: " + std::string(row[0]) + ", Title: " + std::string(row[1]) + ", Writer: " + std::string(row[2]) + ", Description: " + std::string(row[3]);

    query = "SELECT * FROM comments WHERE post_num=" + std::to_string(post_num);
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "댓글 조회 실패: " << mysql_error(conn) << std::endl;
        return false;
    }
    result = mysql_store_result(conn);
    while ((row = mysql_fetch_row(result))) {
        comments.push_back("CommentID: " + std::string(row[0]) + ", Writer: " + std::string(row[1]) + ", Description: " + std::string(row[2]));
    }
    return true;
}

