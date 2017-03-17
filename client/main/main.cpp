//
//  main.cpp
//  R01C00
//
//  Created by 赵军岩 on 2017/3/12.
//  Copyright © 2017年 赵军岩. All rights reserved.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include "../CParseXML/CParseXML.hpp"

#define PORT 1025
#define MAXDATASIZE 5000
using namespace std;
long talkFriendID = 0;
string strID;
string strDisplayMsg;
void* recvMsg(void* arg)
{
    int sockfd = (int)(*(int*)arg);
    //cout<<"recv from sockfd: "<<sockfd<<endl;
    ssize_t recvMsgLen = 0;
    char buf[1024] = {0};
    while (1) {
        recvMsgLen = recv(sockfd, buf, 1024, 0);
        if (-1 == recvMsgLen) {
            perror("recv error");
            close(sockfd);
        }
        cout<<"massge: "<<buf<<endl;
        cout<<strDisplayMsg<<endl;
    }
    
    return NULL;
}

void* sendMsg(void* arg)
{
    int sockfd = (int)(*(int*)arg);
    ssize_t res = 0;
    char buf[1024] = {0};
    string str;
    string strMsg;
    string strSentToID;
    unsigned long SendToID = 0;
    unsigned int index = 0;
    while (1) {
        cout<<"input friend ID to send:"<<endl;
        strDisplayMsg.assign("input friend ID to send:");
        getline(cin, strSentToID);
        if (strSentToID.size()>=16) {
            continue;
        }
        sscanf(strSentToID.c_str(), "%ld", &SendToID);
        index = 0;
        while (strSentToID[index] != '\0') {
            buf[index] = strSentToID[index];
            ++index;
        }
        buf[index++] = ':';
        for (; index<16; ++index) {
            buf[index] = '>';
        }
        for (index=16; index<16+strID.size(); ++index) {
            buf[index] = strID[index-16];
        }
        buf[index++] = ':';
        for (; index<16+16; ++index) {
            buf[index] = '>';
        }
        cout<<"input msg to send:"<<endl;
        strDisplayMsg.assign("input msg to send:");
        getline(cin, strMsg);
        for (index=32; index<32+strMsg.size(); ++index) {
            buf[index] = strMsg[index-32];
        }
        buf[index] = '\0';
        cout<<"send: "<<buf<<endl;
        res = send(sockfd, buf, 1024, 0);
        if (-1 == res) {
            perror("send error");
        }
    }
    
    return NULL;
}

int main(int argc, const char * argv[]) {
    CParseXML cparse;
    cparse.openXMLFile();
    std::cout << "Hello, World! The client is starting! \n";
    cout<<"input your ID"<<endl;
    getline(cin, strID);
    int sockfd;
    char buf[1024];
    struct sockaddr_in srvaddr;
    //struct hostent *he;  //#include <netdb.h>
    
    if ((sockfd=socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Create socket error");
        exit(1);
        
    }
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port=htons(1025);
    srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //#include <arpa/inet.h>
    if ((in_addr_t)-1 == srvaddr.sin_addr.s_addr) {
        perror("IP error");
        exit(1);
    }
    //srvaddr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (const struct sockaddr *)&srvaddr, sizeof(struct sockaddr)) == -1) {
        perror("connect error");
        exit(1);
    }
    
    cout<<"connect to server"<<endl;
    
    ssize_t len = 0;
    ssize_t res = 0;
    res = send(sockfd, strID.c_str(), strID.size(), 0);
    if (-1 == res)
    {
        while (res != -1)
        {
            //perror("send error");
            cout<<"please retry"<<endl;
            cin>>strID;
            res = send(sockfd, strID.c_str(), strID.size(), 0);
        }
    }
    //pid_t pid;
    pthread_t threadID;
    if (pthread_create(&threadID, NULL, recvMsg, &sockfd) != 0) {
        perror("create thread error");
    }
    if (pthread_create(&threadID, NULL, sendMsg, &sockfd) != 0) {
        perror("create thread error");
    }
    while (1) {
        sleep(60);
        continue;
    }
    
    /*关闭套接字*/
    len = recv(sockfd, buf, 1024, 0);
    buf[len] = '\0';
    if(!strcmp(buf,"quit")) {
        cout<<"ready close"<<endl;
        close(sockfd);  //#include <unistd.h>
    }
    
    
    return 0;
}
