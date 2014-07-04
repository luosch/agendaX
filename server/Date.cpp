// Copyright (c) 2014 lsc
// Date.cpp
#include "Date.h"
#include <iostream>
using std::cout;
using std::endl;

/*------------
members of Date
  e.g 2014-6-30/8:00
  year_   -> 2014
  month_  -> 6
  day_    -> 30
  hour_   -> 8
  minute_ -> 0
------------*/

const int num_month[] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool Date::isLeap() const {
  if (0 == year_ % 100) {
    if (0 == year_ % 400)
      return 1;
    else
      return 0;
  } else {
    if (0 == year_ % 4)
      return 1;
    else
      return 0;
  }
}

Date::Date() {
  // default Date 2014-6-30/8:00
  year_   = 2014;
  month_  = 6;
  day_    = 30;
  hour_   = 8;
  minute_ = 0;
}

Date::Date(int y, int m, int d, int h, int mi) {
  // set member value
  year_   = y;
  month_  = m;
  day_    = d;
  hour_   = h;
  minute_ = mi;
}

// get value
int Date::getYear() const   { return year_;   }
int Date::getMonth() const  { return month_;  }
int Date::getDay() const    { return day_;    }
int Date::getHour() const   { return hour_;   }
int Date::getMinute() const { return minute_; }

// set value
void Date::setYear(int input)   { year_   = input; }
void Date::setMonth(int input)  { month_  = input; }
void Date::setDay(int input)    { day_    = input; }
void Date::setHour(int input)   { hour_   = input; }
void Date::setMinute(int input) { minute_ = input; }

// check if Date is valid
bool Date::isValid(const Date &date) {
  int y  = date.year_;
  int m  = date.month_;
  int d  = date.day_;
  int h  = date.hour_;
  int mi = date.minute_;

  bool leap = date.isLeap();

  if (y < 1000 || y > 9999 || m <= 0 || d <= 0 || h < 0 || mi < 0)
    return 0;

  if (m > 12 || h > 23 || mi > 59)
    return 0;

  if (leap) {
    if (2 == m)
      return d <= 29;
    else
      return d <= num_month[m];
  } else {
    return d <= num_month[m];
  }
}

// string => Date
Date Date::stringToDate(const string &dateString) {
  // checkformat is ignored
  int st = -1, ed = 0, tmp = 0, i = 0;
  Date date;

  for (ed = st+1; dateString[ed] != '-'; ed++) { }
  for (tmp = 0, i = st+1; i < ed; i++) {
    tmp *= 10;
    tmp += dateString[i] - '0';
  }
  date.setYear(tmp);
  st = ed;

  for (ed = st+1; dateString[ed] != '-'; ed++) { }
  for (tmp = 0, i = st+1; i < ed; i++) {
    tmp *= 10;
    tmp += dateString[i] - '0';
  }
  date.month_ = tmp;
  st = ed;

  for (ed = st+1; dateString[ed] != '/'; ed++) { }
  for (tmp = 0, i = st+1; i < ed; i++) {
    tmp *= 10;
    tmp += dateString[i] - '0';
  }
  date.day_ = tmp;
  st = ed;

  for (ed = st+1; dateString[ed] != ':'; ed++) { }
  for (tmp = 0, i = st+1; i < ed; i++) {
    tmp *= 10;
    tmp += dateString[i] - '0';
  }
  date.hour_ = tmp;
  st = ed;

  int len = dateString.length();
  for (ed = st+1; ed < len; ed++) {}
  for (tmp = 0, i = st+1; i < ed; i++) {
    tmp *= 10;
    tmp += dateString[i] - '0';
  }
  date.minute_ = tmp;
  st = ed;

  return date;
}

// Date => string
string Date::dateToString(const Date &date) {
  int y  = date.year_;
  int m  = date.month_;
  int d  = date.day_;
  int h  = date.hour_;
  int mi = date.minute_;
  string y_str, m_str, d_str, h_str, mi_str, date_str;

  if (!isValid(date))
    return "invalid date";

  y_str += y / 1000 + '0';
  y_str += y / 100 % 10 + '0';
  y_str += y / 10 % 10 + '0';
  y_str += y % 10 +'0';

  m_str  += m / 10 + '0';
  m_str  += m  % 10 + '0';

  d_str  += d / 10 + '0';
  d_str  += d  % 10 + '0';

  h_str  += h / 10 + '0';
  h_str  += h  % 10 + '0';

  mi_str += mi / 10 + '0';
  mi_str += mi % 10 + '0';


  date_str = y_str + '-' + m_str + '-' + d_str + '/' + h_str + ':' + mi_str;
  return date_str;
}

// overload operator
Date& Date::operator=(const Date &date) {
  this->year_   = date.year_;
  this->month_  = date.month_;
  this->day_    = date.day_;
  this->hour_   = date.hour_;
  this->minute_ = date.minute_;
  return *this;
}

bool Date::operator==(const Date &date) const {
  return this->year_   == date.year_   &&
         this->month_  == date.month_  &&
         this->day_    == date.day_    &&
         this->hour_   == date.hour_   &&
         this->minute_ == date.minute_;
}

bool Date::operator>(const Date& date) const {
  if (*this == date)
    return 0;
  if (this->year_ < date.year_)
    return 0;
  else if (this->year_ > date.year_)
    return 1;
  if (this->month_ < date.month_)
    return 0;
  else if (this->month_ > date.month_)
    return 1;
  if (this->day_ < date.day_)
    return 0;
  else if (this->day_ > date.day_)
    return 1;
  if (this->hour_ < date.hour_)
    return 0;
  else if (this->hour_ > date.hour_)
    return 1;
  if (this->minute_ < date.minute_)
    return 0;
  else if (this->minute_ > date.minute_)
    return 1;

  return 1;
}

bool Date::operator<(const Date& date) const {
  if (*this == date)
    return 0;
  if (this->year_ > date.year_)
    return 0;
  else if (this->year_ < date.year_)
    return 1;
  if (this->month_ > date.month_)
    return 0;
  else if (this->month_ < date.month_)
    return 1;
  if (this->day_ > date.day_)
    return 0;
  else if (this->day_ < date.day_)
    return 1;
  if (this->hour_ > date.hour_)
    return 0;
  else if (this->hour_ < date.hour_)
    return 1;
  if (this->minute_ > date.minute_)
    return 0;
  else if (this->minute_ < date.minute_)
    return 1;

  return 1;
}

bool Date::operator>=(const Date& date) const {
  return *this > date || *this == date;
}
bool Date::operator<=(const Date& date) const {
  return *this < date || *this == date;
}

