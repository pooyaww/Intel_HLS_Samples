#include "HLS/hls.h"
#include "HLS/task.h"
#include <iostream>
#include "HLS/math.h"
#include "HLS/extendedmath.h"
#define N 3

struct D3Vector {
  float d[3];
};

ihc::stream_in<D3Vector> inputs;

hls_max_concurrency(0)
component float dut() {
  D3Vector new_item, coef, coef1;
  coef.d[0] = .2;coef.d[1] = .3;coef.d[2] = .4;
  coef1.d[0] = .6;coef1.d[1] = .7;coef1.d[2] = .8;

  auto item = inputs.read();
  new_item.d[0] = sqrt(item.d[0] * coef.d[0] + item.d[1] * coef.d[1] + item.d[2] * coef.d[2]);
  item = inputs.read();
  new_item.d[1] = sqrt(item.d[0] * coef.d[0] + item.d[1] * coef.d[1] + item.d[2] * coef.d[2]);
  item = inputs.read();
  new_item.d[2] = sqrt(item.d[0] * coef.d[0] + item.d[1] * coef.d[1] + item.d[2] * coef.d[2]); 
  
  return sqrt(new_item.d[0] * coef1.d[0] + new_item.d[1] * coef1.d[1] + new_item.d[2] * coef1.d[2]);
}

int main() {
  // initialize input D3Vector
  const float test_vecs[N][3] = {
    {.49, .26, .82},
    {.78, .43, .92},
    {.17, .72, .34}
  };

  float results[10];
  for (int i = 0; i < 10; i++) {
    D3Vector items[N];
    for (int i = 0; i < N; i++) {
      items[i].d[0] = test_vecs[i][0];
      items[i].d[1] = test_vecs[i][1];
      items[i].d[2] = test_vecs[i][2];
      inputs.write(items[i]);
    }
    ihc_hls_enqueue(&results[i], &dut);
  }
  ihc_hls_component_run_all(dut);

  bool succ = true;
  for (int i = 1; i < 10; i++) {
    if (results[i] != results[i-1]) succ = false;
  }
  if (succ) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}
