// Copyright (c) 2014 lsc
// User.cpp
#include "User.h"

/*-------------------------
members of User
  name_     -> lsc
  password_ -> 123456
  email_    -> me@lsich.com
  phone_    -> 10000
-------------------------*/

User::User() {
  name_     = "lsc";
  password_ = "123456";
  email_    = "me@lsich.com";
  phone_    = "10000";
}

User::User(string na, string pw, string em, string ph) {
  name_     = na;
  password_ = pw;
  email_    = em;
  phone_    = ph;
}

// get member value
string User::getName() const     { return name_;     }
string User::getPassword() const { return password_; }
string User::getEmail() const    { return email_;    }
string User::getPhone() const    { return phone_;    }

// set member value
void User::setName(string na)     { name_ = na;     }
void User::setPassword(string pw) { password_ = pw; }
void User::setEmail(string em)    { email_ = em;    }
void User::setPhone(string ph)    { phone_ = ph;    }

