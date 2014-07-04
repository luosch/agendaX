#ifndef X_SERVER_H
#define X_SERVER_H

#include "AgendaService.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/shm.h>
#include <time.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
using std::cout;
using std::endl;
using std::list;
using std::string;
using std::ostringstream;

class Xserver {
 public:
  Xserver();
  string getHelp();
  string userLogIn(string str);
  string OperationControl(char *buff);
  string listAllUsers(string str);
  string listAllMeetings(string str);
  string listAllSponsorMeetings(string str);
  string listAllParticipateMeetings(string str);
  string printMeetings(const list<Meeting> &m);
  string listAllUsers();
  string createMeeting(string uName, string title, string part, string sDate, string eDate);
  string queryMeetingByTitle(string uName, string title);
  string queryMeetingByTimeInterval(string uName, string sDate, string eDate);
  string deleteMeetingByTitle(string uName, string title);
  string deleteAllMeetings(string uName);

  AgendaService agendaService_;
};

#endif

