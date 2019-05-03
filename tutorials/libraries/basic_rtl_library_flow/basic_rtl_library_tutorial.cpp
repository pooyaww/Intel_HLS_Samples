#include <stdio.h>
#include <stdlib.h>
#include "HLS/hls.h"

#include "rtl_inet.h"

#ifdef _WIN64
  #include <winsock2.h>
#else
  #include <arpa/inet.h>
#endif

#include <stdint.h>

component void bulk_rtl_ntohl(const uint32_t* net_longs, uint32_t* host_longs, int N) {
  for (int i = 0; i < N; ++i) {
    host_longs[i] = rtl_ntohl(net_longs[i]);
  }
}

component void bulk_rtl_ntohs(const uint16_t* net_shorts, uint16_t* host_shorts, int N) {
  for (int i = 0; i < N; ++i) { 
    host_shorts[i] = rtl_ntohs(net_shorts[i]);
  }
}


component void bulk_rtl_htonl(const uint32_t* host_longs, uint32_t* net_longs, int N) {
  for (int i = 0; i < N; ++i) {
    net_longs[i] = rtl_htonl(host_longs[i]);
  }
}

component void bulk_rtl_htons(const uint16_t* host_shorts, uint16_t* net_shorts, int N) {
  for (int i = 0; i < N; ++i) {
    net_shorts[i] = rtl_htons(host_shorts[i]);
  }
}

#define TEST_SIZE 128
#define SEED 4

int main(void) {

  uint32_t net_longs[TEST_SIZE];
  uint32_t host_longs[TEST_SIZE];
  uint16_t net_shorts[TEST_SIZE];
  uint16_t host_shorts[TEST_SIZE];

  bool passed = true;

  // Prepare the input data
  srand(SEED);
  for (int i = 0; i < TEST_SIZE; ++i) {
    net_longs[i] = rand();
    net_shorts[i] = rand() & 0xFFFF;
  }

  // Run the network-to-host address byte order conversion components
  bulk_rtl_ntohl(net_longs, host_longs, TEST_SIZE);
  bulk_rtl_ntohs(net_shorts, host_shorts, TEST_SIZE);

  // Check the output
  for (int i = 0; i < TEST_SIZE; ++i) {
    if (host_longs[i] != ntohl(net_longs[i])) {
      passed = false;
      printf("ERROR: host_longs[%d] = 0x%.8x != 0x%.8x\n", i, host_longs[i], (uint32_t)ntohl(net_longs[i]));
    }
    if (host_shorts[i] != ntohs(net_shorts[i])) {
      passed = false;
      printf("ERROR: host_shorts[%d] = 0x%.4x != 0x%.4x\n", i, host_shorts[i], ntohs(net_shorts[i]));
    }
  }

  // Run the host-to-network address byte order conversion components
  bulk_rtl_htonl(host_longs, net_longs, TEST_SIZE);
  bulk_rtl_htons(host_shorts, net_shorts, TEST_SIZE);

  // Check the output
  for (int i = 0; i < TEST_SIZE; ++i) {
    if (net_longs[i] != htonl(host_longs[i])) {
      passed = false;
      printf("ERROR: net_longs[%d] = 0x%.8x != 0x%.8x\n", i, net_longs[i], (uint32_t)htonl(host_longs[i]));
    }
    if (net_shorts[i] != htons(host_shorts[i])) {
      passed = false;
      printf("ERROR: net_shorts[%d] = 0x%.4x != 0x%.4x\n", i, net_shorts[i], htons(host_shorts[i]));
    }
  }

  if (passed) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  return 0;
}
