// rtl_inet_cmodels.cpp
// This file contains C-language equivalents for the RTL-based functions of the rtl_inet library. 
// When code that uses these functions is compiled in emulation mode, their C-language 
// implementations will be taken from this file. 

#include "rtl_inet.h"

rtl_inet_uint16 rtl_htons(rtl_inet_uint16 hostshort) { 
  return (  ((hostshort & 0xFF00) >> 8) 
          | ((hostshort & 0x00FF) << 8) );
}

rtl_inet_uint16 rtl_ntohs(rtl_inet_uint16 netshort) { 
  return (  ((netshort & 0xFF00) >> 8) 
          | ((netshort & 0x00FF) << 8) );
}

rtl_inet_uint32 rtl_htonl(rtl_inet_uint32 hostlong) { 
  return (  ((hostlong & 0xFF000000) >> 24) 
          | ((hostlong & 0x00FF0000) >> 8 )
          | ((hostlong & 0x0000FF00) << 8 ) 
          | ((hostlong & 0x000000FF) << 24) );
}

rtl_inet_uint32 rtl_ntohl(rtl_inet_uint32 netlong) { 
  return (  ((netlong & 0xFF000000) >> 24) 
          | ((netlong & 0x00FF0000) >> 8 )
          | ((netlong & 0x0000FF00) << 8 ) 
          | ((netlong & 0x000000FF) << 24) );
}
