//
//  HDTensorFloat.h
//  Hodor
//
//  Created by Test on 11.11.2013.
//
//

#ifndef __Hodor__HDTensorFloat__
#define __Hodor__HDTensorFloat__

#include <iostream>

void vector_add(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size);
void vector_sub(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size);
void vector_mul(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size);
void vector_div(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size);

void vector_exp(float *target,int targetStride,float* argument1,int argument1Stride,int size);
void vector_log(float *target,int targetStride,float* argument1,int argument1Stride,int size);
void vector_log1p(float *target,int targetStride,float* argument1,int argument1Stride,int size);
void vector_sin(float *target,int targetStride,float* argument1,int argument1Stride,int size);
void vector_cos(float *target,int targetStride,float* argument1,int argument1Stride,int size);
void vector_tan(float *target,int targetStride,float* argument1,int argument1Stride,int size);
void vector_atan(float *target,int targetStride,float* argument1,int argument1Stride,int size);

void vector_copy(float *target,int targetStride,float* argument1,int argument1Stride,int size);

float vector_dotProduct(float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size);
float vector_sum(float* argument1,int argument1Stride,int size);
float vector_mean(float* argument1,int argument1Stride,int size);

void vector_addScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size);
void vector_subScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size);
void vector_mulScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size);
void vector_divScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size);

void vector_powScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size);


#endif /* defined(__Hodor__HDTensorFloat__) */
