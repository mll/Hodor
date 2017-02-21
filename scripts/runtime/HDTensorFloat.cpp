//
//  HDTensorFloat.cpp
//  Hodor
//
//  Created by Test on 11.11.2013.
//
//

#include "HDTensorFloat.h"
#include <math.h>
#include <Accelerate/Accelerate.h>

void vector_add(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size)
{
    vDSP_vadd(argument1,argument1Stride,argument2,argument2Stride,target,targetStride,size);
}
void vector_sub(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size)
{
   vDSP_vsub(argument2,argument2Stride,argument1,argument1Stride,target,targetStride,size);
}

void vector_mul(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size)
{
    vDSP_vmul(argument1,argument1Stride,argument2,argument2Stride,target,targetStride,size);
}
void vector_div(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size)
{
    vDSP_vdiv(argument2,argument2Stride,argument1,argument1Stride,target,targetStride,size);
}




#pragma mark - Transcendental functions

inline void vector_computeTranscendentalFunction(float *target,int targetStride,float* argument1,int argument1Stride,int size,void (*func)(float*,const float*,const int*))
{
    float *buffer;
    float *retBuf;
    
    if(argument1Stride == 1) buffer = argument1;
    else
    {
        buffer = (float *)malloc(sizeof(float)*size);
        vector_copy(buffer,1,argument1,argument1Stride,size);
    }
    
    if(targetStride == 1) retBuf = target;
    else
    {
        retBuf = (float *)malloc(sizeof(float)*size);
    }
    
    func(retBuf,buffer,&size);
    
    if(targetStride > 1)
    {
        vector_copy(target,targetStride,retBuf,1,size);
        free(retBuf);
    }
    if(argument1Stride > 1) free(buffer);
}

inline void vector_computeTwoArgumentTranscendentalFunction(float *target,int targetStride,float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size,void (*func)(float*,const float*,const float*,const int*))
{
    float *buffer,*buffer2;
    float *retBuf;
    
    if(argument1Stride == 1) buffer = argument1;
    else
    {
        buffer = (float *)malloc(sizeof(float)*size);
        vector_copy(buffer,1,argument1,argument1Stride,size);
    }
    
    if(argument2Stride == 1) buffer2 = argument2;
    else
    {
        buffer2 = (float *)malloc(sizeof(float)*size);
        vector_copy(buffer2,1,argument2,argument2Stride,size);
    }
    
    if(targetStride == 1) retBuf = target;
    else
    {
        retBuf = (float *)malloc(sizeof(float)*size);
    }
    
    func(retBuf,buffer2,buffer,&size);
    
    if(targetStride > 1)
    {
        vector_copy(target,targetStride,retBuf,1,size);
        free(retBuf);
    }
    if(argument1Stride > 1) free(buffer);
    if(argument2Stride > 1) free(buffer2);
}



void vector_exp(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvexpf);
}

void vector_log(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvlogf);
}
void vector_log1p(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvlog1pf);
}
void vector_sin(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvsinf);
}
void vector_cos(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvcosf);
}
void vector_tan(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvtanf);
}
void vector_atan(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvatanf);
}


void vector_powScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size)
{
    float *buffer = (float *)malloc(sizeof(float)*size);
    for(int i=0;i<size;i++) buffer[i] = scalar;
    vector_computeTwoArgumentTranscendentalFunction(target,targetStride,argument1,argument1Stride,buffer,1,size,vvpowf);
    free(buffer);
}



void vector_copy(float *target,int targetStride,float* argument1,int argument1Stride,int size)
{
    if(targetStride == 1 && argument1Stride == 1)
    {
        memcpy(target,argument1,sizeof(float)*size);
        return;
    }
    for(int i=0;i<size;i++)
    { /* how to quickly copy with stride? */
        target[i*targetStride]= argument1[i*argument1Stride];
    }
}

float vector_dotProduct(float* argument1,int argument1Stride,float* argument2,int argument2Stride,int size)
{
    float retVal = 0.0;
    vDSP_dotpr(argument1,argument1Stride,argument2,argument2Stride,&retVal,size);
    return retVal;
}
float vector_sum(float* argument1, int argument1Stride ,int size)
{
    float retVal = 0.0;
    vDSP_sve(argument1,argument1Stride,&retVal,size);
    return retVal;
}
float vector_mean(float* argument1, int argument1Stride ,int size)
{
    float retVal = 0.0;
    vDSP_sve(argument1,argument1Stride,&retVal,size);
    return retVal/size;
}

void vector_addScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size)
{
    vDSP_vsadd(argument1,argument1Stride,&scalar,target,targetStride,size);
}

void vector_subScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size)
{
    scalar = scalar * (-1);
    vDSP_vsadd(argument1,argument1Stride,&scalar,target,targetStride,size);
}

void vector_mulScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size)
{
    vDSP_vsmul(argument1,argument1Stride,&scalar,target,targetStride,size);
}

void vector_divScalar(float *target,int targetStride,float* argument1,int argument1Stride,float scalar,int size)
{
    vDSP_vsdiv(argument1,argument1Stride,&scalar,target,targetStride,size);
}




