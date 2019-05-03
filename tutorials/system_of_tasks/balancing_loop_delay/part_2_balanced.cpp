#include "HLS/hls.h"
#include "HLS/task.h"
#include <iostream>
#include <utility>

#define SIDE_FIFO_CAPACITY 20

ihc::stream_in<float> sig_in;
ihc::stream<float> filt_in;
ihc::stream<float> filt_out;
/// This stream is parallel to delay_block, which contain some sofware delay.
/// Therefore, this stream needs to be buffered with sufficient capacity.
/// Otherwise, the hardware will hang, and will not produce any output.
ihc::stream<float, ihc::buffer<SIDE_FIFO_CAPACITY> > pass_through;
ihc::stream_out< std::pair<float, float> > comb_out;

constexpr int N_DELAY = 16;
constexpr int N_FRAME = 128;

/// Simple task that copies the input stream and send to 2 downstreams.
void splitter() {
  for (int i = 0; i < N_FRAME; i++) {
    float data = sig_in.read();
    filt_in.write(data);
    pass_through.write(data);
  }
}

/// Simple task that have write delayed for a few iterations.
void delay_block() {
  float shift_register[N_DELAY]; // shift register to delay the output
  unsigned pos = 0; // Current position of the data to write out

  for (int i = 0; i < N_DELAY + N_FRAME; i++) {
    float data = shift_register[N_DELAY-1];
#pragma unroll
    for (int j = N_DELAY-1; j > 0; j--) {
      shift_register[j] = shift_register[j-1];
    }
    if (i < N_FRAME)
      shift_register[0] = filt_in.read();
    if (i >= N_DELAY)
      filt_out.write(data);
  }
}

/// Simple task that combines the data from two streams.
void merger() {
  for (int i = 0; i < N_FRAME; i++) {
    float data_orig = pass_through.read();
    float data_filt = filt_out.read();
    comb_out.write(std::make_pair(data_orig, data_filt));
  }
}

/// Simple top-level component that just calls all sub-tasks.
component void top_level() {

  ihc::launch(splitter);
  ihc::launch(delay_block);
  ihc::launch(merger);

  ihc::collect(splitter);
  ihc::collect(delay_block);
  ihc::collect(merger);

}


int main() {
  for (int i = 0; i < N_FRAME; i++) {
    // Generate a simple sawtooth wave;
    float data = (abs((i % 9) - 4) - 2) * 1.0f;
    sig_in.write(data);
  }

  std::cout << "Input data loaded.\n";

  top_level();

  std::cout << "Run complete. Reading results.\n";

  for (int i = 0; i < N_FRAME; i++) {
    auto pair = comb_out.read();
  }

  std::cout << "Complete.\n";

  return 0;
}
