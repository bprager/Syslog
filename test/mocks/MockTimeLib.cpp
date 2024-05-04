#include "MockTimeLib.h"

// Initialize the mock time to the current system time as default.
std::time_t mock_current_time = std::time(nullptr);

// Definition of all declared functions
void setMockTime(std::time_t t) { mock_current_time = t; }

int hour() {
  struct tm *timeinfo = localtime(&mock_current_time);
  return timeinfo->tm_hour;
}

int minute() {
  struct tm *timeinfo = localtime(&mock_current_time);
  return timeinfo->tm_min;
}

int second() {
  struct tm *timeinfo = localtime(&mock_current_time);
  return timeinfo->tm_sec;
}

int day() {
  struct tm *timeinfo = localtime(&mock_current_time);
  return timeinfo->tm_mday;
}

int weekday() {
  struct tm *timeinfo = localtime(&mock_current_time);
  return timeinfo->tm_wday + 1; // +1 to adjust for the typical 1-7 range
}

int month() {
  struct tm *timeinfo = localtime(&mock_current_time);
  return timeinfo->tm_mon + 1; // +1 because tm_mon is 0-11
}

int year() {
  struct tm *timeinfo = localtime(&mock_current_time);
  return timeinfo->tm_year + 1900; // tm_year is years since 1900
}

time_t now() { return mock_current_time; }

void setTime(time_t t) { mock_current_time = t; }

void setTime(int hr, int min, int sec, int day, int month, int yr) {
  struct tm t = {};
  t.tm_hour = hr;
  t.tm_min = min;
  t.tm_sec = sec;
  t.tm_mday = day;
  t.tm_mon = month - 1;  // struct tm expects months to be 0-11
  t.tm_year = yr - 1900; // struct tm expects years since 1900
  mock_current_time = mktime(&t);
}

void adjustTime(long adjustment) { mock_current_time += adjustment; }

const char *monthShortStr(int month) {
  static const char *monthNames[] = {"Err", "Jan", "Feb", "Mar", "Apr",
                                     "May", "Jun", "Jul", "Aug", "Sep",
                                     "Oct", "Nov", "Dec"};
  if (month < 1 || month > 12) {
    return monthNames[0]; // Return "Err" if month is out of range
  }
  return monthNames[month];
}

const char *monthStr(int month) {
  static const char *monthNames[] = {
      "Error", "January", "February",  "March",   "April",    "May",     "June",
      "July",  "August",  "September", "October", "November", "December"};
  if (month < 1 || month > 12) {
    return monthNames[0]; // Return "Error" if month is out of range
  }
  return monthNames[month];
}

const char *dayShortStr(int weekday) {
  static const char *weekdayNames[] = {"Err", "Sun", "Mon", "Tue",
                                       "Wed", "Thu", "Fri", "Sat"};
  if (weekday < 1 || weekday > 7) {
    return weekdayNames[0]; // Return "Err" if weekday is out of range
  }
  return weekdayNames[weekday];
}
