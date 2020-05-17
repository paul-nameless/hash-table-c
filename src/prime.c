#include <math.h>

#include "prime.h"


int is_prime(const int x) {
  if (x <= 3) {
    return x > 1;
  }
  if ((x % 2) == 0 || (x % 3) == 0) {
    return 0;
  }

  int i = 5;

  while (i * i <= x) {
    if ((x % i) == 0 || (x % (i+2)) == 0) {
      return 0;
    }
    i += 6;
  }

  return 1;
}

int next_prime(int x) {
  while (is_prime(x) != 1) {
    x++;
  }
  return x;
}
