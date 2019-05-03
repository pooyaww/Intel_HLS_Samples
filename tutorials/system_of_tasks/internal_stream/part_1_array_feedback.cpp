#include "HLS/hls.h"
#include "HLS/task.h"
#include <iostream>
#include <math.h>

constexpr int N_SIZE = 32;

// Simple component implementing a FIR filter
component float feedback() {
  float res = 0.0f;
  float buffer[N_SIZE] = {};
  for (int i = 0; i < N_SIZE; ++i) {
    for (int j = i; j < N_SIZE; ++j) {
      float previous = 0.0f;
      if (i != 0) {
        previous = buffer[j]; // read from buffer
      }
      float x = i * previous + j;
      res += x;
      if (j > i) {
        buffer[j] = x; // write to buffer
      }
    }
  }
  return res;
}

int main() {
  float res = feedback();
  std::cout << "Result of array implementation: " << res <<"\n";
  if (fabs(res - 7.3916e+35) < (res * 1e-6)) {
    std::cout << "PASSED\n";
  } else {
    std::cout << "FAILED\n";
  }

  return 0;
}
