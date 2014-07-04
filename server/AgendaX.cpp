#include <iostream>
#include <string>
#include "Xserver.h"
#define PERM S_IRUSR|S_IWUSR
const int BACKLOG = 20;
const int MYPORT = 8888;
using namespace std;

/*// int => string
string int2str(int num) {
	string o_str, res;
	while (num) {
		o_str += num % 10 + '0';
		num /= 10;
	}
	int len = o_str.length();
	for (int i = 0; i < len; i++)
		res += o_str[len - 1 - i];
	return res;
}

// 获取当前系统时间
string getCurTime() {
	time_t now_time = time(0);
	tm *local = localtime(&now_time);
	string curTime;
	cout << local->tm_year+1900 << endl;
	cout << local->tm_mon+1 << endl;
	cout << local->tm_mday << endl;
	curTime += (int2str(local->tm_year+1900));
	curTime += '-';
	curTime += int2str(local->tm_mon+1);
	curTime += '-';
	curTime += int2str(local->tm_mday);
	curTime += '/';
	curTime += int2str(local->tm_hour);
	curTime += ':';
	curTime += int2str(local->tm_min);
	curTime += ':';
	curTime += int2str(local->tm_sec);
	return curTime;
}*/

// 端口绑定函数，创建套件字，并绑定到指定端口
int bindPort(int port) {
  int sockfd;
  sockaddr_in my_addr;
  //创建基于六套接字
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // IPv4协议族
  my_addr.sin_family = AF_INET;
  // 端口转换
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(my_addr.sin_zero),0);
  if ((bind(sockfd, (sockaddr*)(&my_addr), sizeof(sockaddr))) == -1) {
    perror("bind");
    exit(1);
  }
  printf("bind success!\n");
  return sockfd;
}

// 创建共享存储区
key_t shm_create() {
  key_t shmid;
  if ((shmid = shmget(IPC_PRIVATE, 1024, PERM)) == -1) {
    printf("Create Share Memory Error:%s\n\a",strerror(errno));
    exit(1);
  }
  return shmid;
}

int main() {
  // 定义监听套接字、客户套接字
  int sockfd, clientfd;
  socklen_t sin_size;
  // 定义地址结构
  sockaddr_in their_addr; 
  // 定义父子线程标记变量
  pid_t ppid;
  // 定义临时存储区
  char buff[255];
  // 检查是否登录成功
  bool login = 0;
  // 创建共享存储区
  // 绑定端口
  sockfd = bindPort(MYPORT);
  while (1) {
     // 指定端口上监听
    if (listen(sockfd, BACKLOG) == -1) {
      perror("listen");
      exit(1);
    }
    printf("listening......\n");
    if ((clientfd = accept(sockfd, (sockaddr*)&their_addr, &sin_size)) == -1) {
      perror("accept");
      exit(1);
    }
    printf("accept from: %s\n", inet_ntoa(their_addr.sin_addr));
    // 发送问候消息
    
    ppid = fork(); // 创建子进程
    if (ppid == 0) {
      while (1) {
        // 父进程用于接收信息
        memset(buff, 0, 255);
        Xserver x;
	      if (recv(clientfd, buff, 255, 0) <= 0) {
	        perror("recv");
	        close(clientfd);
	        raise(SIGKILL);
	        exit(1);
	      }
	      string rec(buff);
	      
	      if (strlen(buff) != 0) {
	        login = 1;
	        rec = x.OperationControl(buff);
	      }

	      if (rec != "[error] log in fail!\n")
	        login = 1;

        if (login) {
          if (send(clientfd, rec.c_str(), rec.length(), 0) == -1) {
            perror("send");
          }
        }
	    }
    } else if (ppid > 0) {
      continue;
    } else {
      perror("fork");
    }
  }
  printf("------------------------\n");
  close(sockfd);
  close(clientfd);
  return 0;
}
