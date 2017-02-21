//
//  HDStdlib.cpp
//  Hodor
//
//  Created by Test on 06.01.2014.
//
//
#include "HDTensor.h"
#include "HDUniversalEnvelope.h"
#include <execinfo.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "HDList.h"
#include <map>
#include <sys/time.h>
#include <math.h>


template <class T>
inline HDTensor<T> tensorProduct(const HDTensor<T> &arg1,const HDTensor<T> &arg2)
{
    return HDTensor<T>::tensorProduct(arg1,arg2);
}

template <class T>
inline HDTensor<T> matrixMultiply(const HDTensor<T> &arg1,const HDTensor<T> &arg2)
{
    return HDTensor<T>::tensorProduct(arg1,arg2).contract(arg1.HDFunction__size().count()-1,arg1.HDFunction__size().count());
}


template <class T,class C>
HDTensor<T> HDFunction__exp(const HDTensor<T> &arg)
{
    return HDTensor<T>::exp(arg);
}

template <class T, class C>
HDTensor<T> HDFunction__pow(const HDTensor<T> &arg,T exponent)
{
    printf("T ** %f\n",exponent);
    return HDTensor<T>::pow(arg,exponent);
}

template <class T,class C>
HDTensor<T> HDFunction__pow(const HDTensor<T> &arg,int exponent)
{
     printf("T ** %d\n",exponent);
    return HDTensor<T>::pow(arg,exponent);
}

template <class T,class C>
HDTensor<T> HDFunction__sin(const HDTensor<T> &arg)
{
    return HDTensor<T>::sin(arg);
}

template <class T,class C>
HDTensor<T> HDFunction__cos(const HDTensor<T> &arg)
{
    return HDTensor<T>::cos(arg);
}

template <class T,class C>
HDTensor<T> HDFunction__log(const HDTensor<T> &arg)
{
    return HDTensor<T>::log(arg);
}

template <class T,class C>
HDTensor<T> HDFunction__log1p(const HDTensor<T> &arg)
{
    return HDTensor<T>::log1p(arg);
}

template <class T,class C>
HDTensor<T> HDFunction__tan(const HDTensor<T> &arg)
{
    return HDTensor<T>::tan(arg);
}


template <class T,class C>
T HDFunction__exp(T arg)
{
    return exp(arg);
}

template <class T,class C>
T HDFunction__pow(T arg,T exponent)
{
    return pow(arg,exponent);
}


template <class T,class C>
T HDFunction__sin(T arg)
{
    return sin(arg);
}

template <class T,class C>
T HDFunction__cos(T arg)
{
    return cos(arg);
}

template <class T,class C>
T HDFunction__log(T arg)
{
    return log(arg);
}

template <class T,class C>
T HDFunction__log1p(T arg)
{
    return log1p(arg);
}

template <class T,class C>
T HDFunction__tan(T arg)
{
    return tan(arg);
}


static void print_trace (void)
{
   void *array[10];
   size_t size;
   char **strings;
   size_t i;
   size = backtrace (array, 10);
   strings = backtrace_symbols (array, size);
   printf ("Obtained %zd stack frames.\n", size);
   for (i = 0; i < size; i++)
                   printf ("%s\n", strings[i]);
   free (strings);
}

template <class T, class C>
void HDFunction__assert(int condition, const HDStringEnvelope<T,C> &text)
{
  if(condition) return;
  printf("Assertion failed: %s\n",((std::string)text).c_str());
  print_trace();
  exit(1); 
}

template <class T, class C>
HDList<T,C> HDFunction__range(int lo, int hi)
{
  HDList<T,C> aList;
  for(int i=lo;i<=hi;i++) aList.append(i);
  return aList;
}

template <class T, class C>
HDList<T,C> HDFunction__reverseRange(int hi, int lo)
{
  HDList<T,C> aList;
  for(int i=hi;i>=lo;i--)  aList.append(i);
  return aList;
}


template <class T, class C>
void HDFunction__printResults()
{
  HDTensor<T>::printResults();
}


template <class T, class C>
HDStringEnvelope<T,C> HDFunction__simplePrintf(const HDStringEnvelope<T,C> &buf, const HDMemoryModel &object)
{
  static int bufSizeooo = 0;
  static C *bufooo = NULL;
  if(bufSizeooo<buf.length()+1000) 
  {
    if(bufooo) free(bufooo);
    bufSizeooo = buf.length()+1000;
    bufooo = (char *)malloc(bufSizeooo*sizeof(C));
  }
  
  
  
  HDStringEnvelope<T,C> lastCharS = buf[buf.length()-1];
  C lastChar = lastCharS.c_str().c_str()[0];
  switch(lastChar)
  {
     case 'd':
     {
       const HDUniversalEnvelope<T,C> &o = dynamic_cast<const HDUniversalEnvelope<T,C> &>(object);
       snprintf(bufooo,bufSizeooo-1,buf.c_str().c_str(),HDUniversalEnvelope<T,C>::explicitIntCast(o));
     }
       break;
     case 'f':
     case 'e':
     case 'E':
     {
       const HDUniversalEnvelope<T,C> &o = dynamic_cast<const HDUniversalEnvelope<T,C> &>(object);
       snprintf(bufooo,bufSizeooo-1,buf.c_str().c_str(),HDUniversalEnvelope<T,C>::explicitDecimalCast(o));     
     }
     break;
     case 's':
     {
       const HDStringEnvelope<T,C> &o = dynamic_cast<const HDStringEnvelope<T,C> &>(object);
       snprintf(bufooo,bufSizeooo-1,buf.c_str().c_str(),o.c_str().c_str());          
     }
       break;
     break;
     default:
     HDFunction__assert<T,C>(0,HDStringEnvelope<T,C>("Wrong format type: ")+lastCharS);
  }
  return HDStringEnvelope<T,C>(bufooo);
}


