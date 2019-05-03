#include "HLS/hls.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "HLS/math.h"
#include "HLS/rand_lib.h"

#define SIZE 10
#define SEED_VAL 1543
#define EPSILON 1e-5f

//////////////////////////////////////////////////////
// Uniform integer random number generator       
//////////////////////////////////////////////////////
component void RngUnif(int lower_range, int upper_range, ihc::stream_out<float> &res) {
       
    
     static RNG_Uniform<float> m_rng(SEED_VAL);

     for (int i=0; i<SIZE; i++){
         float new_rn = m_rng.rand()*(upper_range-lower_range) + lower_range;
         res.write(new_rn);
     }
}

void RngUnifX86(float lower_range, float upper_range, float * res) {
 
     static RNG_Uniform<float> m_rng(SEED_VAL);
     
     for (int i=0; i<SIZE; i++){
         float new_rn = m_rng.rand()*(upper_range-lower_range) + lower_range;
         res[i] = new_rn;
     }     
}


//////////////////////////////////////////////////////
// Float random number generator using CLT algorithm         
//////////////////////////////////////////////////////
component void RngGauss(float new_sigma, float new_mean, ihc::stream_out<float> &res) {

     static RNG_Gaussian<float, ihc::GAUSSIAN_CLT> m_rng(SEED_VAL);
     float new_rn = m_rng.rand()* new_sigma + new_mean;
     res.write(new_rn);
}

void RngGaussX86(float new_sigma, float new_mean, float &res) {

     static RNG_Gaussian<float, ihc::GAUSSIAN_CLT> m_rng(SEED_VAL);
     float new_rn = m_rng.rand()* new_sigma + new_mean;
     res = new_rn;
}

int main() {
  
  ihc::stream_out<float> result_f;
  float res_float[SIZE];
  float test_res_float[SIZE];
  bool passed = true;
  float lower_range = -5.0 ;
  float upper_range = 17.0;
  float new_sigma = 2.0;
  float new_mean = 1.0;

  RngUnif(lower_range, upper_range, result_f);
  RngUnifX86(lower_range, upper_range, test_res_float);
  for (int i = 0; i < SIZE; i++) {
      res_float[i] = result_f.read();

      if (test_res_float[i] != res_float[i]){
         passed = false ;
         break;
      }
  }
  
  printf("Testing RngUnif:\n"); 
  if (passed) 
      printf("PASSED\n");
  else
      printf("FAILED\n");

  passed = true;

  for (int i = 0; i < SIZE; i++) {
      RngGauss(new_sigma, new_mean, result_f);
      RngGaussX86(new_sigma, new_mean, test_res_float[i]);
      res_float[i] = result_f.read();

      if (std::abs(test_res_float[i]-res_float[i]) > EPSILON){
         passed = false ;
         break;
      }
  }

  printf("Testing RngGauss:\n");
  if (passed)
     printf("PASSED\n");
  else
     printf("FAILED\n");
  return 0;
}
