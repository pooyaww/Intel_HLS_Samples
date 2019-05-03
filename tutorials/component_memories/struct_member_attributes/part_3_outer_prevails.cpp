#include "HLS/hls.h"
#include <stdio.h>

// The following struct contains member variables with attributes.
// These attributes will affect the memory system that is generated
// by the compiler.
class State {
public:
  static constexpr auto MEM_SIZE = 16;

  int register_attr[MEM_SIZE] hls_register;
  int register_no_attr;
  int memory[MEM_SIZE] hls_memory;
  int small_memory hls_memory;

  // The initialization code is not relevant to this example.
  // It prevents the compiler from optimizing away access to this struct.
  State() : register_attr{0}, register_no_attr{0}, small_memory{0} {
    for (auto i = 0; i < MEM_SIZE; i++) {
      register_attr[i] = i;
      register_no_attr += i;
      memory[i] = MEM_SIZE - i;
      small_memory *= i;
    }
  }
};

// Helper function to access all members of State.
int do_something(const State& S, int idx) {
  return S.register_attr[idx] +
         S.register_no_attr +
         S.memory[idx] +
         S.small_memory;
}

// For this component, the attributes of State will be ignored because we apply
// an attribute to the instantiantion of this struct. A single memory will be
// created for the contents of MyState.
component int outer_prevails(int idx) {
  State MyState hls_memory;
  return do_something(MyState, idx);
}
int outer_prevails_cpu(int idx) {
  State MyState hls_memory;
  return do_something(MyState, idx);
}

int main(int argc, char **argv) {
  bool ok = true;
  for (int i = 0; i < 10; i++)
    ok &= (outer_prevails(i) == outer_prevails_cpu(i));

  if(ok)
    printf("PASSED\n");
  else
    printf("FAILED\n");

  return 0;
}
