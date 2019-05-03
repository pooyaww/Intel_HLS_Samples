// rtl_inet.h
// These functions are hardware equivalents of the inet.h functions that
// provide conversions between native and network byte-order. 
//
// The HLS and OpenCL compilers use a little-endian word format (as do x86 and ARM 
// CPUs), while network byte order is big-endian. In a little-endian format the
// least significant byte of the word is stored or transmitted first. 
//
// This library is suitable for use in HLS and OpenCL programs on both
// Linux and Windows platforms. To make this possible, the header must include 
// appropriate mapping of types. 
//
// The RTL-based functions do not have mangled names and HLS programs that
// make use of them must tream them as "C" externals. 

#ifndef _RTL_INET_H_
#define _RTL_INET_H_

#ifdef __OPENCL_VERSION__
  typedef ushort rtl_inet_uint16;
  typedef uint   rtl_inet_uint32;
#else
  #include <stdint.h>
  typedef uint16_t rtl_inet_uint16;
  typedef uint32_t rtl_inet_uint32;
#endif

#ifdef __cplusplus
extern "C" {
#endif

rtl_inet_uint16 rtl_htons(rtl_inet_uint16 hostshort);
rtl_inet_uint16 rtl_ntohs(rtl_inet_uint16 netshort);
rtl_inet_uint32 rtl_htonl(rtl_inet_uint32 hostlong);
rtl_inet_uint32 rtl_ntohl(rtl_inet_uint32 netlong);

#ifdef __cplusplus
}
#endif

#endif // _RTL_INET_H_

