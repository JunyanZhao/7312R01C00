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
using namespace std;

#define MYPORT 3490 //开放的端口号
#define MAXNUM 5    //最大请求数目

void *recvMsgFun(void *arg)
{
    int sockfd = (int)(*(int*)arg);
    cout<<"recv msg, sockfd: "<<sockfd<<endl;
    printf("reveiving msg: \n");
    char buf[1024];
    unsigned int len = 0;
    while (1) {
        if ((len = recv(sockfd, buf, 1024, 0)) == -1) {
            perror("recv error!");
            continue;
        }
        buf[len] = '\0';
        if (0==len) {
            cout<<"len=0"<<endl;
            continue;
        }
        cout<<"recv: "<<buf<<endl;
        char *msg = "msg received!";
        if (send(sockfd, msg, strlen(msg), 0) == -1) {
            perror("write error");
        }
        if (strcmp(buf, "quit") == 0) {
            cout<<"close socket"<<endl;
            break;
        }

    }
    return NULL;
}

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
    struct sockaddr_in cliaddr;
    int new_fd;
    while (1) {
        int sin_size = sizeof(struct sockaddr_in);
        if ((new_fd=accept(sockfd, (struct sockaddr *)&cliaddr, (unsigned int *)&sin_size))==-1) {
            perror("accept error");
            exit(1);
        }
        pid_t revMsgPid;
        pthread_t revMsgThreadID;
        if ((pthread_create(&revMsgThreadID, NULL, recvMsgFun, &new_fd)) != 0) {
            perror("create thread failed");
        }
        continue;
        //int ret = accept(sockfd, (struct sockaddr*)&cliaddr, (unsigned int *)&sizeof(struct sockaddr_in));
        //int new_fd=accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t *)&sizeof(struct sockaddr_in));
        //int	accept(int, struct sockaddr * __restrict, socklen_t * __restrict)
        char buf[1024];
        unsigned int len = 0;
        while (1) {
            len = recv(new_fd, buf, 1024, 0);
            if (0 == len) {
                cout<<"len = 0"<<endl;
                close(sockfd);
                break;
            }
            buf[len] = '\0';
            cout<<buf<<endl;
            if(!strcmp(buf,"quit")){
                cout<<"ready close"<<endl;
                char* msg = "quit";
                send(new_fd, msg, 5, 0);
                close(sockfd);
                break;
            }
        }

    }
    

#if 0
    int sockfd, new_fd;
    struct sockaddr_in srvaddr; /* 本地的地址信息 */
    struct sockaddr_in cliaddr; /* 连接者的地址信息 */
    int sin_size;
    
    //创建套接字描述符
    if (-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("socket error");
        exit(1);
    }
    
    bzero(&srvaddr, sizeof(srvaddr)); /* 将结构中未使用部分全部清零 */
    //用自己的IP地址和端口填充一个Internet套接字地址
    
    
    
    /*函数bind将服务器的地址和套接字绑定在一起*/
    for (unsigned short sPort=1025; sPort<0xffff; ++sPort)
    {
        if(bind(sockfd,(const struct sockaddr *)&srvaddr,(unsigned int)sizeof(struct sockaddr))>=0)
        {
            cout<<"port: "<<sPort<<endl;
            srvaddr.sin_family=AF_INET;  /* 主机字节顺序 */
            srvaddr.sin_addr.s_addr=INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
            srvaddr.sin_port=htons(sPort); /* 网络字节顺序，短整型 */
            break;
        }
       
    }
    
    /*listen函数告诉内核，这个套接字可以接受来自客户机的请求*/
    if (listen(sockfd, MAXNUM) == -1) {
        perror("accept error");
        exit(1);
    }
    
    cout<<"Server is running and accepting request!"<<endl;
    
    
    while (1) {
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd=accept(sockfd, (struct sockaddr *)&cliaddr, (unsigned int *)&sin_size))==-1) {
            perror("accept error");
            continue;
        }
        cout<<"server: got connection from "<<inet_ntoa(cliaddr.sin_addr)<<endl; //#include <arpa/inet.h>
        char *msg = "Hello! World!";
        if (send(new_fd, msg, strlen(msg), 0) == -1) {
            perror("write error");
        }
        /*
        pid_t revMsgPid;
        pthread_t revMsgThreadID;
        if ((pthread_create(&revMsgThreadID, NULL, recvMsgFun, &sockfd)) != 0) {
            perror("create thread failed");
        }
        
        revMsgPid = getpid();  //#include <unistd.h>
        revMsgThreadID = pthread_self();
        cout<<"pid: "<<revMsgPid<<", ThreadID: "<<revMsgThreadID<<endl;
        //int send(int sockfd, const void *msg, int len, int flags)
         */
        char buf[1024];
        while (1) {
            recv(new_fd, buf, 1024, 0);
            cout<<buf<<endl;
        }
        string quitBuf;
        cin>>quitBuf;
        if (quitBuf == "quit") {
            close(sockfd);
        }
    }
#endif
    return 0;
}
