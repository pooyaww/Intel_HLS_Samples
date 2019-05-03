#include "HLS/hls.h"
#include "HLS/task.h"
#include <iostream>
#include <utility>

ihc::stream_in<float> sig_in;
ihc::stream<float> filt_in;
ihc::stream<float> filt_out;
/// This stream is parallel to filter, which contain a deep pipeline for
/// calculating the dot product.
/// Therefore, this stream needs to be buffered with sufficient capacity.
/// Otherwise, the throughput will significantly drop.
/// The hardware can still be functional without the buffer in this case.
ihc::stream<float> pass_through;
ihc::stream_out< std::pair<float, float> > comb_out;

constexpr int N_TAPS = 16;
constexpr int N_FRAME = 128;

/// Simple task that copies the input stream and send to 2 downstreams.
void splitter() {
  for (int i = 0; i < N_FRAME; i++) {
    float data = sig_in.read();
    filt_in.write(data);
    pass_through.write(data);
  }
}

/// Simple task implementing a FIR filter
void filter() {
  static const float coeffs[N_TAPS] = {
    0.007537995674188784f,
    -0.011211077287440443f,
    0.06365912824643992f,
    0.10016861585467923f,
    -0.05201892745373349f,
    -0.2194681135600675f,
    -0.0767203890034225f,
    0.21884627232526455f,
    0.21884627232526455f,
    -0.0767203890034225f,
    -0.2194681135600675f,
    -0.05201892745373349f,
    0.10016861585467923f,
    0.06365912824643992f,
    -0.011211077287440443f,
    0.007537995674188777f,
  };
  static float taps[N_TAPS];

  for (int i = 0; i < N_FRAME; i++) {
    float data_in = filt_in.read();
    float y = 0.0f;

    // shift in input data
#pragma unroll
    for (int i = (N_TAPS - 1); i > 0; --i) {
      taps[i] = taps[i - 1];
    }
    taps[0] = data_in;

    // accumulate over all taps for this particular channel
#pragma unroll
    for (int i = (N_TAPS - 1); i >= 0; --i) {
      y += taps[i] * coeffs[i];
    }

    filt_out.write(y);

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
  ihc::launch(filter);
  ihc::launch(merger);

  ihc::collect(splitter);
  ihc::collect(filter);
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
    std::cout << pair.first << " , " << pair.second << std::endl;
  }

  std::cout << "Complete.\n";

  return 0;
}
