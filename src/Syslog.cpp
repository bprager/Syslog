#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>
#include <TimeLib.h> // Include TimeLib to handle time functionalities

#include "Syslog.h"

// Public Methods //////////////////////////////////////////////////////////////

Syslog::Syslog(UDP &client, uint8_t protocol) {
  this->_client = &client;
  this->_protocol = protocol;
  this->_server = NULL;
  this->_port = 0;
  this->_deviceHostname = SYSLOG_NILVALUE;
  this->_appName = SYSLOG_NILVALUE;
  this->_priDefault = LOG_KERN;
}

Syslog::Syslog(UDP &client, const char* server, uint16_t port, const char* deviceHostname, const char* appName, uint16_t priDefault, uint8_t protocol) {
  this->_client = &client;
  this->_protocol = protocol;
  this->_server = server;
  this->_port = port;
  this->_deviceHostname = (deviceHostname == NULL) ? SYSLOG_NILVALUE : deviceHostname;
  this->_appName = (appName == NULL) ? SYSLOG_NILVALUE : appName;
  this->_priDefault = priDefault;
}

Syslog::Syslog(UDP &client, IPAddress ip, uint16_t port, const char* deviceHostname, const char* appName, uint16_t priDefault, uint8_t protocol) {
  this->_client = &client;
  this->_protocol = protocol;
  this->_ip = ip;
  this->_server = NULL;
  this->_port = port;
  this->_deviceHostname = (deviceHostname == NULL) ? SYSLOG_NILVALUE : deviceHostname;
  this->_appName = (appName == NULL) ? SYSLOG_NILVALUE : appName;
  this->_priDefault = priDefault;
}

Syslog &Syslog::server(const char* server, uint16_t port) {
  this->_server = server;
  this->_port = port;
  return *this;
}

Syslog &Syslog::server(IPAddress ip, uint16_t port) {
  this->_ip = ip;
  this->_server = NULL;
  this->_port = port;
  return *this;
}

Syslog &Syslog::deviceHostname(const char* deviceHostname) {
  this->_deviceHostname = (deviceHostname == NULL) ? SYSLOG_NILVALUE : deviceHostname;
  return *this;
}

Syslog &Syslog::appName(const char* appName) {
  this->_appName = (appName == NULL) ? SYSLOG_NILVALUE : appName;
  return *this;
}

Syslog &Syslog::defaultPriority(uint16_t pri) {
  this->_priDefault = pri;
  return *this;
}

Syslog &Syslog::logMask(uint8_t priMask) {
  this->_priMask = priMask;
  return *this;
}


bool Syslog::log(uint16_t pri, const __FlashStringHelper *message) {
  return this->_sendLog(pri, message);
}

bool Syslog::log(uint16_t pri, const String &message) {
  return this->_sendLog(pri, message.c_str());
}

bool Syslog::log(uint16_t pri, const char *message) {
  return this->_sendLog(pri, message);
}


bool Syslog::vlogf(uint16_t pri, const char *fmt, va_list args) {
  char *message;
  size_t initialLen;
  size_t len;
  bool result;

  initialLen = strlen(fmt);

  message = new char[initialLen + 1];

  len = vsnprintf(message, initialLen + 1, fmt, args);
  if (len > initialLen) {
    delete[] message;
    message = new char[len + 1];

    vsnprintf(message, len + 1, fmt, args);
  }

  result = this->_sendLog(pri, message);

  delete[] message;
  return result;
}

bool Syslog::vlogf_P(uint16_t pri, PGM_P fmt_P, va_list args) {
  char *message;
  size_t initialLen;
  size_t len;
  bool result;

  initialLen = strlen_P(fmt_P);

  message = new char[initialLen + 1];

  len = vsnprintf_P(message, initialLen + 1, fmt_P, args);
  if (len > initialLen) {
    delete[] message;
    message = new char[len + 1];

    vsnprintf(message, len + 1, fmt_P, args);
  }

  result = this->_sendLog(pri, message);

  delete[] message;
  return result;
}


bool Syslog::logf(uint16_t pri, const char *fmt, ...) {
  va_list args;
  bool result;

  va_start(args, fmt);
  result = this->vlogf(pri, fmt, args);
  va_end(args);
  return result;
}

