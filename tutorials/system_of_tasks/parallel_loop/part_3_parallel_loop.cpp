#include "HLS/hls.h"
#include "HLS/task.h"
#include <iostream>
#include <math.h>

constexpr int N_SUM_INTERVAL = 4;
constexpr int N_SAMPLES = 64;

ihc::stream<float> add_res;

float multiply_task(){
  float res = 1.0f; 
  for (int i = 0; i < N_SAMPLES/N_SUM_INTERVAL; ++i){
    res *= add_res.read();
  }
  return res;
}

component float add_mult(ihc::stream_in<float> &sig_in) {
  ihc::launch(multiply_task);
  float add = 0.0f;  
  for (int i = 0; i < N_SAMPLES; ++i){
    if (i % N_SUM_INTERVAL == 0) {
      add = 0.0f;
    }
    add += sig_in.read();
    if ( (i+1) % N_SUM_INTERVAL == 0) {
      add_res.write(add);
    }
  }

  return ihc::collect(multiply_task);
}

int main() {
  ihc::stream_in<float> sig_in;
  for (int i = 1; i <= N_SAMPLES; i++) {
    // Generate a simple sawtooth wave;
    float data = i * 0.05f;
    sig_in.write(data);
  }

  std::cout << "Testing parallel loops.\n";
  float res = add_mult(sig_in);
  std::cout << "Result = " << res <<"\n";
  if (fabs(res - 1.44091e+11) < (res * 1e-5)) {
    std::cout << "PASSED\n";
  } else {
    std::cout << "FAILED\n";
  }

  return 0;
}
