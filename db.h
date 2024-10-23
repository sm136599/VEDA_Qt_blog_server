#ifndef DB_H
#define DB_H

#include <string>
#include "/usr/include/mysql/mysql.h"
#include <vector>

extern MYSQL *conn;
// MariaDB 연결 함수
bool connectDB();

// 유저 정보를 추가하는 함수
bool addUser(const std::string &user_id, const std::string &user_pw, const std::string &nickname);

// 로그인 정보 확인 함수
int loginUser(std::string user_id, std::string user_pw);

// 글 정보를 추가하는 함수
bool addPost(const std::string &user_id, const std::string &title, const std::string &post_des);

// 댓글 정보를 추가하는 함수
bool addComment(int post_num, const std::string &user_id, const std::string &comment_des);

// 모든 글을 가져오는 함수
std::vector<std::string> getAllPosts();

// 특정 글과 댓글을 가져오는 함수
bool getPostWithComments(int post_num, std::string &post, std::vector<std::string> &comments);

#endif // DB_H

