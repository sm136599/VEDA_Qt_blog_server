# VEDA_Qt_blog_server

## 소개

**VEDA_Qt_blog_server**는 `cpp-httplib`와 `jsoncpp`를 사용하여 HTTP 통신을 처리하고, MariaDB를 이용해 사용자, 게시글, 댓글을 관리하는 서버 애플리케이션입니다.

## 사용 오픈소스

- **cpp-httplib**: HTTP 서버 및 클라이언트 라이브러리  
  [cpp-httplib GitHub 링크](https://github.com/yhirose/cpp-httplib/blob/master/httplib.h)
- **jsoncpp**: JSON 처리 라이브러리  
  [jsoncpp GitHub 링크](https://github.com/open-source-parsers/jsoncpp)

## 파일 구조

```plaintext
/project-root
    ├── cpp-httplib
    │   └── httplib.h
    ├── nlohmann_json
    │   └── json.hpp
    ├── server.cpp
    ├── db.cpp
    └── CMakeLists.txt
```
## MariaDB 설정

```bash
sudo systemctl start mariadb
sudo systemctl enable mariadb
mysql -u root -p
```

## DB 설계

```sql
Table userinfo {
  user_id varchar [primary key]
  user_pw varchar
  nickname varchar 
  role varchar
}

Table posts {
  post_num integer [primary key]
  user_id varchar
  title varchar
  post_des text
}

Table comments {
  comment_num integer [primary key]
  user_id varchar
  comment_des text 
  post_num integer
}

Ref: userinfo.user_id > posts.user_id
Ref: userinfo.user_id > comments.user_id
Ref: posts.post_num > comments.post_num
```
