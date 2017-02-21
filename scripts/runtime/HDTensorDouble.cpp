//
//  HDTensorDouble.cpp
//  Hodor
//
//  Created by Marek Lipert on 07.11.2013.
//
//

#include "HDTensorDouble.h"
#include <math.h>

//#define NAIVE_DOUBLE



#ifndef NAIVE_DOUBLE
#include <Accelerate/Accelerate.h>
#endif

/* Naive implementation */


#ifdef NAIVE_DOUBLE

void vector_add(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = argument1[i*argument1Stride] + argument2[i*argument2Stride];
    }
}
void vector_sub(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = argument1[i*argument1Stride] - argument2[i*argument2Stride];
    }
}

void vector_mul(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = argument1[i*argument1Stride] * argument2[i*argument2Stride];
    }
}

void vector_div(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = argument1[i*argument1Stride] / argument2[i*argument2Stride];
    }
}


void vector_exp(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = exp(argument1[i*argument1Stride]);
    }
}
void vector_log(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = log(argument1[i*argument1Stride]);
    }
}
void vector_log1p(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = log1p(argument1[i*argument1Stride]);
    }

}
void vector_sin(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = sin(argument1[i*argument1Stride]);
    }

}
void vector_cos(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = cos(argument1[i*argument1Stride]);
    }

}
void vector_tan(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = tan(argument1[i*argument1Stride]);
    }

}
void vector_cot(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = 1.0/tan(argument1[i*argument1Stride]);
    }
    
}
void vector_atan(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = atan(argument1[i*argument1Stride]);
    }
}

void vector_copy(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
    if(targetStride == 1 && argument1Stride == 1)
    {
        memcpy(target,argument1,sizeof(double)*size);
        return;
    }
    for(int i=0;i<size;i++)
    {
        target[i*targetStride]= argument1[i*argument1Stride];
    }
}

double vector_dotProduct(double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
    double retVal = 0.0;
    for(int i=0;i<size;i++)
    {
        retVal+= argument1[i*argument1Stride]*argument2[i*argument2Stride];
    }
    return retVal;
}
double vector_sum(double* argument1, int argument1Stride ,int size)
{
    double retVal = 0.0;
    for(int i=0;i<size;i++)
    {
        retVal+= argument1[i*argument1Stride];
    }
    return retVal;
}
double vector_mean(double* argument1, int argument1Stride ,int size)
{
    double retVal = 0.0;
    for(int i=0;i<size;i++)
    {
        retVal+= argument1[i*argument1Stride];
    }
    return retVal/size;
}

void vector_addScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
    for(int i=0;i<size;i++)
    {
         target[i*targetStride] = argument1[i*argument1Stride] + scalar;
    }
}

void vector_subScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = argument1[i*argument1Stride] - scalar;
    }
}

void vector_mulScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = argument1[i*argument1Stride]*scalar;
    }
}

void vector_powScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = pow(argument1[i*argument1Stride],scalar);
    }
}


void vector_divScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
    for(int i=0;i<size;i++)
    {
        target[i*targetStride] = argument1[i*argument1Stride]/scalar;
    }
}

#else



void vector_add(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
  vDSP_vaddD(argument1,argument1Stride,argument2,argument2Stride,target,targetStride,size);
}
void vector_sub(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
  vDSP_vsubD(argument2,argument2Stride,argument1,argument1Stride,target,targetStride,size);
}


void vector_mul(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
    vDSP_vmulD(argument1,argument1Stride,argument2,argument2Stride,target,targetStride,size);
}
void vector_div(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
    vDSP_vdivD(argument2,argument2Stride,argument1,argument1Stride,target,targetStride,size);
}



#pragma mark - Transcendental functions

inline void vector_computeTranscendentalFunction(double *target,int targetStride,double* argument1,int argument1Stride,int size,void (*func)(double*,const double*,const int*))
{
  double *buffer;
  double *retBuf;
  
  if(argument1Stride == 1) buffer = argument1;
  else
  {
    buffer = (double *)malloc(sizeof(double)*size);
    vector_copy(buffer,1,argument1,argument1Stride,size);
  }
  
  if(targetStride == 1) retBuf = target;
  else
  {
    retBuf = (double *)malloc(sizeof(double)*size);
  }
  
  func(retBuf,buffer,&size);
  
  if(targetStride > 1)
  {
    vector_copy(target,targetStride,retBuf,1,size);
    free(retBuf);
  }
  if(argument1Stride > 1) free(buffer);
}

inline void vector_computeTwoArgumentTranscendentalFunction(double *target,int targetStride,double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size,void (*func)(double*,const double*,const double*,const int*))
{
  double *buffer,*buffer2;
  double *retBuf;
  
  if(argument1Stride == 1) buffer = argument1;
  else
  {
    buffer = (double *)malloc(sizeof(double)*size);
    vector_copy(buffer,1,argument1,argument1Stride,size);
  }
  
  if(argument2Stride == 1) buffer2 = argument2;
  else
  {
    buffer2 = (double *)malloc(sizeof(double)*size);
    vector_copy(buffer2,1,argument2,argument2Stride,size);
  }
  
  if(targetStride == 1) retBuf = target;
  else
  {
    retBuf = (double *)malloc(sizeof(double)*size);
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



void vector_exp(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvexp);
}

void vector_log(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvlog);
}
void vector_log1p(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvlog1p);
}
void vector_sin(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvsin);
}
void vector_cos(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvcos);
}
void vector_tan(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvtan);
}
void vector_atan(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  vector_computeTranscendentalFunction(target,targetStride,argument1,argument1Stride,size,vvatan);
}


void vector_powScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
  double *buffer = (double *)malloc(sizeof(double)*size);
  for(int i=0;i<size;i++) buffer[i] = scalar;
  vector_computeTwoArgumentTranscendentalFunction(target,targetStride,argument1,argument1Stride,buffer,1,size,vvpow);
  free(buffer);
}



void vector_copy(double *target,int targetStride,double* argument1,int argument1Stride,int size)
{
  if(targetStride == 1 && argument1Stride == 1)
  {
    memcpy(target,argument1,sizeof(double)*size);
    return;
  }
  for(int i=0;i<size;i++)
  { /* how to quickly copy with stride? */
    target[i*targetStride]= argument1[i*argument1Stride];
  }
}

double vector_dotProduct(double* argument1,int argument1Stride,double* argument2,int argument2Stride,int size)
{
  double retVal = 0.0;
  vDSP_dotprD(argument1,argument1Stride,argument2,argument2Stride,&retVal,size);
  return retVal;
}
double vector_sum(double* argument1, int argument1Stride ,int size)
{
  double retVal = 0.0;
  vDSP_sveD(argument1,argument1Stride,&retVal,size);
  return retVal;
}
double vector_mean(double* argument1, int argument1Stride ,int size)
{
  double retVal = 0.0;
  vDSP_sveD(argument1,argument1Stride,&retVal,size);
  return retVal/size;
}

void vector_addScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
  vDSP_vsaddD(argument1,argument1Stride,&scalar,target,targetStride,size);
}

void vector_subScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
  scalar = scalar * (-1);
  vDSP_vsaddD(argument1,argument1Stride,&scalar,target,targetStride,size);
}

void vector_mulScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
  vDSP_vsmulD(argument1,argument1Stride,&scalar,target,targetStride,size);
}

void vector_divScalar(double *target,int targetStride,double* argument1,int argument1Stride,double scalar,int size)
{
  vDSP_vsdivD(argument1,argument1Stride,&scalar,target,targetStride,size);
}
#endif




