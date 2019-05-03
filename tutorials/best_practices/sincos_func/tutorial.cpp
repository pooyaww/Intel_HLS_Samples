// See README for a description of this tutorial
#include "HLS/hls.h"
#include <stdio.h>
// Use HLS/math.h instead of math.h to get an FPGA optimized math library
#include "HLS/math.h"
// Use HLS/extendedmath.h which contains sinpi/cospi
#include "HLS/extendedmath.h"

#include <stdlib.h>

#define SAMPLE_N 28

// Test angles that goes to various ranges.

float test_angle_f[SAMPLE_N] = {
    0.00001f, 0.0001f, 0.001f, 0.01f, 0.025f, 0.05f, 0.1f,  0.2f,  0.4f,  0.8f,
    1.2f,     1.6f,    2.0f,   2.4f,  2.8f,   3.2f,  3.6f,  5.0f,  10.0f, 25.0f,
    100.0f,   1000.0f, 1e+4f,  1e+5f, 1e+6f,  1e+7f, 2e+7f, 5e+7f
};

// Test Thresholds
static const float threshold_f = 1e-7f;

// Simple components that calculate sin/cos directly,
// or renormalize and use sinpi/cospi
component float get_sin(float x) { return sinf(x); }

component float get_sin_renorm(float x) {
  return sinpif(x * M_PI_INV_F);
}


// Testing the numerical difference for single percision
int main() {
  bool passed = true;
  float sin_ref[SAMPLE_N];
  float sin_norm[SAMPLE_N];


  // Use enqueued call to speed up simulator test
  for (int i = 0; i < SAMPLE_N; ++i) {
    ihc_hls_enqueue(&sin_ref[i], &get_sin, test_angle_f[i]);
    ihc_hls_enqueue(&sin_norm[i], &get_sin_renorm, test_angle_f[i]);
  }
  ihc_hls_component_run_all(&get_sin);
  ihc_hls_component_run_all(&get_sin_renorm);

  for (int i = 0; i < SAMPLE_N; ++i) {
    float x = test_angle_f[i];
    //float diff = (sin_norm[i] - sin_ref[i]);
    float diff = (sin_norm[i] - sinf(x));
    printf("float: x=%8g sin(x): % .8f sinpi(x/pi): % .8f diff % .5e \n", x,
           sin_ref[i], sin_norm[i], diff);
    if (fabsf(diff) / fmaxf(fabsf(x), 1) > threshold_f) {
      printf("DIFF ABOVE THRESHOLD\n");
      passed = false;
    }
  }
  printf("\n");


  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }
  return 0;
}
