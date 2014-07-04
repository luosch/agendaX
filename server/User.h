#ifndef USER_H
#define USER_H

#include <string>
using std::string;

class User {
 public:
  User();
  User(string, string, string, string);
  string getName() const;
  void setName(string);
  string getPassword() const;
  void setPassword(string);
  string getEmail() const;
  void setEmail(string);
  string getPhone() const;
  void setPhone(string);

 private:
  string name_;
  string password_;
  string email_;
  string phone_;
};

#endif
