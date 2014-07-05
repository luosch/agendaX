#include "Xserver.h"
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

Xserver::Xserver() { }

// int => string
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
}

string Xserver::OperationControl(char *buff) {
  string str(buff);
  string op, name;
  if (str.find("::") != string::npos)
    return userLogIn(str);

  int x = str.find('*');
  op = str.substr(0, x);
  name = str.substr(x + 1, str.length() - x - 1);

  if (op == "h")
    return getHelp();
  if (op == "lu")
    return listAllUsers(name);
  if (op == "la")
    return listAllMeetings(name);
  if (op == "las")
    return listAllSponsorMeetings(name);
  if (op == "lap")
    return listAllParticipateMeetings(name);
  if (op == "da")
    return deleteAllMeetings(name);
    
  if (op.substr(0, 2) == "qm" || op.substr(0, 2) == "dm") {
    string title;
    title = op.substr(3, op.length() - 3);
    if (op.substr(0, 2) == "qm")
      return queryMeetingByTitle(name, title);

    if (op.substr(0, 2) == "dm")
      return deleteMeetingByTitle(name, title);
  }
  // qt,1000-01-01/00:00,9999-01-01/00:00
  if (op.substr(0, 2) == "qt") {
    string sDate, eDate;
    int i;
    for (i = 3; op[i] != ','; i++) {}
    sDate = op.substr(3, 16);
    eDate = op.substr(i+1, 16);
    return queryMeetingByTimeInterval(name, sDate, eDate);
  }
  if (op.substr(0, 2) == "dc") {
  	string pw;
  	int i;
    for (i = 3; op[i] != '*'; i++) {}
    cout << pw << endl;
    pw = op.substr(3, i - 3);
    return deleteUser(name, pw);
  }
  // cm,ZZZ,MIO,2014-01-01/00:00,2014-01-02/00:00
  if (op.substr(0, 2) == "cm") {
    string title, part, sDate, eDate;
    int s1 = 2, s2, s3, s4, len = op.length();
    for (s2 = s1+1; op[s2] != ','; s2++) {}
    for (s3 = s2+1; op[s3] != ','; s3++) {}
    for (s4 = s3+1; op[s4] != ','; s4++) {}
    title = op.substr(s1+1, s2-s1-1);
    part = op.substr(s2+1, s3-s2-1);
    sDate = op.substr(s3+1, s4-s3-1);
    eDate = op.substr(s4+1, len-s4-1);
    return createMeeting(name, title, part, sDate, eDate);
  }
  
  if (op.substr(0, 1) == "r") {
    string name, password, email, phone;
    int s1 = 2, s2, s3, s4, len = op.length();
    for (s2 = s1 + 1; op[s2] != ','; s2++) {}
    for (s3 = s2 + 1; op[s3] != ','; s3++) {}
    for (s4 = s3 + 1; op[s4] != ','; s4++) {}
    name = op.substr(s1+1, s2-s1-1);
    password = op.substr(s2+1, s3-s2-1);
    email = op.substr(s3+1, s4-s3-1);
    phone = op.substr(s4+1, len-s4-1);
    return userRegister(name, password, email, phone);
  }
  
  return "\ninvalid input\n\n";
}

string Xserver::userRegister(string uName, string password
, string email, string phone) {
  bool reg = agendaService_.userRegister(uName, password, email, phone);
  if (!reg)
  	return "[error] register fail!\n";
  else
    return "[register] succeed!\n";
}

string Xserver::deleteUser(string uName, string pw) {
	bool del = agendaService_.deleteUser(uName, pw);
	if (!del) {
		return "\n[error] delete agenda account fail!\n\n";
	}
  else {
    return "\n[delete agenda account] succeed!\n\n";
  }
}

string Xserver::getHelp() {
  string tmp;
  tmp += "\n----------------Welcome to AgendaX-----------------\n";
  tmp += "Action :\n";
  tmp += "r   - register an agenda account\n";
  tmp += "lu  - list all Agenda user\n";
  tmp += "la  - list all meetings\n";
  tmp += "las - list all sponsor meetings\n";
  tmp += "lap - list all participate meetings\n";
  tmp += "cm  - create a meeting\n";
  tmp += "qm  - query meeting by title)\n";
  tmp += "qt  - query meeting by time interval(qt,startDate,endDate)\n";
  tmp += "dm  - delete meeting by title\n";
  tmp += "da  - delete all meetings\n";
  tmp += "dc  - delete agenda account\n";
  tmp += "h   - get help\n";
  tmp += "---------------------------------------------------\n\n";
  return tmp;
}

string Xserver::deleteAllMeetings(string uName) {
	bool dam = agendaService_.deleteAllMeetings(uName);
	cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] da" << endl;
	if (!dam)
    return "\n[delete all meetings] fail!\n";
  else
    return "\n[delete all meetings] succeed!\n";
}