bool Syslog::logf(const char *fmt, ...) {
  va_list args;
  bool result;

  va_start(args, fmt);
  result = this->vlogf(this->_priDefault, fmt, args);
  va_end(args);
  return result;
}

bool Syslog::logf_P(uint16_t pri, PGM_P fmt_P, ...) {
  va_list args;
  bool result;

  va_start(args, fmt_P);
  result = this->vlogf_P(pri, fmt_P, args);
  va_end(args);
  return result;
}

bool Syslog::logf_P(PGM_P fmt_P, ...) {
  va_list args;
  bool result;

  va_start(args, fmt_P);
  result = this->vlogf_P(this->_priDefault, fmt_P, args);
  va_end(args);
  return result;
}

bool Syslog::log(const __FlashStringHelper *message) {
  return this->_sendLog(this->_priDefault, message);
}

bool Syslog::log(const String &message) {
  return this->_sendLog(this->_priDefault, message.c_str());
}

bool Syslog::log(const char *message) {
  return this->_sendLog(this->_priDefault, message);
}

// Helper function to generate IETF timestamp
String generateIETFTimestamp() {
    char buffer[40];
    sprintf(buffer, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", year(), month(), day(), hour(), minute(), second(), millis() % 1000);
    return String(buffer);
}

// Helper function to generate BSD timestamp
String generateBSDTimestamp() {
    char buffer[20];
    sprintf(buffer, "%s %2d %02d:%02d:%02d", monthShortStr(month()), day(), hour(), minute(), second());
    return String(buffer);
}

// Private Methods /////////////////////////////////////////////////////////////

inline bool Syslog::_sendLog(uint16_t pri, const char *message) {
    int result;

    if ((this->_server == NULL && this->_ip == INADDR_NONE) || this->_port == 0)
        return false;

    if ((LOG_MASK(LOG_PRI(pri)) & this->_priMask) == 0)
        return true;

    if ((pri & LOG_FACMASK) == 0)
        pri = LOG_MAKEPRI(LOG_FAC(this->_priDefault), pri);

    if (this->_server != NULL) {
        result = this->_client->beginPacket(this->_server, this->_port);
    } else {
        result = this->_client->beginPacket(this->_ip, this->_port);
    }

    if (result != 1)
        return false;

    this->_client->print('<');
    this->_client->print(pri);
    this->_client->print(F(">"));

    if (this->_protocol == SYSLOG_PROTO_IETF) {
        this->_client->print("1 "); // Version number for IETF
        this->_client->print(generateIETFTimestamp()); // IETF timestamp
        this->_client->print(F(" "));
    } else {
        this->_client->print(generateBSDTimestamp()); // BSD timestamp
        this->_client->print(F(" "));
    }

    this->_client->print(this->_deviceHostname);
    this->_client->print(' ');
    this->_client->print(this->_appName);
    this->_client->print(F(" - - - \xEF\xBB\xBF")); // Non-translatable part
    this->_client->print(message);
    this->_client->endPacket();

    return true;
}

inline bool Syslog::_sendLog(uint16_t pri, const __FlashStringHelper *message) {
    int result;

    if ((this->_server == NULL && this->_ip == INADDR_NONE) || this->_port == 0)
        return false;

    if ((LOG_MASK(LOG_PRI(pri)) & this->_priMask) == 0)
        return true;

    if ((pri & LOG_FACMASK) == 0)
        pri = LOG_MAKEPRI(LOG_FAC(this->_priDefault), pri);

    if (this->_server != NULL) {
        result = this->_client->beginPacket(this->_server, this->_port);
    } else {
        result = this->_client->beginPacket(this->_ip, this->_port);
    }

    if (result != 1)
        return false;

    this->_client->print('<');
    this->_client->print(pri);
    this->_client->print(F(">"));

    if (this->_protocol == SYSLOG_PROTO_IETF) {
        this->_client->print("1 "); // Version number for IETF
        this->_client->print(generateIETFTimestamp()); // IETF timestamp
        this->_client->print(F(" "));
    } else {
        this->_client->print(generateBSDTimestamp()); // BSD timestamp
        this->_client->print(F(" "));
    }

    this->_client->print(this->_deviceHostname);
    this->_client->print(' ');
    this->_client->print(this->_appName);
    this->_client->print(F(" - - - \xEF\xBB\xBF")); // Non-translatable part
    this->_client->print(message);
    this->_client->endPacket();

    return true;
}
