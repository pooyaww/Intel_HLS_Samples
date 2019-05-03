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
typedef ihc::stream_in <four_pixels, ihc::usesPackets<true>, ihc::bitsPerSymbol<8>, ihc::usesEmpty<true>, ihc::firstSymbolInHighOrderBits<true> > stream_in_pixels;
typedef ihc::stream_out<four_pixels, ihc::usesPackets<true>, ihc::bitsPerSymbol<8>, ihc::usesEmpty<true>, ihc::firstSymbolInHighOrderBits<true> > stream_out_pixels;

void redden_pixel(four_pixels& pixels, unsigned shift_amount) {
  symbol blue  = (pixels & ((four_pixels) 0x00ff00 << shift_amount)) >> shift_amount;
  symbol green = (pixels & ((four_pixels) 0xff0000 << shift_amount)) >> shift_amount;
  if (blue > 200 || green > 200) {
    pixels |= (((four_pixels) 0x0000ff) << shift_amount); // R symbol = 0xff
  }
}

component void redden(stream_in_pixels& in_pixels, stream_out_pixels& out_pixels) {
  bool start_of_packet = false;
  bool end_of_packet   = false;
  int  empty           = 0;

  while (!end_of_packet) {
    // read in next pixels
    four_pixels pixels = in_pixels.read(start_of_packet, end_of_packet, empty);
    
    // set valid R symbols to 0xff if the blue or green symbols exceed the threshold
    // pixel 0
    unsigned shift_amount = 0*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 0
    redden_pixel(pixels, shift_amount);

    // pixel 1
    if(!end_of_packet || empty/SYMBOLS_PER_PIXEL < 3) {
      shift_amount = 1*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 24
      redden_pixel(pixels, shift_amount);
    }

    // pixel 3
    if(!end_of_packet || empty/SYMBOLS_PER_PIXEL < 2) {
      shift_amount = 2*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 48
      redden_pixel(pixels, shift_amount);
    }

    // pixel 4
    if(!end_of_packet || empty/SYMBOLS_PER_PIXEL < 1) {
      shift_amount = 3*BITS_PER_PIXEL*SYMBOLS_PER_PIXEL; // 72
      redden_pixel(pixels, shift_amount);
    }

    // write out result
    out_pixels.write(pixels, start_of_packet, end_of_packet, empty);
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
    bool start_of_packet = (beat == 0);
    bool end_of_packet   = (beat == num_beats - 1);
    int empty = 6; // the number of invalid symbols must be a multiplication of the number of symbols per pixel in all circumstances, e.g. 2 empty pixels * 3 symbols per pixel
    
    four_pixels pixels = 0;
    for (unsigned pixel_offset = 0; pixel_offset < (end_of_packet ? empty/SYMBOLS_PER_PIXEL : PIXELS_PER_BEAT); ++pixel_offset) {
      unsigned shift_amount = pixel_offset * BITS_PER_PIXEL;
      pixel curr_pixel = rand(); // randomize each pixel
      pixels |= (((four_pixels) curr_pixel) << shift_amount);
    }

    in_pixels.write(pixels, start_of_packet, end_of_packet, empty);
  }

  // call component
  redden(in_pixels, out_pixels);

  // check that valid R symbols are set to 0xff if the green or blue symbols exceed the threshold
  bool passed = true;
  bool start_of_packet = false;
  bool end_of_packet   = false;
  int  empty_out       = 0;

  while (!end_of_packet) {
    four_pixels pixels = out_pixels.read(start_of_packet, end_of_packet, empty_out);

    for (unsigned pixel_offset = 0; pixel_offset < (end_of_packet ? empty_out/SYMBOLS_PER_PIXEL : PIXELS_PER_BEAT); ++pixel_offset) {
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
