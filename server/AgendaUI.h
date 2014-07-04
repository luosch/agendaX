#ifndef AGENDAUI_H
#define AGENDAUI_H

#include "AgendaService.h"
#include <string>
#include <list>
using std::list;
using std::string;

class AgendaUI {
 public:
  AgendaUI();
  void OperationLoop();

 private:
  // task functions
  void startAgenda();
  void getHelp();
  string getOperation();
  bool executeOperation(string op);
  void userLogIn();
  void userRegister();
  void quitAgenda();
  void userLogOut();
  void deleteUser();
  void listAllUsers();
  void createMeeting();
  void listAllMeetings();
  void listAllSponsorMeetings();
  void listAllParticipateMeetings();
  void queryMeetingByTitle();
  void queryMeetingByTimeInterval();
  void deleteMeetingByTitle();
  void deleteAllMeetings();
  void printMeetings(const list<Meeting> &meetings);
  // dates
  string userName_;
  string userPassword_;
  AgendaService agendaService_;
};

#endif
