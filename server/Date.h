#ifndef DATE_H
#define DATE_H

#include <string>
#include <cctype>
using std::string;

class Date {
 public:
  Date();
  Date(int year, int month, int day, int hour, int minute);
  int getYear() const;
  void setYear(int year);
  int getMonth() const;
  void setMonth(int month);
  int getDay() const;
  void setDay(int day);
  int getHour() const;
  void setHour(int hour);
  int getMinute() const;
  void setMinute(int minute);
  static bool isValid(const Date& date);
  static Date stringToDate(const string& str);
  static string dateToString(const Date& date);
  bool isLeap() const;
  Date &operator=(const Date& date);
  bool operator==(const Date& date) const;
  bool operator>(const Date& date) const;
  bool operator<(const Date& date) const;
  bool operator>=(const Date& date) const;
  bool operator<=(const Date& date) const;

 private:
  int year_;
  int month_;
  int day_;
  int hour_;
  int minute_;
};

#endif
