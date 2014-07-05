#include "Xclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/shm.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <time.h>
#include <string>
#include <iostream>
using std::endl;
using std::cin;
using std::cout;
using std::string;

int main() {
	// 定义地址结构
	sockaddr_in clientaddr;
	pid_t pid;
	// 定义客户端套接字
	int clientfd, port;
	hostent *host;
	char buff[1000], IP[20];
	char user[15], pw[15];
	printf("输入局域网服务器IP\n");
	scanf("%s", IP);
	printf("输入端口\n");
	scanf("%d", &port);
	printf("输入用户名\n");
	scanf("%s", user);
	printf("输入密码\n");
	scanf("%s", pw);
	host = gethostbyname(IP);

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket\n");
		exit(1);
	}

	// 绑定客户端套接字
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(port);
	clientaddr.sin_addr = *((in_addr*)host->h_addr);
	bzero(&(clientaddr.sin_zero), 0);

	if (connect(clientfd, (sockaddr*)&clientaddr, sizeof(sockaddr)) == -1) {
		// 连接服务器
		perror("connect");
		exit(1);
	}

	memset(buff, 0, sizeof(buff));
	strncat(buff, user, strlen(user));
	strncat(buff, "::", 2);
	strncat(buff, pw, strlen(pw));

  if ((send(clientfd, buff, strlen(buff), 0)) == -1) {
	  perror("send\n");
	  exit(1);
  }

	if (recv(clientfd, buff, 1000, 0) == -1) {
		perror("recv");
		exit(1);
	}

	if (!strcmp(buff, "[error] log in fail!\n")) {
	  printf("用户名或密码错误\n");
	  return 0;
	} else {
	  printf("%s", buff);
	}
	// 创建子进程
	pid = fork();
	while (1) {
		if (pid > 0) {
			// 父进程用于发送消息
			memset(buff, 0, sizeof(buff));
			scanf("%s", buff);
			string buff_r = sendControl(buff);
			buff_r += '*';
			buff_r += string(user);
	    // strncat(buff, "*", 1);
			// strncat(buff, user, strlen(user));
			if ((send(clientfd, buff_r.c_str(), buff_r.length(), 0)) == -1) {
				perror("send\n");
				exit(1);
			}
		} else if (pid == 0) {
			// 子进程用于接收消息
			memset(buff, 0, sizeof(buff));
			if (recv(clientfd, buff, sizeof(buff), 0) <= 0) {
				perror("recv:");
				close(clientfd);
				raise(SIGSTOP);
				exit(1);
			}
			printf("%s", buff);
		} else {
			perror("fork");
		}
	}
	close(clientfd);
	return 0;
}

