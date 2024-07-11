#include "../include/fungrp.h"
#define MY_IP "192.168.5.11"
// #define MY_IP "192.168.250.100"
#define MY_PORT 36777
// #define MY_IP "127.0.0.1"
// #define MY_PORT 8888

int main(int argc, char const* argv[])
{
    //入参检查
    // if (argc < 3) {
    //     printf("usage error: %s <server_ip> <server_port>\n", argv[0]);
    //     exit(1);
    // }
    //创建套接字
    int sockfd = 0;
    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("fail to socket");
        exit(1);
    }
    //填充服务器网络信息结构体
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(MY_IP);
    server_addr.sin_port = htons(MY_PORT);

    //与服务器建立连接
    if (-1 == connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr))) {
        perror("fail to connect");
        exit(-1);
    }
    while (1) {
        do_server(sockfd);
    }
    return 0;
}
