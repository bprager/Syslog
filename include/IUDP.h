// IUDP.h
#ifndef IUDP_H
#define IUDP_H

#include <cstddef>
#include <cstdint>

class IUDP {
public:
  virtual ~IUDP() {}
  virtual uint8_t begin(uint16_t port) = 0;
  virtual void stop() = 0;
  virtual int beginPacket(const char *host, uint16_t port) = 0;
  virtual int beginPacket(IPAddress ip, uint16_t port) = 0;
  virtual int endPacket() = 0;
  virtual size_t write(const uint8_t *buffer, size_t size) = 0;
  virtual int parsePacket() = 0;
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int read(unsigned char *buffer, size_t len) = 0;
  virtual void flush() = 0;
  virtual IPAddress remoteIP() = 0;
  virtual uint16_t remotePort() = 0;

  // Adding print methods
  virtual void print(const String &str) = 0;
  virtual void print(const char *str) = 0;
  virtual void print(int num) = 0;
  virtual void print(unsigned int num) = 0;
  virtual void print(long num) = 0;
  virtual void print(unsigned long num) = 0;
  virtual void print(double num) = 0;
  virtual void print(const __FlashStringHelper *str) = 0;
};

#endif // IUDP_H
