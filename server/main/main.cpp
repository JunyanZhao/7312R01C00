//
//  main.cpp
//  R01C00
//
//  Created by 赵军岩 on 2017/3/12.
//  Copyright © 2017年 赵军岩. All rights reserved.
//

#include <iostream>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <stdio.h>
using namespace std;

#define MYPORT 3490 //开放的端口号
#define MAXNUM 5    //最大请求数目
unsigned long g_sockfd[1024] ={0};
//msg=16b(send)+16(recv)+buf
void *recvMsg(void *arg)
{
    int sockfd = (int)(*(int*)arg);
    cout<<"recv msg, sockfd: "<<sockfd<<endl;
    printf("reveiving msg: \n");
    char buf[1024];
    ssize_t recvMsgLen = 0;
    size_t index=0;
    char ID[16];
    unsigned long sendToUserID = 0;
    while (1) {
        if ((recvMsgLen = recv(sockfd, buf, 1024, 0)) <= 0) {
            cout<<"close socket"<<endl;
            close(sockfd);
            perror("recv error!");
            break;
        }
        buf[recvMsgLen] = '\0';
        if (0==recvMsgLen) {
            cout<<"len=0"<<endl;
            continue;
        }
        cout<<"recv: "<<buf<<endl;
        for (index=0; index<16; ++index) {
            if (buf[index] != ':') {
                ID[index] = buf[index];
            }
        }
        ID[index] = '\0';
        sscanf(ID, "%ld", &sendToUserID);
        
        
        if (send(g_sockfd[sendToUserID], buf+16, 1024, 0) == -1) {
            perror("write error");
        }
        if (strcmp(buf+16, "quit") == 0) {
            cout<<"close socket"<<endl;
            break;
        }
    }
    return NULL;
}
/*
void *sendMsg(void* arg)
{
    int sockfd = (int)(*(int*)arg);
    ssize_t res = 0;
    while (1) {
        cout<<"begin send msg"<<endl;
        string str;
        cin>>str;
        if (str.size() > 0) {
            res = send(sockfd, str.c_str(), str.size(), 0);
            if (-1 == res) {
                perror("send error");
                close(sockfd);
                break;
            }
        }
    }
    return NULL;
}
*/
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World! The server is starting! \n";
    int sockfd;
    struct sockaddr_in srvaddr;
    bzero(&srvaddr, sizeof(struct sockaddr_in));
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port=htons(1025);
    srvaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    int ret = bind(sockfd, (const struct sockaddr*)&srvaddr, (uint32_t)sizeof(struct sockaddr));
    ret = listen(sockfd, MAXNUM);
    struct sockaddr_in clientaddr;
    int new_sockfd;
    int sin_size = sizeof(struct sockaddr_in);
    ssize_t recvMsgLen = 0;
    char buf[64]={0};
    unsigned long userID = 0;
    while (1)
    {
        if ((new_sockfd=accept(sockfd, (struct sockaddr *)&clientaddr, (unsigned int *)&sin_size))==-1) {
            perror("accept error");
            exit(1);
        }
        recvMsgLen = recv(new_sockfd, buf, 64, 0);
        if (-1 == recvMsgLen) {
            perror("recv error");
            close(new_sockfd);
            continue;
        }
        if (0 == recvMsgLen) {
            continue;
        }
        //判断是buf是整数
        sscanf(buf, "%ld", &userID);
        cout<<"userID: "<<userID<<endl;

        g_sockfd[userID] = new_sockfd;
        pid_t revMsgPid;
        pthread_t revMsgThreadID;
        if ((pthread_create(&revMsgThreadID, NULL, recvMsg, &new_sockfd)) != 0) {
            perror("create thread failed");
        }
    }
    
    return 0;
}
