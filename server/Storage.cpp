// Copyright (c) 2014 lsc
// Storage.cpp
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string>

#include "Storage.h"

using std::move;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::for_each;

const char* file = "agenda.data";

Storage* Storage::instance_ = 0;

// transform json to map
map<string, string> json2map(const string &src) {
  map<string, string> j2m;
  int len = src.length();
  for (int cur = 0; cur < len; cur++) {
    if (src[cur] == ':' && !isdigit(src[cur-1])) {
      int fir, sec;
      for (fir = cur; fir >= 0 && src[fir] != '{' && src[fir] != ','; fir--) {}
      for (sec = cur; fir < len && src[sec] != '}' && src[sec] != ','; sec++) {}
      string key   = src.substr(fir + 1, cur - fir - 1),
             value = src.substr(cur + 1, sec - cur - 1);
      if (value[0] == '"')
        value = value.substr(1, value.length()-1);
      if (value[value.length()-1] == '"')
        value = value.substr(0, value.length()-1);
      j2m[key] = value;
    }
  }
  return j2m;
}

// read json line to line
inline string getjson(ifstream & inFile) {
  string res, tmp;
  getline(inFile, tmp);

  while (!tmp.length())
    getline(inFile, tmp);

  res+=tmp;
  while (tmp[tmp.length()-1] != '}') {
    getline(inFile, tmp);
    res+=tmp;;
  }
  return res;
}

inline int str2num(const string &str) {
  int num = 0, len = str.length();
  for (int i = 0; i < len; i++) {
    num *= 10;
    num += str[i] - '0';
  }
  return num;
}

Storage::Storage()  { readFromFile(file);    }

Storage::~Storage() { sync(); instance_ = 0; }

// File IO
bool Storage::sync() { return writeToFile(file); }

bool Storage::readFromFile(const char *fpath) {
  ifstream inFile(fpath);
  map<string, string> m;
  string line;
  if (!inFile)
    return 0;

  line = getjson(inFile);
  m = json2map(line);
  if ("User" == m["collection"]) {
    int num = str2num(m["total"]);
    while (num--) {
      line = getjson(inFile);
      m.clear();
      m = json2map(line);
      userList_.push_back(move(User(m["name"], m["password"], m["email"],
      m["phone"])));
    }
  } else if ("Meeting" == m["collection"]) {
    int num = str2num(m["total"]);
    while (num--) {
      line = getjson(inFile);
      m.clear();
      m = json2map(line);
      meetingList_.push_back(move(Meeting(m["sponsor"], m["participator"],
      Date::stringToDate(m["sdate"]), Date::stringToDate(m["edate"]),
      m["title"])));
    }
  }

  line = getjson(inFile);
  m.clear();
  m = json2map(line);

  if ("User" == m["collection"]) {
    int num = str2num(m["total"]);
    while (num--) {
      line = getjson(inFile);
      m.clear();
      m = json2map(line);
      userList_.push_back(move(User(m["name"], m["password"], m["email"],
      m["phone"])));
    }
  } else if ("Meeting" == m["collection"]) {
    int num = str2num(m["total"]);
    while (num--) {
      line = getjson(inFile);
      m.clear();
      m = json2map(line);
      meetingList_.push_back(move(Meeting(m["sponsor"], m["participator"],
        Date::stringToDate(m["sdate"]), Date::stringToDate(m["edate"]),
        m["title"])));
    }
  }

  inFile.close();
  return 1;
}
bool Storage::writeToFile(const char *fpath) {
  ofstream outFile(fpath);

  if (!outFile)
    return 0;

  outFile << "{collection:\"User\",total:" << userList_.size() << "}" << endl;
  for_each(userList_.begin(), userList_.end(), [&](const User &u) {
    outFile << "{name:\"" << u.getName() << "\",";
    outFile << "password:\"" << u.getPassword() << "\",";
    outFile << "email:\"" << u.getEmail() << "\",";
    outFile << "phone:\"" << u.getPhone() << "\"}" << endl;
  });

  outFile << "{collection:\"Meeting\",total:"
  << meetingList_.size() << "}" << endl;
  for_each(meetingList_.begin(), meetingList_.end(), [&](const Meeting &m) {
    outFile << "{sponsor:\"" << m.getSponsor() << "\",";
    outFile << "participator:\"" << m.getParticipator() << "\",";
    outFile << "sdate:\"" << Date::dateToString(m.getStartDate()) << "\",";
    outFile << "edate:\"" << Date::dateToString(m.getEndDate()) << "\",";
    outFile << "title:\"" << m.getTitle() << "\"}" << endl;
  });

  outFile.close();
  return 1;
}

// singleton
Storage* Storage::getInstance() {
  if (0 == instance_)
    instance_ = new Storage();
  return instance_;
}

// CRUD for User & Meeting using C++11 Function Template and Lambda Expressions
// create a user
void Storage::createUser(const User& user) { userList_.push_back(user); }

// return found users
list<User> Storage::queryUser(function<bool(const User&)> filter) {
  list<User> users;
  for_each(userList_.begin(), userList_.end(), [&users, filter](const User &u) {
    if (1 == filter(u))
      users.push_back(u);
  });
  return users;
}

// return the number of updated users
int Storage::updateUser(function<bool(const User&)> filter,
function<void(User&)> switcher) {
  int num = 0;
  for_each(userList_.begin(), userList_.end(), [&](User & u) {
    if (1 == filter(u)) {
      switcher(u);
      num++;
    }
  });
  return num;
}
// return the number of deleted users
int Storage::deleteUser(function<bool(const User&)> filter) {
  int num = 0;
  for (auto i = userList_.begin(); i != userList_.end();) {
    if (1 == filter(*i)) {
      i = userList_.erase(i);
      num++;
    } else {
    	i++;
    }
  }
  return num;
}

// create meetings
void Storage::createMeeting(const Meeting &meeting) {
  meetingList_.push_back(meeting);
}

// return found meetings
list<Meeting> Storage::queryMeeting(function<bool(const Meeting&)> filter) {
  list<Meeting> meetings;
  for_each(meetingList_.begin(), meetingList_.end(), [&](const Meeting &m) {
    if (1 == filter(m))
      meetings.push_back(m);
  });
  return meetings;
}

// return the number of updated meetings
int Storage::updateMeeting(function<bool(const Meeting&)> filter,
function<void(Meeting&)> switcher) {
  int num = 0;
  for_each(meetingList_.begin(), meetingList_.end(), [&](Meeting & m) {
    if (1 == filter(m)) {
      switcher(m);
      num++;
    }
  });
  return num;
}

// return the number of deleted meetings
int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
  int num = 0;
  for (auto i = meetingList_.begin(); i != meetingList_.end();) {
    if (1 == filter(*i)) {
      i = meetingList_.erase(i);
      num++;
    } else {
    	i++;
    }
  }
  return num;
}

