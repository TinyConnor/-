#include "../include/mysystem.h"

#define MY_IP "192.168.100.10"
#define MY_PORT 36777
// #define MY_IP "127.0.0.1"
// #define MY_PORT 8888


#define DATABASE "./Database/mydatabase.db"//数据库文件
#define MAXSIZE 256//最大监控合数

int epfd;

int main(int argc, char const* argv[])
{
    // 判断传参是否正确
    // if (argc != 3) {
    //     printf("input error!\n");
    //     printf("usage: %s <ip> <port>\n", argv[0]);
    //     exit(-1);
    // }
    sqlite3* db = NULL; // sqlite3句柄指针
    // 打开数据库
    if (SQLITE_OK != sqlite3_open(DATABASE, &db)) {
        printf("error: %s\n", sqlite3_errmsg(db));
        exit(-1);
    }
    // 创建套接字
    int sockfd = 0;
    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("SOCKET FAIL!");
        exit(-1);
    }
    // 填充网络信息结构体
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = inet_addr(argv[1]); // 网络字节序端口号
    // server_addr.sin_port = htons(atoi(argv[2]));      // 网络地址
    server_addr.sin_addr.s_addr = inet_addr(MY_IP); // 网络字节序端口号
    server_addr.sin_port = htons(MY_PORT);      // 网络地址

    // 套接字与网络信息结构体绑定
    if (-1 == bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr))) {
        perror("BIND FAIL");
        exit(-1);
    }

    // 将套接字设置为被动监听状态
    if (-1 == listen(sockfd, 5)) {
        perror("LISTEN FAIL");
        exit(-1);
    }

    // epoll
    struct epoll_event events[MAXSIZE];
    memset(events, 0, sizeof(events));
    epfd = epoll_create(MAXSIZE); // 创建epoll
    if (epfd < 0) {
        printf("epoll create fail\n");
        exit(-1);
    }
    // 为sockfd填充epoll结构体
    struct epoll_event ev;
    ev.data.fd = sockfd;
    // ev.events = EPOLLIN|EPOLLET; // 监视sockfd可读
    ev.events = EPOLLIN;
    int ret = 0;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    if (ret < 0) {
        perror("fail to epoll_ctl");
        exit(1);
    }
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);
    int num = 0; // 监听就绪fd个数
    int i = 0;
    int fdc = 0; // 新连接的fd

    while (1) {
        num = epoll_wait(epfd, events, MAXSIZE, -1); //-1阻塞
        if (num < 0) {
            perror("fail to epoll_wait");
            exit(1);
        }
        for (i = 0; i < num; i++) {
            // 新用户连接
            if (events[i].data.fd == sockfd) {
                if (-1 == (fdc = accept(sockfd, (struct sockaddr*) &client_addr, &client_len))) {
                    perror("accept error");
                    exit(1);
                }
                printf("Acceptrd connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                // 将新连接的client_fd并加入epoll事件中
                ev.events = EPOLLIN;
                ev.data.fd = fdc;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, fdc, &ev) == -1) {
                    perror("epoll_ctl error");
                    exit(1);
                }
            }
            // 收发数据
            else {
                do_client(events[i].data.fd, db);
            }
        }
    }

    close(sockfd);
    close(epfd);
    return 0;
}
