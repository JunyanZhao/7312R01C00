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

#define PORT 1025
#define MAXDATASIZE 5000
using namespace std;
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World! The client is starting! \n";
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
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY); //#include <arpa/inet.h>
    //srvaddr.sin_addr.s_addr = INADDR_ANY;
    if (connect(sockfd, (const struct sockaddr *)&srvaddr, sizeof(struct sockaddr)) == -1) {
        perror("connect error");
        exit(1);
    }
    
    cout<<"connect to server"<<endl;
    
    unsigned int len = 0;
    /*
    while (1) {
        if ((len=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
            perror("recv error");
            continue;
        }
        buf[len] = '\0';
        cout<<buf<<endl;
    }
    */
    
    /*循环的发送接收信息并打印接收信息（可以按需发送）--recv返回接收到的字节数，send返回发送的字节数*/
    while(1)
    {
        printf("Enter string to send:");
        //memset_s(buf, 1024, 0, 1024);
        string str;
        getline(cin, str);
        if(str == "quit")
        {
            cout<<"send close"<<endl;
            len=send(sockfd,str.c_str(),str.size(),0);
            break;
        }
        if (str.size() == 0) {
            continue;
        }
        cout<<"sending..."<<endl;
           len=send(sockfd,str.c_str(),str.size(),0);
           //len=recv(sockfd,buf,5000,0);
           //buf[len]='/0';
           //printf("received:%s/n",buf);
        cout<<"sending success..."<<endl;
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
