// 01_unit_test.cpp
#include "Syslog.h"
#include <unity.h>

void test_unit_test() {
  MockUDP udp;
  Syslog syslog(udp);
  TEST_ASSERT_TRUE(true);
}
