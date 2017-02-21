//
//  HDTensorDouble.h
//  Hodor
//
//  Created by Marek Lipert on 07.11.2013.
//
//

#ifndef __Hodor__HDTensorDouble__
#define __Hodor__HDTensorDouble__

#include <iostream>

    void vector_add(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size);
    void vector_sub(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size);

    void vector_mul(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size);
    void vector_div(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size);


    void vector_exp(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    void vector_log(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    void vector_log1p(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    void vector_sin(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    void vector_cos(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    void vector_tan(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    void vector_cot(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    void vector_atan(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    
    void vector_copy(double *target,int targetStride,double* argument1,int argument1Stride,int size);
    
    double vector_dotProduct(double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size);
    double vector_sum(double* argument1,int argument1Stride,int size);
    double vector_mean(double* argument1,int argument1Stride,int size);
    
    void vector_addScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size);
    void vector_subScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size);
    void vector_mulScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size);
    void vector_powScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size);
    void vector_divScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size);




#endif /* defined(__Hodor__HDTensorDouble__) */
