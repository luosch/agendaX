#ifndef X_CLIENT_H
#define X_CLIENT_H
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

string sendControl(char *buff) {
	string op(buff);
	if (op == "r") {
		cout << "[register] [user name] [password] [email] [phone]" << endl;
		cout << "[register] ";
		string uName, password, email, phone;
		cin >> uName >> password >> email >> phone;
		op += ',';
		op += uName;
		op += ',';
		op += password;
		op += ',';
		op += email;
		op += ',';
		op += phone;
		return op;
	}
	if (op == "qm") {
		cout << "[query meeting] [title]:"<< endl << "[query meeting] ";
		string title;
		cin >> title;
		op += ',';
		op += title;
		return op;
	}
	if (op == "dm") {
		cout << "[delete meeting] [title]" << endl;
		cout << "[delete meeting] ";
		string title;
		cin >> title;
		op += ',';
		op += title;
		return op;
	}
	if (op == "qt") {
		cout << endl << "[query meetings] [start time<yyyy-mm-dd/hh:mm>] "
		<< "[end time<yyyy-mm-dd/hh:mm>]" << endl << "[query meetings] ";
		string sDate, eDate;
		cin >> sDate >> eDate;
		op += ',';
		op += sDate;
		op += ',';
		op += eDate;
		return op;
	}
	if (op == "cm") {
		cout << "[create meeting] [title] [participator] ";
		cout << "[start time<yyyy-mm-dd/hh:mm>] " << endl;
		cout << "[end time<yyyy-mm-dd/hh:mm>]" << endl;
		cout << "[create meeting] ";
		string title, part, sDate, eDate;
		cin >> title >> part >> sDate >> eDate;
		op += ',';
		op += title;
		op += ',';
		op += part;
		op += ',';
		op += sDate;
		op += ',';
		op += eDate;
		return op;
	}
	if (op == "dc") {
		cout << "删除用户前请输入密码" << endl;
		string pw;
		cin >> pw;
		op += ',';
		op += pw;
		return op;
	}
	return op;
}

#endif
