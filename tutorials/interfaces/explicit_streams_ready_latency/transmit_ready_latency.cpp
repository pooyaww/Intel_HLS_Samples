#include "HLS/hls.h"
#include <stdio.h>   // For printf

#define MAX_ATTEMPTS 3 

// Transmits the results after multiplying data in streams A and B to C.
// Observe that stream C has a readyLatency of 3. 
component bool transmit (ihc::stream_in<float>& data_A, ihc::stream_in<float>& data_B,
                   ihc::stream_out<float, ihc::readyLatency<3> >& C, int stream_size) {
  int i = 0;
  bool pass = true; 
  // read in data from the 2 streams one by one if previous results were successful 
  while (i < stream_size && pass) {
    bool success;
    float x = data_A.tryRead(success);
    if (success) {
      float y = data_B.tryRead(success);
      if (success) {
        int attempts = 0;
        success = false;	
        // reattempt upto MAX_ATTEMPTS time before timing out
        while (attempts < MAX_ATTEMPTS && !success) {
          success = C.tryWrite(x*y);
          if (success) {
	    pass &= success;
            i++;
          } else {
	    attempts++;
            if (attempts == MAX_ATTEMPTS) {
              pass &= success; 
            }
	  }
        }
      }
    }
  }
  return pass;
}

int main() {
  
  bool passed = true;
  
  ihc::stream_in<float> data_A;
  ihc::stream_in<float> data_B;
  ihc::stream_out<float, ihc::readyLatency<3> > C;
  
  int stream_size = 20; // set to 20 as an example 
  
  // store data in stream A and B 
  for (int i = 0; i < stream_size; i++) {
    data_A.write( i*i + i*0.1 );
    data_B.write( i*i*i + i*0.5 );
  }

  passed &= transmit( data_A, data_B, C, stream_size);

  // compare result against actual value
  for (int i = 0; i < stream_size; i++) {
    float result  = C.read();
    float actual  = (i*i + i*0.1)*(i*i*i + i*0.5);
    // due to hardware and emulator differences, the results can have variations which are accounted by a tolerance of 0.8  
    passed &= ((result - actual) < 0.8 && (result - actual) > -0.8);
  }

  // checks if the transmission is successfull.  
  if (passed) {
     printf("PASSED\n");
  } else {
     printf("FAILED\n");
  } 	 
  
  return 0;
}
