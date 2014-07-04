// Copyright (c) 2014 lsc
// AgendaService.cpp
#include "AgendaService.h"
#include <iostream>
using std::endl;
using std::cout;
AgendaService::AgendaService()  { startAgenda(); }
AgendaService::~AgendaService() {                }

// check if format is right
inline bool checkFormat(const string& str) {
	if (str.length() < 16)
		return 0;
	bool num = isdigit(str[0]) && isdigit(str[1]) && isdigit(str[2])
	&& isdigit(str[3]) && isdigit(str[5]) && isdigit(str[6])
	&& isdigit(str[8]) && isdigit(str[9]) && isdigit(str[11])
	&& isdigit(str[12]) && isdigit(str[14]) && isdigit(str[15]);
	
	bool sig = (str[4] == '-') && (str[7] == '-')
	&& (str[10] == '/') && (str[13] == ':');
	
	return sig && num;
	
}

bool AgendaService::userLogIn(string userName, string password) {
  return !storage_->queryUser([userName](const User &u)->bool {
    return u.getName() == userName;
  }).empty();
}

bool AgendaService::userRegister(string name, string password,
string email, string phone) {
  bool found = !storage_->queryUser([name](const User &u)->bool {
    if (u.getName() == name)
      return 1;
    else
      return 0;
  }).empty();
  
  if (found) {
    return 0;
  } else {
    storage_->createUser(User(name, password, email, phone));
    storage_->sync();
    return 1;
  }
}

bool AgendaService::deleteUser(string userName, string password) {
	this->deleteAllMeetings(userName);
	bool del = storage_->deleteUser([=](const User &u)->bool {
    return u.getName() == userName && u.getPassword() == password;
  });
  storage_->sync();
  return del; 
}

list<User> AgendaService::listAllUsers() {
  return storage_->queryUser([=](const User &u) { return 1; });
}

bool AgendaService::createMeeting(string uName, string title, string part, string sDate, string eDate) {
  Date stD = Date::stringToDate(sDate);
  Date enD = Date::stringToDate(eDate);
  
  if (!(checkFormat(sDate) && checkFormat(eDate)))
  	return 0;

  if (!Date::isValid(stD))
  	return 0;

  if (!Date::isValid(enD))
  	return 0;
	
	if (stD >= enD)
		return 0;

  if (storage_->queryUser([=](const User &u)->bool {
    return u.getName() == uName;
  }).empty())
    return 0;
  
  if (storage_->queryUser([=](const User &u)->bool {
    return u.getName() == part;
  }).empty())
    return 0;

  if (!storage_->queryMeeting([=](const Meeting &m)->bool {
    return m.getTitle() == title;
  }).empty())
  	return 0;

	if (uName == part)
		return 0;

  if (!meetingQuery(uName, sDate, eDate).empty())
  	return 0;
  
  if (!meetingQuery(part, sDate, eDate).empty())
  	return 0;
  	
  storage_->createMeeting(Meeting(uName, part, stD, enD, title));
  storage_->sync();
  return 1;
}

list<Meeting> AgendaService::meetingQuery(string uName, string title) {
  return storage_->queryMeeting([=](const Meeting &m) {
    return (m.getSponsor() == uName || m.getParticipator() == uName)
    && m.getTitle() == title;
  });
}
list<Meeting> AgendaService::meetingQuery(string uName, string sDate, string eDate) {
  return storage_->queryMeeting([=](const Meeting &m) {
    if (m.getSponsor() == uName || m.getParticipator() == uName) {
      if (!(m.getStartDate() > Date::stringToDate(eDate)
      || m.getEndDate() < Date::stringToDate(sDate)))
        return 1;
      else
        return 0;
    } else {
      return 0;
    }
  });
}

list<Meeting> AgendaService::listAllMeetings(string userName) {
  return storage_->queryMeeting([=](const Meeting &m) {
    return m.getSponsor() == userName || m.getParticipator() == userName;
  });
}

list<Meeting> AgendaService::listAllSponsorMeetings(string userName) {
  return storage_->queryMeeting([=](const Meeting &m) {
    return m.getSponsor() == userName;
  });
}

list<Meeting> AgendaService::listAllParticipateMeetings(string userName) {
  return storage_->queryMeeting([=](const Meeting &m) {
    return m.getParticipator() == userName;
  });
}

bool AgendaService::deleteMeeting(string uName, string title) {
  bool del = storage_->deleteMeeting([=](const Meeting &m) {
    return m.getSponsor() == uName && m.getTitle() == title;
  });
  storage_->sync();
  return del;
}

bool AgendaService::deleteAllMeetings(string uName) {
  bool del = storage_->deleteMeeting([=](const Meeting &m) {
    return m.getSponsor() == uName;
  });
  storage_->sync();
  return del;
}

void AgendaService::startAgenda() { storage_ = Storage::getInstance(); }

void AgendaService::quitAgenda()  { delete storage_;                   }

