// This shared-source file consists of an HLS component that makes use of the
// RTL-based struct packing function, and a testbench to drive it. 

#include "HLS/hls.h"
#include "HLS/ac_int.h"

#include "rtl_struct_packer.h"
#include <cstdio>
#include <iostream>
#include <string>

// This component tests the RTL struct packer, taking an un-packed struct_a and
// returning its packed equivalent.
// To do this correctly, the interpretation of both the packed and un-packed
// structure layouts must be correct. 

component packed_struct_a pack_test(struct_a unpacked_struct) {
  return rtl_struct_a_packer(unpacked_struct);
}

void dump_bytes(std::string title, uint8_t * data, int num_bytes);
void dump_element_bytes(uint32_t index, uint8_t * data, int num_bytes);

#define TEST_SIZE 100

int main(void) {

  // This section of the tutorial demonstrates how the location of structure fields is
  struct_a        test_data_in[TEST_SIZE];
  packed_struct_a test_results[TEST_SIZE];

  bool passed = true;

  for (int test = 0; test < TEST_SIZE; ++test) {
    test_data_in[test].field_1_bool    = (rand() & 0x1) ? true : false;
    test_data_in[test].field_2_int32_t = (int32_t)rand();
    test_data_in[test].field_3_char    = (char)(rand() & 0xFF);
    test_data_in[test].field_4_double  = rand();
    test_data_in[test].field_5_uint8_t = rand() & 0xFF;
    for (int i = 0, end_i = NUM_ARRAY_ELEMENTS(struct_a::field_6_int16_t_array); i < end_i; ++i) {
      test_data_in[test].field_6_int16_t_array[i] = (int16_t)(rand() & 0xFFFF);
    }
    test_data_in[test].field_7_int64_t = (int64_t)(((uint64_t)rand() << 32) | (uint64_t)rand());
  }

  for (int test = 0; test < TEST_SIZE; ++test) {
    test_results[test] = pack_test(test_data_in[test]);
  }

  for (int test = 0; test < TEST_SIZE; ++test) {
    bool field_6_int16_t_array_discrepancy = false;
    for (int i = 0, end_i = NUM_ARRAY_ELEMENTS(struct_a::field_6_int16_t_array); i < end_i; ++i) {
      field_6_int16_t_array_discrepancy |= (test_results[test].field_6_int16_t_array[i] != 
                                            test_data_in[test].field_6_int16_t_array[i]);
    }    
    if ((test_results[test].field_1_bool    != test_data_in[test].field_1_bool) ||
        (test_results[test].field_2_int32_t != test_data_in[test].field_2_int32_t) ||
        (test_results[test].field_3_char    != test_data_in[test].field_3_char) ||
        (test_results[test].field_4_double  != test_data_in[test].field_4_double) ||
        (test_results[test].field_5_uint8_t != test_data_in[test].field_5_uint8_t) ||
        field_6_int16_t_array_discrepancy ||
        (test_results[test].field_7_int64_t != test_data_in[test].field_7_int64_t)) {
      passed = false;
      std::cout << "Discrepancy between packed and original data - test# " << test << "\n";
      for (int i = 0; i < sizeof(struct_a); ++i) {
        printf("%.2x ",((uint8_t*)&(test_data_in[test]))[i]);
        if (i % 32 == 31) std::cout << "\n";
      }
      std::cout << "\n";
      for (int i = 0; i < sizeof(packed_struct_a); ++i) {
        printf("%.2x ",((uint8_t*)&(test_results[test]))[i]);
        if (i % 32 == 31) std::cout << "\n";
      }
      std::cout << "\n";
    }
  }

  std::cout << ((passed) ? "PASSED\n" : "FAILED\n");

  return 0;
}



