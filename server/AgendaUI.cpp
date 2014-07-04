// Copyright (c) 2014 lsc
// Meeting.cpp
#include "AgendaUI.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
using std::ios;
using std::cin;
using std::setw;
using std::cout;
using std::endl;
using std::left;
using std::for_each;
using std::setiosflags;
using std::setfill;

AgendaUI::AgendaUI() { startAgenda(); }

void AgendaUI::getHelp() {
  cout << "----------------------Agenda-----------------------" << endl;
  cout << "Action :" << endl;
  cout << "o   - log out Agenda" << endl;
  cout << "dc  - delete Agenda account" << endl;
  cout << "lu  - list all Agenda user" << endl;
  cout << "cm  - create a meeting" << endl;
  cout << "la  - list all meetings" << endl;
  cout << "las - list all sponsor meetings" << endl;
  cout << "lap - list all participate meetings" << endl;
  cout << "qm  - query meeting by title" << endl;
  cout << "qt  - query meeting by time interval" << endl;
  cout << "dm  - delete meeting by title" << endl;
  cout << "da  - delete all meetings" << endl;
  cout << "h   - view operation method" << endl;
  cout << "---------------------------------------------------" << endl;
  OperationLoop();
}

void AgendaUI::OperationLoop() {
	if (userName_ == "") {
		cout << "----------------------Agenda-----------------------" << endl;
		cout << "Action :" << endl;
		cout << "l    -  log in Agenda by user name and password" << endl;
		cout << "r    -  register an Agenda account" << endl;
		cout << "q    -  quit Agenda" << endl;
		cout << "h    -  view operation method" << endl;
		cout << "---------------------------------------------------" << endl;
  }
  executeOperation(getOperation());
}

void AgendaUI::startAgenda() {
	userName_     = "";
	userPassword_ = "";
}

string AgendaUI::getOperation() {
	string input = "";
	while (input.length() < 1) {
		if ("" == userName_)
      cout << "Agenda : ~$ ";
    else
      cout << "Agenda@" << userName_ << " : # ";
		getline(cin, input, '\n');
	}
	return input;
}
bool AgendaUI::executeOperation(string op) {
	bool exec = 1;
	// unsignned operations
  if (userName_ == "") {
    if (op == "l")
      userLogIn();
    else if (op == "r")
      userRegister();
    else if (op == "q")
      quitAgenda();
    else if (op == "h")
    	getHelp();
    else
      exec = 0;
  } else if (userName_ != "") {
    // signned operations
    if (op == "o")
      userLogOut();
    else if (op == "dc")
      deleteUser();
    else if (op == "lu")
      listAllUsers();
    else if (op == "cm")
      createMeeting();
    else if (op == "la")
      listAllMeetings();
    else if (op == "las")
      listAllSponsorMeetings();
    else if (op == "lap")
      listAllParticipateMeetings();
    else if (op == "qm")
      queryMeetingByTitle();
    else if (op == "qt")
      queryMeetingByTimeInterval();
    else if (op == "dm")
      deleteMeetingByTitle();
    else if (op == "da")
      deleteAllMeetings();
    else if (op == "h")
    	getHelp();
    else
      exec = 0;
  }
  if (exec) {
  	return exec;
  } else {
    cout << "invalid input, try again" << endl;
    OperationLoop();
    return 0;
  }
}

