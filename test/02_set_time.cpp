#include <iostream>
#include <unity.h>

#include "mocks/MockTimeLib.h"

void setFixedTestTime() {
  std::cout << "Setting time..." << std::endl;
  setTime(16, 34, 56, 13, 4, 2021); // Set time to April 13, 2021, 16:34:56
}

// check if a fixed time can be properly set
void test_set_time() {
  setFixedTestTime();
  std::cout << "Time set. Checking components..." << std::endl;
  TEST_ASSERT_EQUAL(16, hour());
  std::cout << "Hour checked." << std::endl;
  TEST_ASSERT_EQUAL(34, minute());
  std::cout << "Minute checked." << std::endl;
  TEST_ASSERT_EQUAL(56, second());
  std::cout << "Second checked." << std::endl;
  TEST_ASSERT_EQUAL(13, day());
  std::cout << "Day checked." << std::endl;
  TEST_ASSERT_EQUAL(4, month());
  std::cout << "Month checked." << std::endl;
  TEST_ASSERT_EQUAL(2021, year());
  std::cout << "Year checked." << std::endl;
}