string Xserver::userLogIn(string str) {
  string uName, pw;
  uName = str.substr(0, str.find(':'));
  pw = str.substr(str.find(':') + 1, str.length() - str.find(':'));
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] try to login" << endl;

  bool login = agendaService_.userLogIn(uName, pw);
  string tmp;
  if (!login) {
    tmp += "[error] log in fail!\n";
    cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] login fail" << endl;
    return tmp;
  } else {
    cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] login succeed" << endl;
    tmp += "[log in] succeed!\n";
    tmp += "\n----------------Welcome to AgendaX-----------------\n";
		tmp += "Action :\n";
		tmp += "lu  - list all Agenda user\n";
		tmp += "la  - list all meetings\n";;
		tmp += "las - list all sponsor meetings\n";
		tmp += "lap - list all participate meetings\n";
		tmp += "cm  - create a meeting\n";
		tmp += "qm  - query meeting by title\n";
		tmp += "qt  - query meeting by time interval\n";
		tmp += "dm  - delete meeting by title\n";
		tmp += "da  - delete all meetings\n";
		tmp += "h   - get help\n";
		tmp += "---------------------------------------------------\n\n";
    return tmp;
  }
}
string Xserver::listAllUsers(string uName) {
  ostringstream oss;
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] lu" << endl;
	oss << "[list all users]" << endl << endl;
  oss << "name             email            phone" << endl;
  list<User> lau = agendaService_.listAllUsers();
  for_each(lau.begin(), lau.end(), [&](const User &u) {
  	oss << setiosflags(ios::left) << setw(16) << u.getName() << ' ';
  	oss << setiosflags(ios::left) << setw(16) << u.getEmail() << ' ';
  	oss << setiosflags(ios::left) << u.getPhone() << endl;
  });
  oss << endl;
  return oss.str();
}
string Xserver::listAllMeetings(string uName) {
  string tmp;
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] la" << endl;
  tmp += "[list all meetings]\n\n";
  tmp += printMeetings(agendaService_.listAllMeetings(uName));
  return tmp;
}

string Xserver::listAllSponsorMeetings(string uName) {
  string tmp;
  cout << "[" << uName << "] " << "[action] las" << endl;
  tmp += "[list all sponsor meetings]\n\n";
  tmp += printMeetings(agendaService_.listAllSponsorMeetings(uName));
  return tmp;
}

string Xserver::listAllParticipateMeetings(string uName) {
  string tmp;
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] lap" << endl;
  tmp += "[list all participate meetings]\n\n";
  tmp += printMeetings(agendaService_.listAllParticipateMeetings(uName));
  return tmp;
}

string Xserver::queryMeetingByTitle(string uName, string title) {
  ostringstream oss;
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] qm!" << endl;
  oss << printMeetings(agendaService_.meetingQuery(uName, title));
  return oss.str();
}
string Xserver::queryMeetingByTimeInterval(string uName, string sDate, string eDate) {
  ostringstream oss;
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] qt" << endl;
  oss << endl << "[query meetings]" << endl;
  oss << printMeetings(agendaService_.meetingQuery(uName, sDate, eDate));
  return oss.str();
}

string Xserver::deleteMeetingByTitle(string uName, string title) {
  ostringstream oss;
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] dm" << endl;
  bool qmb = agendaService_.deleteMeeting(uName, title);
  if (!qmb)
    oss << endl << "[error] delete meeting fail!" << endl;
  else
    oss << endl << "[delete meeting by title] succeed!" << endl;
  return oss.str();
}

string Xserver::createMeeting(string uName, string title
, string part, string sDate, string eDate) {
  ostringstream oss;
  cout << getCurTime() << ":" << endl << "[" << uName << "] " << "[action] cm" << endl;
  bool crm = agendaService_.createMeeting(uName, title, part, sDate, eDate);
  if (!crm)
    oss << endl << "[error] create meeting fail!" << endl << endl;
  else
    oss << endl << "[create meeting] succeed!" << endl << endl;
  return oss.str();
}

string Xserver::printMeetings(const list<Meeting> &meetings) {
  ostringstream oss;
	if (meetings.empty()) {
		oss << "[meetings not found]" << endl << endl;
		return oss.str();
	}
	oss << "title      sponsor    participator  start time       end time" << endl;
  for_each(meetings.begin(), meetings.end(), [&](const Meeting & m) {
  	oss << setiosflags(ios::left) << setw(10) << m.getTitle() << ' ';
  	oss << setiosflags(ios::left) << setw(10) << m.getSponsor() << ' ';
  	oss << setiosflags(ios::left) << setw(13) << m.getParticipator() << ' ';
  	oss << setiosflags(ios::left) << Date::dateToString(m.getStartDate()) << ' ';
  	oss << setiosflags(ios::left) << Date::dateToString(m.getEndDate()) << endl;
  });
  return oss.str();
}

