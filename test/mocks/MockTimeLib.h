#ifndef MOCK_TIME_LIB_H
#define MOCK_TIME_LIB_H

#include <ctime>

// Declare a global variable to hold the current mocked time.
extern std::time_t mock_current_time;

// Function declarations
void setMockTime(std::time_t t); // Sets the mocked time

// Getters for time components that don't require a specific time_t argument
int hour();    // Returns the current mocked hour
int minute();  // Returns the current mocked minute
int second();  // Returns the current mocked second
int day();     // Returns the current mocked day
int weekday(); // Returns the current mocked weekday
int month();   // Returns the current mocked month
int year();    // Returns the current mocked year
time_t now();  // Returns the current mocked time

// Setters for time components
void setTime(time_t t); // Sets the current mocked time to a specific time_t
void setTime(
    int hr, int min, int sec, int day, int month,
    int yr); // Sets the current mocked time to a specific date and time
void adjustTime(long adjustment); // Adjusts the current mocked time by a given
                                  // number of seconds

// Utility functions for time formatting
const char *monthShortStr(
    int month); // Returns the short string representation of a month (1-12)
const char *
monthStr(int month); // Returns the full string representation of a month (1-12)
const char *dayShortStr(int weekday); // Returns the short string representation
                                      // of a weekday (1-7, assuming 1 = Sunday)

#endif // MOCK_TIME_LIB_H