static map<int,FILE *> fileMap;

template <class T, class C>
int HDFunction__open(const HDStringEnvelope<T,C> &path, const HDStringEnvelope<T,C> &params)
{
   FILE *f = fopen(path.c_str().c_str(),params.c_str().c_str());
   HDFunction__assert(f != NULL,HDStringEnvelope<T,C>("Unable to open file: '")+path+HDStringEnvelope<T,C>("'"));
   fileMap[fileno(f)] = f;
   return fileno(f);
}

static map<int,struct timeval> timeMap;
static int newestTimeVal = 0;

template <class T, class C>
int  HDFunction__getTimeReference()
{
 struct timeval tv;
 gettimeofday(&tv,NULL);
 timeMap[newestTimeVal] = tv;
 return newestTimeVal++;
}

template <class T, class C>
T HDFunction__getTimeDifference(int a, int b)
{
   map<int,struct timeval>::iterator t1 = timeMap.find(a);
   map<int,struct timeval>::iterator t2 = timeMap.find(b);
   HDFunction__assert(t1!=timeMap.end(),HDStringEnvelope<T,C>("Time reference was not set for index ")+a);
   HDFunction__assert(t2!=timeMap.end(),HDStringEnvelope<T,C>("Time reference was not set for index ")+b);
   int usec = ((*t2).second.tv_usec-(*t1).second.tv_usec);
   int sec = ((*t2).second.tv_sec-(*t1).second.tv_sec) - ((usec < 0) ? 1 : 0);
//   printf("Usec %d, sec %d\n",usec,sec);

   usec = (usec >= 0) ? usec : (1000000+usec); 
   return ((T)sec) + ((T)usec)/1000000.0;
}


template <class T, class C>
int HDFunction__write(int fileNo, const HDStringEnvelope<T,C> &text)
{
   return write(fileNo,text.c_str().c_str(),text.length()*sizeof(C));
}

template <class T,class C>
void HDFunction__close(int fileNo)
{
  FILE *f = fileMap[fileNo];
  assert(f != NULL);
  fclose(f);
  fileMap.erase(fileNo);
}

template <class T, class C>
HDStringEnvelope<T,C> HDFunction__read(int fileNo, int byteCount)
{
  static C *buffer = NULL;
  static int bufferLen = 0;
  if(bufferLen<byteCount)
  {
    if(buffer) free(buffer);
    buffer = malloc(sizeof(C)*byteCount);
    bufferLen = byteCount;
  }
  byteCount = read(fileNo,buffer,byteCount);
  if(byteCount) buffer[byteCount] = 0;
  return HDStringEnvelope<T,C>(buffer);
}

template <class T,class C>
HDStringEnvelope<T,C> HDFunction__printf(const HDStringEnvelope<T,C> &arg, const HDList<T,C> &vars)
{
  HDStringEnvelope<T,C> retVal;
  int state = 0;
  HDStringEnvelope<T,C> oneBuffer;
  int currentArg = 1;
  for(typename HDStringEnvelope<T,C>::iterator c = arg.begin();c!=arg.end();c++)
{
  if((state == 0))
{
  if(((*c) == HDStringEnvelope<T,C>("%")))
{
  oneBuffer = (*c).HDFunction__copy();
  state = 1;
}
else
{
  retVal = (retVal + (*c));
}
;
}
else
{
  if(((((*c) == HDStringEnvelope<T,C>("d")) || ((*c) == HDStringEnvelope<T,C>("f"))) || ((*c) == HDStringEnvelope<T,C>("s")) || ((*c) == HDStringEnvelope<T,C>("e")) || ((*c) == HDStringEnvelope<T,C>("E"))))
{
  oneBuffer = (oneBuffer + (*c));
  retVal += HDFunction__simplePrintf(oneBuffer,vars[currentArg-1]);
  currentArg = (currentArg + 1);
  state = 0;
}
else
{
  oneBuffer = (oneBuffer + (*c));
}
;
}
;
}
;
  HDFunction__assert((state == 0),(HDStringEnvelope<T,C>("Wrong format string: ") + arg));
    return retVal;
;
}


