#ifndef __FUNGRP_H__
#define __FUNGRP_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define L 1 // login
#define Q 2 // quit
// #define R 3 // retrun
#define I 4 // insert
#define D 5 // delete
#define M 6 // modify
#define S 7 // seek
//user power
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


void do_server(int fd);//连接服务器
void do_login(int fd, MSG* msg);//登录
void do_seek(int fd, MSG* msg);//查询
void do_inster(int fd, MSG* msg);//插入
void do_delete(int fd, MSG* msg);//删除
void do_modify(int fd, MSG* msg);//修改
// void do_return(int fd,MSG* msg);
void do_quit(int fd, MSG* msg);//退出


#endif