#include <iostream>
#include "HLS/hls.h"
#define N 256

// It is recommended to use ihc::stream_in/ihc::stream_out for communicating
// outside the component and ihc::stream for within the component. However, in
// this example, since the generalized task_function can only take ihc::stream
// as a reference parameter, we must use ihc::stream for communicating
// with the external system.
ihc::stream<int> s0, s1, s2, s3;

// Functions that are launched cannot take reference parameters so to reuse
// this function (to instantiate three copies of the HW), we must create a
// wrapper for each instance (connected to different streams)
void task_function(ihc::stream<int> &s_in, ihc::stream<int> &s_out) {
  for (int i = 0; i < N; i++) {
    int input = s_in.read();
    input += 1;
    s_out.write(input);
  }
}

void task_1() {
  task_function(s0, s1);
}

void task_2() {
  task_function(s1, s2);
}

void task_3() {
  task_function(s2, s3);
}

component void foo() {
  ihc::launch(task_1);
  ihc::launch(task_2);
  ihc::launch(task_3);
  ihc::collect(task_1);
  ihc::collect(task_2);
  ihc::collect(task_3);
}

int main() {
  for (int i = 0; i < N; i++) {
    s0.write(i);
  }

  foo();

  bool pass = true;
  for (int i = 0; i < N; i++) {
    int result = s3.read();
    if (result != i + 3) {
      std::cout << "Result mismatch at index: " << i << "\n";
      pass = false;
    }
  }

  if (pass) std::cout << "PASSED\n";
  else std::cout << "FAILED\n";

  return 0;
}

