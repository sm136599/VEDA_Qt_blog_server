#ifndef DB_H
#define DB_H

#include <string>
#include "/usr/include/mariadb/mysql.h"
#include <vector>
#include "json/json.h"

extern MYSQL *conn;
// DB 연결 
bool connectDB();

// 유저 정보 추가 
bool addUser(const std::string &user_id, const std::string &user_pw, const std::string &nickname);

// 로그인 정보 확인 
int loginUser(std::string user_id, std::string user_pw);

// 유저 닉네임 검색
std::string findNick(const std::string &user_id);

// 글 정보 추가
bool addPost(const std::string &user_id, const std::string &title, const std::string &post_des);

// 댓글 정보 추가
bool addComment(int post_num, const std::string &user_id, const std::string &comment_des);

// 모든 글 가져오기
std::vector<Json::Value> getAllPosts();

// 특정 글과 댓글 조회
std::string getPostWithComments(int post_num, std::string &post, std::vector<std::string> &comments);

//글 및 댓글 수정
bool editPost(int post_id, const std::string &title, const std::string &post_des);
bool editComment(int comment_id, const std::string &comment_des);

#endif // DB_H
