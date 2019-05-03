// The C model of rtl_struct_a_packer is an HLS implementation of
// the RTL-based function. This is used when the HLS code is compiled
// for the host architecture (x86-64).
// The function performs a field-by-field copy of the data from
// the un-packed structure to its packed equivalent. 

#include "rtl_struct_packer.h"

packed_struct_a rtl_struct_a_packer(struct_a struct_a_in) {
  packed_struct_a result;
  result.field_1_bool    = struct_a_in.field_1_bool;
  result.field_2_int32_t = struct_a_in.field_2_int32_t;
  result.field_3_char    = struct_a_in.field_3_char;
  result.field_4_double  = struct_a_in.field_4_double;
  result.field_5_uint8_t = struct_a_in.field_5_uint8_t;
  for (int i = 0, end_i = NUM_ARRAY_ELEMENTS(struct_a_in.field_6_int16_t_array); i < end_i; ++i) {
    result.field_6_int16_t_array[i] = struct_a_in.field_6_int16_t_array[i];
  }
  result.field_7_int64_t = struct_a_in.field_7_int64_t;
  return result;
}

