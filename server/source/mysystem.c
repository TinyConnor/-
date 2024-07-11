#include "../include/mysystem.h"
MSG msg1;
/*
    @brief 客户端连接诶
    @param
        @fd:用户文件描述符
        @db:数据库句柄
    @retyal 无
*/
void do_client(int fd, sqlite3* db)
{
    MSG msg;
    int ret = 1;
    memset(&msg, 0, sizeof(msg));
    ret = recv(fd, &msg, sizeof(msg), 0);
    msg1 = msg;
    if (-1 == ret) {
        perror("recv");
    } else if (ret == 0) {
        printf("_________________异常退出_____________________\n");
        do_quit(fd, &msg, db);
    }

    switch (msg.type) {
    case L: // 登录
        do_login(fd, &msg, db);
        break;
    case Q: // 退出
        do_quit(fd, &msg, db);
        break;
    case I: // 插入
        do_inster(fd, &msg, db);
        break;
    case D: // 删除
        do_delete(fd, &msg, db);
        break;
    case M: // 修改
        do_modify(fd, &msg, db);
        break;
    case S: // 查询
        do_seek(fd, &msg, db);
        break;
    default:
        break;
    }
}
/*
    @brief 用户登录
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
        @db:数据库句柄
    @retyal 无
*/
void do_login(int fd, MSG* msg, sqlite3* db)
{
    char sqlstr[256] = { 0 };
    char* errmsg = NULL, ** result = NULL;
    int nrow = 0, ncolumn = 0;
    sprintf(sqlstr, "select * from employee where number=%s and passwd=%s", msg->job_num, msg->password);

    if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg)) {
        printf("error:%s\n", errmsg);
    }
    sqlite3_free(errmsg);

    // 通过nrow判断是否查询的到，如果值为0，就是查询不到，如果值不为0，就是查到了
    if (nrow == 0) {
        strcpy(msg->data, "name or password is wrong!");
    } else {
        // 是否在线
        if (0 != atoi(result[ncolumn + 6])) {
            strcpy(msg->data, "user is already online!");
        } else {
            strcpy(msg->data, "login ok");
            strcpy(msg->name, result[ncolumn]);
            strcpy(msg->job_num, result[ncolumn + 2]);
            msg->salary = atoi(result[ncolumn + 3]);
            msg->sale = atoi(result[ncolumn + 4]);
            if (strcmp("root", result[ncolumn + 1]) == 0)
                msg->identity = ROOT;
            else
                msg->identity = NORMAL;
            strcpy(msg->password, result[ncolumn + 5]);
            sprintf(sqlstr, "update employee set flag=%d where number=%s", fd, msg->job_num);

            if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg)) {
                printf("error:%s\n", errmsg);
            }
            sqlite3_free(errmsg);
        }
    }
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
        printf("send error\n");
    } else {
        // printf("%d号机登录\n", fd - 5);
        printf("%d号机%s\n", fd - 5, msg->data);
    }
}
/*
    @brief 插入新用户信息
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
        @db:数据库句柄
    @retyal 无
*/
void do_inster(int fd, MSG* msg, sqlite3* db)
{
    char sqlstr[256] = { 0 };
    char** result = NULL;
    sprintf(sqlstr, "insert into employee values('%s','%s',%s,%d,%d,%s,0)",
        msg->name, ((msg->identity == ROOT) ? "root" : "user"), msg->job_num, msg->salary, msg->sale, msg->password);
    if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, NULL, NULL, NULL)) {
        printf("inster error\n");
        strcpy(msg->data, "insert error!");
    } else {
        strcpy(msg->data, "inster ok!");
    }
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
        printf("send error\n");
    }
    return;
}
/*
    @brief 删除用户信息
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
        @db:数据库句柄
    @retyal 无
*/
void do_delete(int fd, MSG* msg, sqlite3* db)
{
    char sqlstr[256] = { 0 };
    char** result = NULL;
    sprintf(sqlstr, "delete from employee where number=%s", msg->job_num);
    if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, NULL, NULL, NULL)) {
        printf("delete error\n");
        strcpy(msg->data, "no worker!");
    } else {
        strcpy(msg->data, "delete ok!");
    }
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
    }
    return;
}
/*
    @brief 修改用户信息
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
        @db:数据库句柄
    @retyal 无
*/
void do_modify(int fd, MSG* msg, sqlite3* db)
{
    char sqlstr[256] = { 0 };
    char** result = NULL;
    // 1.名字   2.工号   3.密码   4.薪资   5.业绩   6.权限
    switch (msg->data[0]) {
    case '1':
        sprintf(sqlstr, "update employee set name='%s' where number=%s", msg->name, msg->job_num);
        break;
    case '2':
        sprintf(sqlstr, "update employee set number=%s where number=%s", msg->data, msg->job_num);
        break;
    case '3':
        sprintf(sqlstr, "update employee set passwd=%s where number=%s", msg->password, msg->job_num);
        break;
    case '4':
        sprintf(sqlstr, "update employee set salary=%d where number=%s", msg->salary, msg->job_num);
        break;
    case '5':
        sprintf(sqlstr, "update employee set sale=%d where number=%s", msg->sale, msg->job_num);
        break;
    case '6':
        sprintf(sqlstr, "update employee set identity='%s' where number=%s", (msg->identity == ROOT ? "user" : "root"), msg->job_num);
        break;
    }

    if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, NULL, NULL, NULL)) {
        printf("modify error\n");
        strcpy(msg->data, "no worker!");
    } else {
        strcpy(msg->data, "modify ok!");
    }
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
        printf("send error\n");
    }
}
/*
    @brief 查询用户信息
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
        @db:数据库句柄
    @retyal 无
*/
void do_seek(int fd, MSG* msg, sqlite3* db)
{
    char sqlstr[256] = { 0 };
    char* errmsg = NULL, ** result = NULL;
    int nrow = 0, ncolumn = 0;
    sprintf(sqlstr, "select * from employee where number=%s", msg->job_num);

    if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg)) {
        printf("error:%s\n", errmsg);
    }
    sqlite3_free(errmsg);
    if (nrow == 0) {
        strcpy(msg->data, "no worker!");
    } else {
        strcpy(msg->data, "seek ok");
        strcpy(msg->name, result[ncolumn]);
        strcpy(msg->job_num, result[ncolumn + 2]);
        msg->salary = atoi(result[ncolumn + 3]);
        msg->sale = atoi(result[ncolumn + 4]);
        if (strcmp("root", result[ncolumn + 1]) == 0)
            msg->identity = ROOT;
        else
            msg->identity = NORMAL;
        strcpy(msg->password, result[ncolumn + 5]);
    }
    send(fd, msg, sizeof(MSG), 0);
}
/*
    @brief 用户断开服务器连接,清空标志位
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
        @db:数据库句柄
    @retyal 无
*/
void do_quit(int fd, MSG* msg, sqlite3* db)
{

    char sqlstr[256] = { 0 };
    char** result = NULL, * errmsg = NULL;
    int nrow = 0, ncolumn = 0;
    sprintf(sqlstr, "select number from employee where flag=%d", fd);
    printf("%s\n", sqlstr);
    if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg)) {
        printf("quit error2\n");
        printf("error:%s\n", errmsg);
    }
    sqlite3_free(errmsg);
    if(nrow!=0){
        strcpy(msg1.job_num, result[1]);
        memset(sqlstr, 0, sizeof(sqlstr));
        sprintf(sqlstr, "update employee set flag=0 where number=%s", msg1.job_num);
        printf("%s\n", sqlstr);
        if (SQLITE_OK != sqlite3_get_table(db, sqlstr, &result, NULL, NULL, NULL)) {
            printf("quit error2\n");
        }
    }
    
    extern int epfd;
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == 0) {
        strcpy(msg->data, "quit ok");
    } else {
        perror("epoll_ctl");
        strcpy(msg->data, "quit error");
    }
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
        printf("send error\n");
    } else {
        printf("%d号机已退出\n", fd - 5);
    }
    close(fd);
}