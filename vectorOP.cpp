#include "PPintrin.h"
#include <stdlib.h>
#include <math.h>

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

void clampedExpVector(float *values, int *exponents, float *output, int N)
{
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
    __pp_vec_float x;
    __pp_vec_int y;
    __pp_vec_float result;
    __pp_vec_int count;
    __pp_vec_int zero = _pp_vset_int(0);
    //float onefloat = 1.f;
    int zeroint = 1;
    //float limitfloat = 9.999999f;
    __pp_vec_int one = _pp_vset_int(1);
    __pp_vec_float limit = _pp_vset_float(9.999999f);
    __pp_mask maskAll, maskexpiszero, maskexpisnotzero, maskcountnotzero, maskresultoutrange, maskand;
    float * limitfloat;
    limitfloat = (float *)malloc(VECTOR_WIDTH*sizeof(float));
    for (int i = 0; i<VECTOR_WIDTH; i++) {
        limitfloat[i] = 9.999999f;
    }
    float *onefloat;
    onefloat = (float *)malloc(VECTOR_WIDTH*sizeof(float));
    for (int i = 0; i<VECTOR_WIDTH; i++) {
        onefloat[i] = 1.f;
    }
    int a = N/VECTOR_WIDTH;
    int b = N%VECTOR_WIDTH;

    float * limitfloat_out;
    limitfloat_out = (float *)malloc(b*sizeof(float));
    for (int i = 0; i<b; i++) {
        limitfloat_out[i] = 9.999999f;
    }
    float *onefloat_out;
    onefloat_out = (float *)malloc(b*sizeof(float));
    for (int i = 0; i<b; i++) {
        onefloat_out[i] = 1.f;
    }
    
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
      if (i == a*VECTOR_WIDTH) {


          
          maskAll = _pp_init_ones(b);
          
          maskexpiszero = _pp_init_ones(0);
          
          maskcountnotzero = _pp_init_ones(0);
          
          maskresultoutrange = _pp_init_ones(0);
          
          maskand = _pp_init_ones(b);
          
          _pp_vload_float(x, values + i, maskAll); // float x = values[i];
          
          _pp_vload_int(y, exponents + i, maskAll); // int y = exponents[i];
          
          _pp_veq_int(maskexpiszero, y, zero, maskAll); // if (y == 0)
          
          _pp_vload_float(result, onefloat_out, maskexpiszero); // output[i] = 1.f;
          
          maskexpisnotzero = _pp_mask_not(maskexpiszero); // else
          
          maskexpisnotzero = _pp_mask_and(maskexpisnotzero,maskand);
          
          _pp_vload_float(result, values + i, maskexpisnotzero); // float result = x;
          
          _pp_vsub_int(count, y, one, maskexpisnotzero); // int count = y - 1;
          
          _pp_vgt_int(maskcountnotzero ,count ,zero ,maskexpisnotzero);
          int numgtzero = _pp_cntbits(maskcountnotzero);

          while (numgtzero > 0) {
              _pp_vmult_float(result, result, x, maskcountnotzero); //result *= x;
              _pp_vsub_int(count, count, one, maskcountnotzero); // count--;
              
              _pp_vgt_int(maskcountnotzero ,count ,zero ,maskexpisnotzero);
              numgtzero = _pp_cntbits(maskcountnotzero);
          }

          _pp_vgt_float(maskresultoutrange,result, limit, maskexpisnotzero);// if (result > 9.999999f)
          
          
          _pp_vload_float(result, limitfloat_out, maskresultoutrange);// result = 9.999999f;
          

          _pp_vstore_float(output + i, result, maskAll);// output[i] = result;
          
          //free(limitfloat);
          //free(onefloat);
      }
      else{
          maskAll = _pp_init_ones();

          maskexpiszero = _pp_init_ones(0);
          
          maskcountnotzero = _pp_init_ones(0);
          
          maskresultoutrange = _pp_init_ones(0);
          
          _pp_vload_float(x, values + i, maskAll); // float x = values[i];
          
          _pp_vload_int(y, exponents + i, maskAll); // int y = exponents[i];
          
          _pp_veq_int(maskexpiszero, y, zero, maskAll); // if (y == 0)
          
          _pp_vload_float(result, onefloat, maskexpiszero); // output[i] = 1.f;
          
          maskexpisnotzero = _pp_mask_not(maskexpiszero); // else
          
          _pp_vload_float(result, values + i, maskexpisnotzero); // float result = x;
          
          _pp_vsub_int(count, y, one, maskexpisnotzero); // int count = y - 1;
          
          _pp_vgt_int(maskcountnotzero ,count ,zero ,maskexpisnotzero);
          int numgtzero = _pp_cntbits(maskcountnotzero);
          
          while (numgtzero > 0) {
              _pp_vmult_float(result, result, x, maskcountnotzero); //result *= x;
              _pp_vsub_int(count, count, one, maskcountnotzero); // count--;
              
              _pp_vgt_int(maskcountnotzero ,count ,zero ,maskexpisnotzero);
              numgtzero = _pp_cntbits(maskcountnotzero);
          }
          
          _pp_vgt_float(maskresultoutrange,result, limit, maskexpisnotzero);// if (result > 9.999999f)
          
          
          _pp_vload_float(result, limitfloat, maskresultoutrange);// result = 9.999999f;
          
          
          _pp_vstore_float(output + i, result, maskAll);// output[i] = result;
          
          //free(limitfloat);
          //free(onefloat);
      }
      

  }
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N)
{

  //
  // PP STUDENTS TODO: Implement your vectorized version of arraySumSerial here
  //

    //float finalsum[VECTOR_WIDTH] = {0.f};
    float *finalsum;
    finalsum = (float *)malloc(VECTOR_WIDTH*sizeof(float));

    __pp_vec_float x = _pp_vset_float(0.f);
    __pp_vec_float sum = _pp_vset_float(0.f);// float sum = 0;
    __pp_mask arrmaskAll;
    
    //float aaa = 0;

  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
      
      arrmaskAll = _pp_init_ones();

      _pp_vload_float(x ,values + i, arrmaskAll); // x = values[i];
      
      _pp_vadd_float(sum ,sum ,x , arrmaskAll); // sum += values[i];
      
      
  }
    
    int times = log(VECTOR_WIDTH)/log(2);
    
    for (int i = 0; i < times ; i++ ) {
        _pp_hadd_float(sum ,sum);
        
        _pp_interleave_float(sum ,sum);
    }
    /*_pp_hadd_float(sum ,sum);
    
    _pp_interleave_float(sum ,sum);
    
    _pp_hadd_float(sum ,sum);*/
        
    _pp_vstore_float(finalsum, sum, arrmaskAll);
    

    return finalsum[0];
}
