#ifndef I_TIME_H
#define I_TIME_H

#include <cstdint>

class ITime {
public:
  virtual void setTime(int hr, int min, int sec, int day, int month,
                       int yr) = 0;
  virtual long now() = 0;
  virtual int hour(long time) = 0;
  virtual int minute(long time) = 0;
  virtual int second(long time) = 0;
  virtual int day(long time) = 0;
  virtual int month(long time) = 0;
  virtual int year(long time) = 0;

  virtual ~ITime() {}
};

#endif // I_TIME_H
