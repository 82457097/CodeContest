/*Copyright [2020.11.04] <qianwanjun>*/
#ifndef SOCKET_H_
#define SOCKET_H_

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<memory.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<vector>

#define BUFFER_SIZE 1024
#define PORT 9527
#define MAX_CONNECT 20
#define IP_ADDR "10.0.28.187"
#define ERR_RET -1
#define ERR_ZERO 0
#define KEY "(afcf4c2d11cb4cbbbb0825d2f85174a0)"

#endif  // SOCKET_H_

using std::cout;
using std::cin;
using std::endl;
using std::string;

class Socket {
public:
    Socket();
    ~Socket() {}

    bool SockInitClient(int port, char* ipAddr);
    bool SockConnect();
    int SockRecv(int recvfd, char buf[], int len);
    int SockSend(int sendfd, const char buf[], int len);
    bool SockClose();

    int sockfd;
    int recvLen;
    int sendLen;
    int m_port;
    char* m_ipAddr;

private:
    struct sockaddr_in m_sockaddr;
};

Socket::Socket() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < ERR_ZERO) {
        cout << "socket build failed." << endl;
    } else {
        cout << "socket build success." << endl;
    }
}

bool Socket::SockInitClient(int port, char* ipAddr) {
    m_port = port;
    m_ipAddr = ipAddr;
    memset(&m_sockaddr, 0, sizeof(m_sockaddr));
    m_sockaddr.sin_family = AF_INET;
    m_sockaddr.sin_port = htons(m_port);
    m_sockaddr.sin_addr.s_addr = inet_addr(m_ipAddr);

    return true;
}

bool Socket::SockConnect() {
    if ((connect(sockfd, (struct sockaddr*)&m_sockaddr,
        sizeof(m_sockaddr))) == ERR_RET) {
        return false;
    }

    return true;
}

int Socket::SockRecv(int recvfd, char buf[], int len) {
    memset(buf, 0, BUFFER_SIZE);
    recvLen = recv(recvfd, buf, len, 0);

    return recvLen;
}

int Socket::SockSend(int sendfd, const char buf[], int len) {
    sendLen = send(sendfd, buf, len, 0);

    return sendLen;
}


bool Socket::SockClose() {
    close(sockfd);
    cout << "close success." << endl;

    return true;
}
