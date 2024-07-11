#include "../include/fungrp.h"

int flag = 0; // 用户权限 ROOT/USER
MSG msg1;     // 用户数据
/*
    @brief 连接服务器
    @param
        @fd:用户文件描述符
    @retyal 无
*/
void do_server(int fd)
{
    MSG msg;
    int choose = 0;
    printf("欢迎使用销售人员管理系统!!\n\n");
    while (1) {
        printf("**********************\n");
        printf("* 1: login   2: quit *\n");
        printf("**********************\n");
        printf("please choose : ");
        scanf("%d", &choose);
        switch (choose) {
        case 1:
            do_login(fd, &msg);
            break;
        case 2:
            exit(1);
            break;
        default:
            system("clear");
            printf("请输入正确的选项!\n\n");
            break;
        }
    }
}
/*
    @brief 用户登录
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
    @retyal 无
*/
void do_login(int fd, MSG* msg)
{
    int choose = 0;
    msg->type = L;
    printf("请输入工号:");
    scanf("%s", msg->job_num);
    getchar();
    printf("请输入密码:");
    scanf("%s", msg->password);
    getchar();
    // 发送数据
    send(fd, msg, sizeof(MSG), 0);
    // 接收数据
    recv(fd, msg, sizeof(MSG), 0);
    // 判断登录成功
    if (strcmp(msg->data, "login ok") == 0) {
        // 登录成功
        system("clear");
        printf("\n登录成功!\n\n");
        sleep(2);
        system("clear");
        msg1 = *msg;
        if (msg->identity == ROOT)
            flag = 1;
        while (1) {
            printf("****************************************************\n");
            printf("* 1: 查询数据 2:增加数据 3:删除数据 4:修改数据 5:退出 *\n");
            printf("****************************************************\n");
            printf("please choose : ");
            scanf("%d", &choose);
            switch (choose) {
            case 1:
                do_seek(fd, msg);
                break;
            case 2:
                do_inster(fd, msg);
                break;
            case 3:
                do_delete(fd, msg);
                break;
            case 4:
                do_modify(fd, msg);
                break;
            case 5:
                do_quit(fd, msg);
                break;
            default:
                system("clear");
                printf("请输入正确的选项!\n\n");
                sleep(2);
                system("clear");
                break;
            }
        }
    } else {
        // 登录失败
        printf("\n%s\n\n", msg->data);
        do_server(fd);
    }
}
/*
    @brief 查询用户
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
    @retyal 无
*/
void do_seek(int fd, MSG* msg)
{
    msg->type = S;
    // 普通用户只能查看自己的信息
    if (msg1.identity != ROOT) {
        printf("姓名:%s\n", msg1.name);
        printf("工号:%s\n", msg1.job_num);
        printf("销售额:%d\n", msg1.sale);
        printf("薪资:%d\n\n\n", msg1.salary);
    }
    // ROOT用户可以查询所有用户信息
    else {
        printf("请输入要查询的工号:");
        scanf("%s", msg->job_num);
        if (-1 == send(fd, msg, sizeof(MSG), 0)) {
            printf("send error\n");
        }
        if (-1 == recv(fd, msg, sizeof(MSG), 0)) {
            printf("recv error\n");
        }
        if(0==(strcmp(msg->data,"seek ok"))){
            printf("姓名:%s\n", msg->name);
            printf("工号:%s\n", msg->job_num);
            printf("销售额:%d\n", msg->sale);
            printf("薪资:%d\n\n\n", msg->salary);
        }
        else
        {
            printf("\n%s\n\n",msg->data);
        }
        
    }
    return;
}
/*
    @brief 插入新用户
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
    @retyal 无
*/
void do_inster(int fd, MSG* msg)
{
    int choose;
    msg->type = I;
    if (flag != 1) {
        printf("权限不足,无法插入!!!\n\n\n");
        return;
    }
    // 输入插入成员信息
    printf("请输入新名字:");
    scanf("%s", msg->name);
    printf("请输入工号:");
    scanf("%s", msg->job_num);
    printf("初始密码为:123456,是否要修改?");
    printf("******************************************\n");
    printf("********1.Yes   2.No**********************\n");
    printf("******************************************\n");
    printf("您的选择(1/2):");
    scanf("%d", &choose);
    if (1 == choose) {
        printf("请输入密码:");
        scanf("%s", msg->password);
    } else {
        strcpy(msg->password, "123456");
    }
    choose = 0;
    printf("是否为管理员(1.Yes/2.No):");
    scanf("%d", &choose);
    if (1 == choose)
        msg->identity = ROOT;
    else
        msg->identity = NORMAL;
    printf("请输入薪资:");
    scanf("%d", &msg->salary);
    choose = 0;
    printf("是否有业绩(1.Yes/2.No):");
    scanf("%d", &choose);
    if (1 == choose) {
        printf("请输入销售额:");
        scanf("%d", &msg->sale);
    } else {
        msg->sale = 0;
    }
    // 发送用户数据
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
        printf("send error\n");
        return;
    }
    if (-1 == recv(fd, msg, sizeof(MSG), 0)) {
        printf("recv error\n");
        return;
    }
    printf("%s!!!!\n\n\n", msg->data);
}
/*
    @brief 删除用户
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
    @retyal 无
*/
void do_delete(int fd, MSG* msg)
{
    int choose;
    msg->type = D;
    if (flag != 1) {
        printf("权限不足,无法删除!!!\n\n\n");
        return;
    }
    printf("请输入要删除的工号:");
    scanf("%s", msg->job_num);
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
        printf("send error\n");
        return;
    }
    if (-1 == recv(fd, msg, sizeof(MSG), 0)) {
        printf("recv error\n");
        return;
    }
    printf("\n%s!!!!\n\n\n\n", msg->data);
}
/*
    @brief 修改用户信息
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
    @retyal 无
*/
void do_modify(int fd, MSG* msg)
{
    int choose;
    msg->type = M;
    // 非ROOT用户只能修改自己的密码
    if (flag != 1) {
        printf("当前用户只能修改密码,是否修改\n");
        printf("*******************************************************\n");
        printf("*********************1.Yes   2.No**********************\n");
        printf("*******************************************************\n");
        printf("您的选择(1/2):");
        scanf("%d", &choose);
        if (1 == choose) {
            printf("请输入密码:");
            scanf("%s", msg->password);
            msg->data[0] = '3';
        } else
            return;
    }
    // ROOT用户可以修改任意人的所有信息
    else {
        printf("请输入要修改的工号:");
        scanf("%s", msg->job_num);
        printf("请输入要修改的属性:");
        printf("***********************************************************************\n");
        printf("********1.名字   2.工号   3.密码   4.薪资   5.业绩   6.权限**************\n");
        printf("***********************************************************************\n");
        printf("请输入选择(1/2/3/4/5/6):");
        scanf("%d", &choose);
        switch (choose) {
        case 1:
            printf("请输入名字:");
            scanf("%s", msg->name);
            msg->data[0] = '1';
            break;
        case 2:
            printf("请输入工号:");
            scanf("%s", msg->data);
            msg->data[0] = '2';
            break;
        case 3:
            printf("请输入密码:");
            scanf("%s", msg->password);
            msg->data[0] = '3';
            break;
        case 4:
            printf("请输入薪资:");
            scanf("%d", &msg->salary);
            msg->data[0] = '4';
            break;
        case 5:
            printf("请输入销售额:");
            scanf("%d", &msg->sale);
            msg->data[0] = '5';
            break;
        case 6:
            printf("请输入权限:");
            msg->identity = !msg->identity;
            msg->data[0] = '6';
            break;
        }
    }
    //发送要修改的信息
    if (-1 == send(fd, msg, sizeof(MSG), 0)) {
        printf("send error\n");
        return;
    }
    if (-1 == recv(fd, msg, sizeof(MSG), 0)) {
        printf("recv error\n");
        return;
    }
    printf("%s!!!!\n\n\n", msg->data);
    return;
}
/*
    @brief 用户退出
    @param
        @fd:用户文件描述符
        @msg:用户信息结构体
    @retyal 无
*/
void do_quit(int fd, MSG* msg)
{
    msg1.type = Q;
    flag = 0;
    if (-1 == send(fd, &msg1, sizeof(MSG), 0)) {
        printf("send error\n");
        return;
    }
    if (-1 == recv(fd, msg, sizeof(MSG), 0)) {
        printf("recv error\n");
        return;
    }
    system("clear");
    printf("%s!!!!\n\n\n", msg->data);
    system("clear");
    do_server(fd);
    return;
}