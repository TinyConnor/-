#ifndef __MYSYSTEM_H__
#define __MYSYSTEM_H__

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#define L 1 // login
#define Q 2 // quit
#define R 3 // retrun
#define I 4 // insert
#define D 5 // delete
#define M 6 // modify
#define S 7 // seek

enum POWER {

    NORMAL,
    ROOT
};



typedef struct {
    char type;
    char job_num[7];//工号
    char password[7];//密码
    char name[20];//名字
    char identity;//权限
    int sale;//销售额
    int salary;//薪资
    char data[56]; // other data
} MSG;


void do_client(int fd, sqlite3* db);//处理传递信息
void do_login(int fd, MSG* msg, sqlite3* db);//登录
// void do_return(int fd,MSG* msg,sqlite3 *db);//返回
void do_inster(int fd, MSG* msg, sqlite3* db);//插入
void do_delete(int fd, MSG* msg, sqlite3* db);//删除
void do_modify(int fd, MSG* msg, sqlite3* db);//修改
void do_seek(int fd, MSG* msg, sqlite3* db);//查询
void do_quit(int fd, MSG* msg, sqlite3* db);//退出

#endif