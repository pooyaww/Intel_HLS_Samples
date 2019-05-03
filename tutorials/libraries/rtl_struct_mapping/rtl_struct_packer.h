// This file defines the interface to the RTL-based structure packing library

#ifndef _RTL_STRUCT_PACKER_H_
#define _RTL_STRUCT_PACKER_H_

#include "HLS/hls.h"
#include "HLS/ac_int.h"

#include <cstdint>

#define PACKED_STRUCT(_NAME_,_DEFN_) \
  struct _NAME_ { _DEFN_ } __attribute__((packed))

#define NUM_ARRAY_ELEMENTS(_DECL_) \
  (sizeof(_DECL_) / sizeof(_DECL_[0]))
 
// The packed and unpacked structure declarations are paired - they have exactly the same
// fields.
// Fields of the packed struct are aligned on byte boundaries. 
// In the un-packed struct the alignment of fields is determined by the compiler, which
// usually aligns fields on boundaries which are a multiple of the size of the field
// data type. 

struct struct_a {
  bool    field_1_bool;
  int32_t field_2_int32_t;
  char    field_3_char;
  double  field_4_double;
  uint8_t field_5_uint8_t;
  int16_t field_6_int16_t_array[9];
  int64_t field_7_int64_t;
};

PACKED_STRUCT(packed_struct_a,
  bool    field_1_bool;
  int32_t field_2_int32_t;
  char    field_3_char;
  double  field_4_double;
  uint8_t field_5_uint8_t;
  int16_t field_6_int16_t_array[9];
  int64_t field_7_int64_t;
);

// Given a struct_a, returns the equivalent packed_struct_a
extern "C" {
  packed_struct_a rtl_struct_a_packer(struct_a struct_a_in);
}

#endif // _RTL_STRUCT_PACKER_H_

