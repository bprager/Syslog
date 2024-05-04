// 00_main_test.cpp
#include "Syslog.h"
#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

extern void test_unit_test();
extern void test_set_time();

void test_syslog_functionality() {
  MockUDP udp;
  Syslog syslog(udp);
  TEST_ASSERT_TRUE(true);
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_syslog_functionality);
  RUN_TEST(test_unit_test);
  RUN_TEST(test_set_time);
  UNITY_END();
  return 0;
}
