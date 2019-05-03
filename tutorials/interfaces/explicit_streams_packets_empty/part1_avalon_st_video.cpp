#include "HLS/hls.h"
#include "HLS/ac_int.h"
#include "HLS/stdio.h"

#define PIXELS_PER_BEAT 4
#define SYMBOLS_PER_PIXEL 3 // RGB pixel
#define BITS_PER_SYMBOL 8
#define BITS_PER_PIXEL (BITS_PER_SYMBOL*SYMBOLS_PER_PIXEL)

typedef ac_int<BITS_PER_PIXEL*PIXELS_PER_BEAT, false> four_pixels;
typedef ac_int<BITS_PER_PIXEL, false> pixel;
typedef ac_int<BITS_PER_SYMBOL, false> symbol;
typedef ihc::stream_in <four_pixels, ihc::bitsPerSymbol<8>, ihc::firstSymbolInHighOrderBits<false> > stream_in_pixels;
typedef ihc::stream_out<four_pixels, ihc::bitsPerSymbol<8>, ihc::firstSymbolInHighOrderBits<false> > stream_out_pixels;

void redden_pixel(four_pixels& pixels, unsigned shift_amount) {
  symbol blue  = (pixels & ((four_pixels) 0x00ff00 << shift_amount)) >> shift_amount;
  symbol green = (pixels & ((four_pixels) 0xff0000 << shift_amount)) >> shift_amount;
  if (blue > 200 || green > 200) {
    pixels |= (((four_pixels) 0x0000ff) << shift_amount); // R symbol = 0xff
  }
}

component void redden(stream_in_pixels& in_pixels, stream_out_pixels& out_pixels, unsigned num_pixels) {
  unsigned num_beats = (num_pixels + PIXELS_PER_BEAT - 1)/PIXELS_PER_BEAT; // equivalent to ceil((double) num_pixels/PIXELS_PER_BEAT)
  unsigned num_invalid = PIXELS_PER_BEAT - num_pixels % PIXELS_PER_BEAT;

  for (unsigned beat = 0; beat < num_beats; ++beat) {
    bool last_beat = (beat == num_beats - 1);
    
    // read in next pixels
    four_pixels pixels = in_pixels.read();
    
    // set valid R symbols to 0xff if the blue or green symbols exceed the threshold
    // pixel 0
    unsigned shift_amount = 0*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 0
    redden_pixel(pixels, shift_amount);

    // pixel 1
    if(!last_beat || num_invalid < 3) {
      shift_amount = 1*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 24
      redden_pixel(pixels, shift_amount);
    }

    // pixel 3
    if(!last_beat || num_invalid < 2) {
      shift_amount = 2*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 48
      redden_pixel(pixels, shift_amount);
    }

    // pixel 4
    if(!last_beat || num_invalid < 1) {
      shift_amount = 3*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 72
      redden_pixel(pixels, shift_amount);
    }

    // write out result
    out_pixels.write(pixels);
  }
}

int main(void) {

  // video streams
  stream_in_pixels  in_pixels;
  stream_out_pixels out_pixels;

  // test video packet
  unsigned num_pixels = 1366;
  unsigned num_beats = ceil((double) num_pixels/PIXELS_PER_BEAT);

  // randomize pixel data
  srand(3);
  for (unsigned beat = 0; beat < num_beats; ++beat) {
    four_pixels pixels = 0;
    for (unsigned pixel_offset = 0; pixel_offset < ((beat == num_beats - 1) ? num_pixels % PIXELS_PER_BEAT : PIXELS_PER_BEAT); ++pixel_offset) {
      unsigned shift_amount = pixel_offset * BITS_PER_PIXEL;
      pixel curr_pixel = rand(); // randomize each pixel
      pixels |= (((four_pixels) curr_pixel) << shift_amount);
    }

    in_pixels.write(pixels);
  }

  // call component
  redden(in_pixels, out_pixels, num_pixels); 

  // check that R symbols are set to 0xff if the green or blue symbols exceed the threshold
  bool passed = true;

  for (unsigned beat = 0; beat < num_beats; ++beat) {
    four_pixels pixels = out_pixels.read();

    for (unsigned pixel_offset = 0; pixel_offset < ((beat == num_beats - 1) ? num_pixels % PIXELS_PER_BEAT : PIXELS_PER_BEAT); ++pixel_offset) {
      unsigned shift_amount = pixel_offset * BITS_PER_PIXEL;
      symbol blue  = (pixels & ((four_pixels) 0x00ff00 << shift_amount)) >> shift_amount;
      symbol green = (pixels & ((four_pixels) 0xff0000 << shift_amount)) >> shift_amount;

      if (blue > 200 || green > 200) {
        passed &= ((pixels |= (((four_pixels) 0x0000ff) << shift_amount)) == pixels); // R symbol = 0xff
      }
    }
  }

  if (passed) {
    printf("PASSED\n"); 
  } else {
    printf("FAILED\n"); 
  }

  return 0;
}
