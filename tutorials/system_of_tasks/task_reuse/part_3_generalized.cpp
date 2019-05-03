#include <iostream>
#include "HLS/hls.h"
#define N 256

// It is recommended to use ihc::stream_in/ihc::stream_out for communicating
// outside the component and ihc::stream for within the component.
ihc::stream_in<int> s0;
ihc::stream<int> s1, s2;
ihc::stream_out<int> s3;

// If the stream type should also be parametrizable, it can be done as follows:
// Note that templated functions must have their entire definition and
// declaration within the header file or in the source file where they 
// are used.
template <typename stream_type1, stream_type1 &s_in,
          typename stream_type2, stream_type2 &s_out>
void task_function() {
  for (int i = 0; i < N; i++) {
    // This now uses auto so that the task_function can accept any type of
    // stream
    auto input = s_in.read();
    input += 1;
    s_out.write(input);
  }
}

component void foo() {
  // To invoke the task_function, you can either specify the expected type
  // as shown here:
  ihc::launch((task_function<ihc::stream_in<int>, s0, ihc::stream<int>, s1>));  // call (1)
  // Or use the C++ decltype keyword to automatically infer the type as shown
  // here. decltype(X) will return the type of the variable X. The key advantage
  // is that the type of the stream is only ever declared in one place.
  ihc::launch((task_function<decltype(s1), s1, decltype(s2), s2>));             // call (2)
  ihc::launch((task_function<decltype(s2), s2, decltype(s3), s3>));

  ihc::collect((task_function<ihc::stream_in<int>, s0, ihc::stream<int>, s1>));
  ihc::collect((task_function<decltype(s1), s1, decltype(s2), s2>));
  ihc::collect((task_function<decltype(s2), s2, decltype(s3), s3>));
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
