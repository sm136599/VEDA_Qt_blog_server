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
    ├── json
    │   └── json.h
    ├── writejson.cpp    
    ├── readjson.cpp
    ├── thread.cpp
    ├── db.cpp
    └── CMakeLists.txt
```
## MariaDB 설정

```bash
sudo systemctl start mariadb
sudo systemctl enable mariadb
mysql -u root -p
```
```sql
CREATE DATABASE blogdb;
USE blogdb;
CREATE TABLE userinfo (
    user_id VARCHAR(255) PRIMARY KEY,
    user_pw VARCHAR(255),
    nickname VARCHAR(255),
    role VARCHAR(50)
);
CREATE TABLE posts (
    post_num INT AUTO_INCREMENT PRIMARY KEY,
    user_id VARCHAR(255),
    title VARCHAR(255),
    post_des TEXT,
    FOREIGN KEY (user_id) REFERENCES userinfo(user_id) ON DELETE CASCADE
);
CREATE TABLE comments (
    comment_num INT AUTO_INCREMENT PRIMARY KEY,
    user_id VARCHAR(255),
    comment_des TEXT,
    post_num INT,
    FOREIGN KEY (user_id) REFERENCES userinfo(user_id) ON DELETE CASCADE,
    FOREIGN KEY (post_num) REFERENCES posts(post_num) ON DELETE CASCADE
);
```
## jsoncpp 설치
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install jsoncpp
```
jsoncpp 디렉토리에서 json 디렉토리를 프로젝트 폴더안으로 이동
