#include "test.h"
#include <calculator.h>

int test_add() {
  _assert(add(1, 1) == 2);
  return 0;
}
