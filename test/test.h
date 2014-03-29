#include <stdio.h>

#define FAIL() printf("\nfailure in %s() line %d\n", __func__, __LINE__)
#define _assert(condition) do { if (!(condition)) { return 1; } } while(0)
#define _verify(test) do { int r=test(); total_tests++; if(!r){ printf("."); }else{ total_failures++; printf("F"); } } while(0)
#define _summary() printf("\n%d tests ran\n%d success, %d failure(s).\n", total_tests, (total_tests - total_failures), total_failures)

int total_tests    = 0;
int total_failures = 0;
