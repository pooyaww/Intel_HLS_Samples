#include <iostream>
#include "HLS/hls.h"
#define N 256

// It is recommended to use ihc::stream_in/ihc::stream_out for communicating
// outside the component and ihc::stream for within the component. However, in
// this example, since the generalized task_function can only take ihc::stream
// as a template reference parameter, we must use ihc::stream for communicating
// with the external system.
ihc::stream<int> s0, s1, s2, s3;

// Task functions must either directly use the stream in the body of the
// function or provided as template reference parameters as shown here.
// Note that templated functions must have their entire definition and
// declaration within the header file or in the source file where they 
// are used.
template <ihc::stream<int> &s_in, ihc::stream<int> &s_out>
void task_function() {
  for (int i = 0; i < N; i++) {
    int input = s_in.read();
    input += 1;
    s_out.write(input);
  }
}

component void foo() {
  ihc::launch((task_function<s0, s1>));
  ihc::launch((task_function<s1, s2>));
  ihc::launch((task_function<s2, s3>));
  ihc::collect((task_function<s0, s1>));
  ihc::collect((task_function<s1, s2>));
  ihc::collect((task_function<s2, s3>));
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
