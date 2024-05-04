// MockUDP.h
#ifndef MOCK_UDP_H
#define MOCK_UDP_H

#include <ArduinoFake.h> // This must come before any usage of Arduino types

#include "IUDP.h"

class MockUDP : public IUDP {
public:
  uint8_t begin(uint16_t port) override {
    return 1; // Example implementation
  }

  void stop() override {
    // Mock behavior
  }

  // Implement all the print methods required by IUDP
  void print(const String &str) override {
    // Optionally log the string or simply do nothing
  }

  void print(const char *str) override {
    // Optionally log the string or simply do nothing
  }

  void print(int num) override {
    // Optionally log the number or simply do nothing
  }

  void print(unsigned int num) override {
    // Optionally log the number or simply do nothing
  }

  void print(long num) override {
    // Optionally log the number or simply do nothing
  }

  void print(unsigned long num) override {
    // Optionally log the number or simply do nothing
  }

  void print(double num) override {
    // Optionally log the number or simply do nothing
  }

  int beginPacket(const char *host, uint16_t port) override {
    return 1; // Simulate a successful packet initialization
  }

  int beginPacket(IPAddress ip, uint16_t port) override {
    return 1; // Simulate a successful packet initialization
  }

  int endPacket() override {
    return 1; // Simulate a successful packet send
  }

  size_t write(const uint8_t *buffer, size_t size) override {
    return size; // Simulate writing all bytes successfully
  }

  int parsePacket() override {
    return 0; // Simulate no packet available to parse
  }

  int available() override {
    return 0; // Simulate no data available
  }

  int read() override {
    return -1; // Simulate end of data
  }

  int read(unsigned char *buffer, size_t len) override {
    return 0; // Simulate no bytes read
  }

  void flush() override {
    // Simulate flushing the data
  }

  IPAddress remoteIP() override {
    return IPAddress(192, 168, 1, 1); // Return a dummy IP address
  }

  uint16_t remotePort() override {
    return 1234; // Return a dummy port number
  }

  void print(const __FlashStringHelper *str) override {
    Serial.println(str); // Just print the string to the serial monitor for now
    // In actual unit tests, you might log this to a string buffer or similar
    // for verification
  }
};

#endif // MOCK_UDP_H