void AgendaUI::userLogIn() {
	cout << "[log in] [user name] [password]" << endl;
  cout << "[log in] ";
  string uName, pw;
  cin >> uName >> pw;
  cin.ignore();
  bool login = agendaService_.userLogIn(uName, pw);
  if (!login) {
    cout << "[error] log in fail!" << endl;
    OperationLoop();
  } else {
  	userName_ = uName;
  	userPassword_ = pw;
    cout << "[log in] succeed!" << endl << endl;
    cout << "----------------------Agenda-----------------------" << endl;
    cout << "Action :" << endl;
    cout << "o   - log out Agenda" << endl;
    cout << "dc  - delete Agenda account" << endl;
    cout << "lu  - list all Agenda user" << endl;
    cout << "cm  - create a meeting" << endl;
    cout << "la  - list all meetings" << endl;
    cout << "las - list all sponsor meetings" << endl;
    cout << "lap - list all participate meetings" << endl;
    cout << "qm  - query meeting by title" << endl;
    cout << "qt  - query meeting by time interval" << endl;
    cout << "dm  - delete meeting by title" << endl;
    cout << "da  - delete all meetings" << endl;
    cout << "h   - view operation method" << endl;
    cout << "---------------------------------------------------" << endl;
    OperationLoop();
  }
}
void AgendaUI::userRegister() {
	cout << "[register] [user name] [password] [email] [phone]" << endl;
  cout << "[register] ";
  string uName, password, email, phone;
  cin >> uName >> password >> email >> phone;
  cin.ignore();
  bool reg = agendaService_.userRegister(uName, password, email, phone);
  if (!reg)
  	cout << "[error] register fail!" << endl;
  else
    cout << "[register] succeed!" << endl;
  OperationLoop();
}
void AgendaUI::quitAgenda() { agendaService_.quitAgenda(); }
void AgendaUI::userLogOut() { userName_ = ""; userPassword_ = ""; OperationLoop(); }
void AgendaUI::deleteUser() {
	bool del = agendaService_.deleteUser(userName_, userPassword_);
	if (!del) {
		cout << endl << "[error] delete agenda account fail!" << endl << endl;
	}
  else {
    cout << endl "[delete agenda account] succeed!" << endl << endl;
  }
  userLogOut();
}
void AgendaUI::listAllUsers() {
	cout << "[list all users]" << endl << endl;
  cout << "name             email            phone" << endl;
  list<User> lau = agendaService_.listAllUsers();
  for_each(lau.begin(), lau.end(), [](const User &u) {
  	cout << setiosflags(ios::left) << setw(16) << u.getName() << ' ';
  	cout << setiosflags(ios::left) << setw(16) << u.getEmail() << ' ';
  	cout << setiosflags(ios::left) << u.getPhone() << endl;
  });
  cout << endl;
  OperationLoop();
}
void AgendaUI::createMeeting() {
	cout << "[create meeting] [title] [participator] ";
  cout << "[start time<yyyy-mm-dd/hh:mm>] " << endl;
  cout << "[end time<yyyy-mm-dd/hh:mm>]" << endl;
  cout << "[create meeting] ";
  string title, part, sDate, eDate;
  cin >> title >> part >> sDate >> eDate;
  cin.ignore();
  bool crm = agendaService_.createMeeting(userName_, title, part, sDate, eDate);
  if (!crm)
    cout << endl << "[error] create meeting fail!" << endl << endl;
  else
    cout << endl << "[create meeting] succeed!" << endl << endl;
  OperationLoop();
}
void AgendaUI::listAllMeetings() {
	cout << "[list all meetings]" << endl << endl;
  printMeetings(agendaService_.listAllMeetings(userName_));
  OperationLoop();
}
void AgendaUI::listAllSponsorMeetings() {
	cout << "[list all sponsor meetings]" << endl << endl;
  printMeetings(agendaService_.listAllSponsorMeetings(userName_));
  OperationLoop();
}
void AgendaUI::listAllParticipateMeetings() {
	cout << "[list all participate meetings]" << endl << endl;
  printMeetings(agendaService_.listAllParticipateMeetings(userName_));
  OperationLoop();
}
void AgendaUI::queryMeetingByTitle() {
  cout << "[query meeting] [title]:"<< endl << "[query meeting] ";
  string title;
  cin >> title;
  cin.ignore();
  printMeetings(agendaService_.meetingQuery(userName_, title));
  OperationLoop();
}
void AgendaUI::queryMeetingByTimeInterval() {
	cout << endl << "[query meetings] [start time<yyyy-mm-dd/hh:mm>] "
  << "[end time<yyyy-mm-dd/hh:mm>]" << endl << "[query meetings] ";
  string sDate, eDate;
  cin >> sDate >> eDate;
  cin.ignore();
  cout << endl << "[query meetings]" << endl;
  printMeetings(agendaService_.meetingQuery(userName_, sDate, eDate));
  OperationLoop();
}
void AgendaUI::deleteMeetingByTitle() {
	cout << "[delete meeting] [title]" << endl;
  cout << "[delete meeting] ";
  string title;
  cin >> title;
  cin.ignore();
  bool qmb = agendaService_.deleteMeeting(userName_, title);
  if (!qmb)
    cout << endl << "[error] delete meeting fail!" << endl;
  else
    cout << endl << "[delete meeting by title] succeed!" << endl;
  OperationLoop();
}
void AgendaUI::deleteAllMeetings() {
	bool dam = agendaService_.deleteAllMeetings(userName_);
	if (!dam)
    cout << endl << "[delete all meetings] fail!" << endl;
  else
    cout << endl << "[delete all meetings] succeed!" << endl;
  OperationLoop();
}
void AgendaUI::printMeetings(const list<Meeting> &meetings) {
	if (meetings.empty()) {
		cout << "[meetings not found]" << endl << endl;
		return;
	}
	cout << "title      sponsor    participator  start time       end time" << endl;
  for_each(meetings.begin(), meetings.end(), [](const Meeting & m) {
  	cout << setiosflags(ios::left) << setw(10) << m.getTitle() << ' ';
  	cout << setiosflags(ios::left) << setw(10) << m.getSponsor() << ' ';
  	cout << setiosflags(ios::left) << setw(13) << m.getParticipator() << ' ';
  	cout << setiosflags(ios::left) << Date::dateToString(m.getStartDate()) << ' ';
  	cout << setiosflags(ios::left) << Date::dateToString(m.getEndDate()) << endl;
  });
}

