//
//  HDTensor.cpp
//  Hodor
//
//  Created by Marek Lipert on 03.11.2013.
//
//

#include "HDTensor.h"
#include <strings.h>
#include <stdarg.h>
#include <assert.h>
#include "HDDataReference.h"
#include <math.h>
#include "HDPrimitiveEnvelope.h"

using namespace std;


static int constructorCount = 0;
static int getElementCount = 0;

#pragma mark - Lifecycle


/* tensor derived from other tensor doesn't copy, just reference */
template <class T>
HDTensor<T>::HDTensor() : HDMemoryModel(HDTypeTensor)
{
    data = NULL;
    constructorCount++;
    nonstandardStrides = false;
    totalSize = 0;
    shift = 0;
    dimensionSizes.size();
    dimensionStrides.size();
}

template <class T>
HDMemoryModel *HDTensor<T>::createWeakCopy() const
{
    return new HDTensor<T>(*this);
}

template <class T>
HDMemoryModel *HDTensor<T>::createStrongCopy() const
{
    return new HDTensor<T>(this->copy());
}

template <class T>
HDTensor<T>::HDTensor(const char *special,const HDTensor<T> &t) : HDMemoryModel(HDTypeTensor)
{
    data = NULL;
    constructorCount++;
    nonstandardStrides = false;
    totalSize = 0;
    shift = 0;
    dimensionSizes.size();
    dimensionStrides.size();   
    this->copyStructure(t);
}

template <class T>
void HDTensor<T>::copyStructure(const HDTensor<T> &t)
{
#ifdef VERBOSE_MEMORY
    printf("---> %p Tensor of dimensions/strides ",this);
    for(int i=0;i<t.dimensionStrides.size();i++) printf("%d/%d ",t.dimensionSizes[i], t.dimensionStrides[i]);
    printf(" has copied its structure\nNew structure: ");
#endif
    if(data) referenceForPointer(data)->release();
    
    if(t.data == NULL)
    {
        data = NULL;
        dimensionSizes = vector<int>();
        dimensionStrides = vector<int>();
        return;
    }
    
    
    int tmp = 0;
    dimensionSizes = vector<int>();
    dimensionStrides = vector<int>();
    int dimension = t.dimensionSizes.size();
    if(dimension == 0)
    {
        data = NULL;
        dimensionSizes = vector<int>();
        dimensionStrides = vector<int>();
        return;
    }
    
    totalSize = 1;
    for(int i=0;i<dimension;i++)
    {
        dimensionSizes.push_back(t.dimensionSizes[i]);
        dimensionStrides.push_back(1);
        totalSize*=dimensionSizes[i];
    }

    
    dimensionStrides[dimension-1] = 1;
    
    tmp = dimensionSizes[dimension-1];
    for(int i=dimension-2;i>=0;i--)
    {
        dimensionStrides[i] = tmp;
        tmp*=dimensionSizes[i];
    }
    shift = 0;
    data = (T*)calloc(sizeof(T),totalSize);
    nonstandardStrides = false;
#ifdef VERBOSE_MEMORY
    for(int i=0;i<dimension;i++) printf("%d/%d ",dimensionSizes[i], dimensionStrides[i]);
    printf("\n");
#endif
    //printf("Size: %lu",dimensionSizes.size());
    new HDDataReference((void *)data);
}




template <class T>
HDTensor<T>::HDTensor(const HDTensor<T> &t) : HDMemoryModel(HDTypeTensor)
{
    constructorCount++;
    data = NULL;
    nonstandardStrides = false;
    totalSize = 0;
    shift = 0;
    dimensionSizes.size();
    dimensionStrides.size();

#ifdef VERBOSE_MEMORY
    printf("Copy stealing constructor invoked for tensor with sizes/strides ");
    for(int i=0;i<t.dimensionSizes.size();i++)
    {
        printf("%d/%d ",t.dimensionSizes[i],t.dimensionStrides[i]);
    }
    printf("\n");
#endif
    *this = t;
}





template <class T>
HDTensor<T>::HDTensor(vector<int> &dimensions) : HDMemoryModel(HDTypeTensor)
{
    constructorCount++;
    setup(dimensions);
}

template <class T>
void HDTensor<T>::setup(vector<int> &dimensions)
{
    register int total_size = 1;
    register int tmp;
    int dimension = dimensions.size();
    
    dimensionSizes.reserve(dimension);
    dimensionStrides.reserve(dimension);
    
    for(int i=0;i<dimensions.size();i++)
    {
        tmp = dimensions[i];
        total_size*=tmp;
        this->dimensionSizes.push_back(tmp);
    }
    data = (T *) calloc(sizeof(T),total_size);
    new HDDataReference((void *)data);
    totalSize = total_size;
    
    for(int i=0;i<dimension;i++) dimensionStrides.push_back(0);
    dimensionStrides[dimension-1] = 1;
    
    tmp = dimensionSizes[dimension-1];
    for(int i=dimension-2;i>=0;i--)
    {
        dimensionStrides[i] = tmp;
        tmp*=dimensionSizes[i];
    }
    shift = 0;
    nonstandardStrides = false;
#ifdef VERBOSE_MEMORY
    printf("---> %p Tensor of dimensions/strides ",this);
    for(int i=0;i<dimensionStrides.size();i++) printf("%d/%d ",dimensionSizes[i], dimensionStrides[i]);
    printf(" allocated\n");
#endif

}



template <class T>
HDTensor<T>::HDTensor(int dimension,...) : HDMemoryModel(HDTypeTensor)
{
    constructorCount++;
    va_list ap;
#ifdef VERBOSE_ARRAY_ACCESS
    if(dimension <= 0)
    {
        printf("! Dimension is %d in vararg tensor constructor\n",dimension);
        printTrace(); exit(1);
    }
#endif
    assert(dimension > 0);
    register int tmp;
    vector<int> access;
    access.reserve(dimension);
    
    va_start(ap,dimension);
    for(int i=0;i<dimension;i++)
    {
        tmp = va_arg(ap,int);
#ifdef VERBOSE_ARRAY_ACCESS
    if(tmp <= 0)
    {
        printf("! Dimension is %d in vararg tensor constructor\n",tmp);
        printTrace(); exit(1);
    }
#endif        
        access.push_back(tmp);
    }
    va_end(ap);
    setup(access);
}

template <class T>
HDTensor<T>::~HDTensor()
{
#ifdef VERBOSE_MEMORY
    printf("---> %p Tensor of dimensions/strides ",this);
    for(int i=0;i<dimensionStrides.size();i++) printf("%d/%d ",dimensionSizes[i], dimensionStrides[i]);
    printf(" deallocated\n");
#endif
    if(data) referenceForPointer(data)->release();
}

template <class T>
T* HDTensor<T>::getElement(vector<int> &access) const
{
//    getElementCount++;
    int tmp = shift;
    int dimension = dimensionSizes.size();
    for(int i=0;i<dimension;i++)
    {
        tmp += access[i]*dimensionStrides[i];
    }
    return data+tmp;
}


template <class T>
T* HDTensor<T>::getElement(iterator &access) const
{

    return getElement(access.access);
}

template <class T>
T* HDTensor<T>::getElement(int count,int access1) const
{
#ifdef VERBOSE_ARRAY_ACCESS
    if(count != dimension) {
        printf("Range error! Not enough dimensions for array access provided. Needs %d, got %d\n", dimension,count);
        
        printTrace(); exit(1);
    }
#endif

    return data+access1*dimensionStrides[0];
}

template <class T>
T* HDTensor<T>::getElement(int count,int access1,int access2) const
{
#ifdef VERBOSE_ARRAY_ACCESS
    if(count != dimension) {
        printf("Range error! Not enough dimensions for array access provided. Needs %d, got %d\n", dimension,count);
        
        printTrace(); exit(1);
    }
#endif

    return data+access1*dimensionStrides[0]+access2*dimensionStrides[1];
}


template <class T>
T* HDTensor<T>::getElement(int count,int access1,int access2, int access3) const
{
#ifdef VERBOSE_ARRAY_ACCESS
    if(count != dimension) {
        printf("Range error! Not enough dimensions for array access provided. Needs %d, got %d\n", dimension,count);
        
        printTrace(); exit(1);
    }
#endif

    return data+access1*dimensionStrides[0]+access2*dimensionStrides[1] + access3 * dimensionStrides[2];
}
template <class T>
T* HDTensor<T>::getElement(int count,int access1,int access2,int access3, int access4) const
{
#ifdef VERBOSE_ARRAY_ACCESS
    if(count != dimension) {
        printf("Range error! Not enough dimensions for array access provided. Needs %d, got %d\n", dimension,count);
        
        printTrace(); exit(1);
    }
#endif

    return data+access1*dimensionStrides[0]+access2*dimensionStrides[1] + access3 * dimensionStrides[2] + access4 * dimensionStrides[3];
}
template <class T>
T* HDTensor<T>::getElement(int count,int access1,int access2,int access3, int access4,int access5) const
{
#ifdef VERBOSE_ARRAY_ACCESS
    if(count != dimension) {
        printf("Range error! Not enough dimensions for array access provided. Needs %d, got %d\n", dimension,count);
        
        printTrace(); exit(1);
    }
#endif
    return data+access1*dimensionStrides[0]+access2*dimensionStrides[1] + access3 * dimensionStrides[2] + access4 * dimensionStrides[3] + access5 * dimensionStrides[4];
}
template <class T>
T* HDTensor<T>::getElement(int count,int access1,int access2,int access3, int access4,int access5,int access6) const
{
#ifdef VERBOSE_ARRAY_ACCESS
    if(count != dimension) {
        printf("Range error! Not enough dimensions for array access provided. Needs %d, got %d\n", dimension,count);
        
        printTrace(); exit(1);
    }
#endif

  return data+access1*dimensionStrides[0]+access2*dimensionStrides[1] + access3 * dimensionStrides[2] + access4 * dimensionStrides[3] + access5 * dimensionStrides[4] + access6 * dimensionStrides[5];
}


/*
template <class T>
T* HDTensor<T>::getElement(int count,...) const
{
    register int dimension = dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(count != dimension) {
        printf("Range error! Not enough dimensions for array access provided. Needs %d, got %d\n", dimension,count);
        
        printTrace(); exit(1);
    }
#endif
    vector<int> access;
    va_list ap;
    va_start(ap,count);
    register int tmp;
    access.reserve(dimension);
    for(int i=0;i<dimension;i++) access.push_back(0);
    
    for(int i=0;i<dimension;i++)
    {
        tmp = va_arg(ap,int);
        access[i] = tmp;
#ifdef VERBOSE_ARRAY_ACCESS
        if(access[i] < 0 || access[i] >= dimensionSizes[i]) {
            printf("Range error! Leftmost dimension for array: %d > maximal %d or < 0", access[i],dimensionSizes[i]);
            
            printTrace(); exit(1);
        }
#endif
    }
    va_end(ap);
    return this->getElement(access);
}
*/

template <class T>
HDTensor<T> HDTensor<T>::newSubTensor(int dim,int firstLo,int firstHi,...) const
{
    va_list ap;
    int tmp = 0;
    va_start(ap,firstHi);
    register int dimension = dimensionSizes.size();
    #ifdef VERBOSE_ARRAY_ACCESS
    if(dim>dimension)
    {
        printf("Trying to access a subarray of dimension(%d) greater than the original arrays(%d)!", dim,dimension);
        
        printTrace(); exit(1);
    }
    #endif
    vector<int> los,his;
    los.push_back(firstLo);
    his.push_back(firstHi);
    
    
    for(int i=1;i<dimension;i++)
    {
        if(i<dim)
        {
            tmp = va_arg(ap,int);
            los.push_back(tmp);
            tmp = va_arg(ap,int);
            his.push_back(tmp);
        }
        else
        {
            los.push_back(-2);
            his.push_back(-2);
        }
    }
    
    va_end(ap);

    return newSubTensor(los,his);
}


template <class T>
HDTensor<T> HDTensor<T>::newReversedSubTensor(int dim,int firstLo,int firstHi,...) const
{
    va_list ap;
    int tmp = 0;
    va_start(ap,firstHi);
    register int dimension = dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(dim>dimension)
    {
        printf("Trying to access a subarray of dimension(%d) greater than the original arrays(%d)!", dim,dimension);
        printTrace(); exit(1);
    }
#endif
    vector<int> los,his,nlos,nhis;
    los.push_back(firstLo);
    his.push_back(firstHi);
    int ii = 0;
    
    for(int i=1;i<dimension;i++)
    {
        if(i<dim)
        {
            tmp = va_arg(ap,int);
            los.push_back(tmp);
            tmp = va_arg(ap,int);
            his.push_back(tmp);
        }
        else
        {
            los.push_back(-2);
            his.push_back(-2);
            ii++;
        }
    }
    
    va_end(ap);
    
    for(int i=0;i<ii;i++)
    {
        nlos.push_back(-2);
        nhis.push_back(-2);
    }
    for(int i=0;i<los.size()-ii;i++)
    {
        nlos.push_back(los[i]);
        nhis.push_back(his[i]);
    }
//    printf("New reveresed subtensor dim %d\n",dim);
//    for(int i=0;i<los.size()-ii;i++)
//    {
//        printf("%d %d\n",nlos[i],
//               nhis[i]);
//    }
//    printf("===============================\n");
    return newSubTensor(nlos,nhis);
}




template <class T>
HDTensor<T> HDTensor<T>::newSubTensor(const vector<int> &los, const vector<int> &his) const
{
  //  printf("Subtensor with los.count() %lu highs count %lu dim %lu\n",los.size(),his.size(),dimensionSizes.size());
    /* check bounds exception */
    int lo[1024];
    int hi[1024];
    bool hasDims[1024];
    register int tmp;
    register int address = 0;
    register int dimension = dimensionSizes.size();
    //for(int i=0;i<los.size();i++) printf("SubTensor: %d lo %d hi %d\n",i,los[i],his[i]);
   // printf("======================\n");
#ifdef VERBOSE_ARRAY_ACCESS
    if(data == NULL) {
        printf("Trying to get an element of already freed array");
        printTrace(); exit(1);
    }
#endif
    register int retDim = 0;
    int firstHi = his[0];
    int firstLo = los[0];
    hi[0] = firstHi > -1.5 ? firstHi : dimensionSizes[0]-1;
    lo[0] = firstLo > -1.5 ? firstLo : 0;
    if(lo[0]!=hi[0] || firstHi<-1.5 || firstLo < -1.5) retDim++;
    if(retDim) hasDims[0] = true;
    else hasDims[0] = false;
  
    HDTensor<T> retVal;
    
#ifdef VERBOSE_ARRAY_ACCESS
    if(hi[0] >= dimensionSizes[0] || lo[0] < 0 || hi[0]<lo[0]) {
        printf("%s %s %d: Range error! Dimension of array selection wrong: lo: %d hi: %d, size %d ", __FILE__,__func__,__LINE__,lo[0],hi[0],dimensionSizes[0]);
        printTrace(); exit(1);
    }
#endif

    
    
    for(int i=1;i<dimension;i++)
    {
        bool hasDim = false;
        tmp = los[i];
        lo[i] = tmp >= -1.5 ? tmp : 0;
        if(tmp<-1.5) hasDim = true;
        tmp = his[i];
        if(tmp<-1.5) hasDim = true;
        hi[i] = tmp >= -1.5 ? tmp : dimensionSizes[i]-1;
        if(lo[i]!=hi[i] || hasDim) retDim++;
        hasDims[i] = hasDim || lo[i]!=hi[i];
#ifdef VERBOSE_ARRAY_ACCESS
        if(hi[i] >= dimensionSizes[i] || lo[i] < 0 || hi[i]<lo[i]) {
            printf("Range error!  Dimension of array selection wrong: lo: %d hi: %d, size %d ", lo[i],hi[i],dimensionSizes[i]);
            printTrace(); exit(1);
        }
#endif
    }

#ifdef VERBOSE_ARRAY_ACCESS
    if(retDim == 0)
    {
        printf("%s - %s():%d Range error! Subtensor cannot produce zero-dimensional entity.\n", __FILE__,__func__,__LINE__);
        printTrace(); exit(1);
    }
#endif
    /* shift first */
    int shift = this->shift;
    for(int i=0;i<dimension;i++)
    {
        //    printf("Shift increased by i %d lo %d stride %d\n",i,lo[i],dimensionStrides[i]);
        shift+=lo[i]*dimensionStrides[i];
    }
    
    int j=0;
    retVal.totalSize = 1;
    for(int i=0;i<dimension;i++)
    {
        if(hasDims[i])
        {
            retVal.dimensionSizes.push_back(hi[i]-lo[i]+1);
            retVal.dimensionStrides.push_back(dimensionStrides[i]);
            retVal.totalSize*=retVal.dimensionSizes[j];
            j++;
        }
    }
  if(retDim==0)
  {
   retVal.dimensionSizes.push_back(1);
   retVal.dimensionStrides.push_back(1);
  }
  
  
    retVal.data = data;
    retVal.shift = shift;
    referenceForPointer(data)->retain();
    retVal.nonstandardStrides = nonstandardStrides;
    
    if(dimensionSizes.size()!=retVal.dimensionSizes.size()) retVal.nonstandardStrides = true;
    else
    {
        for(int i=0;i<dimensionSizes.size();i++)
        {
            if(dimensionSizes[i]!=retVal.dimensionSizes[i]) retVal.nonstandardStrides = true;
            if(dimensionStrides[i]!=retVal.dimensionStrides[i]) retVal.nonstandardStrides = true;
        }
        if(retVal.shift) retVal.nonstandardStrides = true;
    }
#ifdef VERBOSE_MEMORY
    printf("---> %p Tensor of dimensions/strides ",&retVal);
    for(int i=0;i<retVal.dimensionStrides.size();i++) printf("%d/%d ",retVal.dimensionSizes[i], retVal.dimensionStrides[i]);
    printf(" created as a subtensor of %p\n",this);
#endif
//    printf("!!!!!!!!!!!!!!!!!!!!!!!\n");
//    retVal.print();
//    printf("!!!!!!!!!!!!!!!!!!!!!!!\n");
    return retVal;
}



template <class T>
HDTensor<T> HDTensor<T>::newCopy() const
{
    return this->copy();
}



template <class T>
HDTensor<T> HDTensor<T>::copy() const
{
    HDTensor<T> retVal;
    int dimension = dimensionSizes.size();
    int tmp = 1;
    
    for(int i=0;i<dimension;i++)
    {
        retVal.dimensionSizes.push_back(dimensionSizes[i]);
        retVal.dimensionStrides.push_back(0);
    }
    
    retVal.data = (T *)malloc(sizeof(T)*totalSize);
    retVal.totalSize = totalSize;
    retVal.shift = 0;
    
    retVal.dimensionStrides[dimension-1] = 1;
    
    tmp = retVal.dimensionSizes[dimension-1];
    
    for(int i=dimension-2;i>=0;i--)
    {
        retVal.dimensionStrides[i] = tmp;
        tmp*=retVal.dimensionSizes[i];
    }
    

    for(HDTensor<T>::iterator i = begin();i!=end();i++)
        *(retVal.getElement(i)) = *(getElement(i));
    new HDDataReference((void *)retVal.data);
    retVal.nonstandardStrides = false;
#ifdef VERBOSE_MEMORY
    printf("---> %p Tensor of dimensions/strides ",&retVal);
    for(int i=0;i<dimensionStrides.size();i++) printf("%d/%d ",retVal.dimensionSizes[i], retVal.dimensionStrides[i]);
    printf(" created as a copy of %p\n",this);
#endif
    return retVal;
}

template <class T>
void HDTensor<T>::print() const
{
    printf("Printing tensor %p\n",this);
    int dimension = dimensionSizes.size();
    for(HDTensor<T>::iterator it = begin();it!=end();it++)
    {
        for(int i=0;i<dimension;i++) printf("%d ",it.access[i]);
        printf("%f\n",*(getElement(it)));
    }
    
}

#pragma mark - Operators
template <class T>
HDTensor<T> HDTensor<T>::tensorProduct(const HDTensor<T> &t1,const HDTensor<T> &t2)
{
    vector<int> sizes;
    sizes.reserve(t1.dimensionSizes.size()+t2.dimensionSizes.size());
    
    for(int i=0;i<t1.dimensionSizes.size();i++) sizes.push_back(t1.dimensionSizes[i]);
    for(int i=0;i<t2.dimensionSizes.size();i++) sizes.push_back(t2.dimensionSizes[i]);
    
    HDTensor<T> retVal;
    retVal.setup(sizes);
    for(HDTensor<T>::iterator it = t1.begin();it!=t1.end();it++)
    {
        vector<int> lows,his;
        for(int i=0;i<it.access.size();i++)
        {
         lows.push_back(it.access[i]);
         his.push_back(it.access[i]);
        }
        for(int i=0;i<t2.dimensionSizes.size();i++)
        {
            lows.push_back(-2);
            his.push_back(-2);
        }
        retVal.newSubTensor(lows,his) = *t1.getElement(it) * t2;
    }
    return retVal;
}


template <class T>
HDTensor<T> HDTensor<T>::contract(int left, int right) const
{
    vector<int> sizes;
    int dimension = dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(dimension<2)
    {
        printf("Contraction on vector!");
        printTrace(); exit(1);
    }
    if(left>=dimension || right>=dimension || left==right || left<0 || right<0)
    {
        printf("Contraction: wrong idnices");
        printTrace(); exit(1);
    }
    if(dimensionSizes[left]!=dimensionSizes[right])
    {
        printf("Contraction: sizes of dimensions do not match");
        printTrace(); exit(1);
    }
    
#endif

    HDTensor<T> retVal;
    int cumulativeStride = dimensionStrides[left]+dimensionStrides[right];

    if(dimension == 2)
    {
        sizes.push_back(1);
        retVal.setup(sizes);
        HDTensor<T>::iterator it(*this,left,0,right,0);
        *(retVal.getElement(0)) = vector_sum(getElement(it),cumulativeStride,dimensionSizes[left]);
        return retVal;
    }

    sizes.reserve(dimension-2);
    for(int i=0;i<dimension;i++) if(i!=left && i!=right) sizes.push_back(dimensionSizes[i]);
    retVal.setup(sizes);

    for(HDTensor<T>::iterator it(*this,left,0,right,0); it!=this->end();it++)
    {
        vector<int> access;
        access.reserve(dimension-2);

        T retValNum = vector_sum(getElement(it),cumulativeStride,dimensionSizes[left]);
        for(int i=0;i<dimension;i++) if(i!=left && i!=right) access.push_back(it.access[i]);
        *(retVal.getElement(access)) = retValNum;
    }
    return retVal;
}




template <class T>
HDTensor<T> HDTensor<T>::applyOneArgumentMethod(const HDTensor<T> &argument,void (*fun)(T *,int,T*,int,int))
{
    HDTensor<T> retVal;
    retVal.copyStructure(argument);
    int dimension = argument.dimensionSizes.size();
    if(argument.nonstandardStrides == false)
    { /* quick method */
        fun(retVal.data,1,argument.data,1,argument.totalSize);
#ifdef VERBOSE_ARRAY_ACCESS
        for(int i=0;i<argument.totalSize;i++) {
            if(isnan(retVal.data[i]))
            {
                printf("Applying operation %p to %f gave nan",fun,argument.data[i]);
                printTrace(); exit(1);
            }
        }
#endif

    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(argument.dimensionSizes[i]>max)
            {
                max = argument.dimensionSizes[i];
                vectorIndex = i;
            }
        }
        for(iterator it = iterator(argument,vectorIndex,0);it!=argument.end();it++)
        {
            fun(retVal.getElement(it),1,argument.getElement(it),argument.dimensionStrides[vectorIndex],max);

#ifdef VERBOSE_ARRAY_ACCESS
            T * array = retVal.getElement(it);
            T * arg = argument.getElement(it);
            for(int i=0;i<max;i++) {
                if(isnan(array[i]))
                {
                    printf("Applying operation %p on %f gave nan",fun,arg[i*argument.dimensionStrides[vectorIndex]]);
                    printTrace(); exit(1);
                }
            }
#endif
        }
    }
    return retVal;
}

template <class T>
HDTensor<T> HDTensor<T>::exp(const HDTensor<T> &argument)
{
    return applyOneArgumentMethod(argument,vector_exp);
}
template <class T>
HDTensor<T> HDTensor<T>::log(const HDTensor<T> &argument)
{
    return applyOneArgumentMethod(argument,vector_log);
}
template <class T>
HDTensor<T> HDTensor<T>::log1p(const HDTensor<T> &argument)
{
    return applyOneArgumentMethod(argument,vector_log1p);
}
template <class T>
HDTensor<T> HDTensor<T>::sin(const HDTensor<T> &argument)
{
    return applyOneArgumentMethod(argument,vector_sin);
}
template <class T>
HDTensor<T> HDTensor<T>::cos(const HDTensor<T> &argument)
{
    return applyOneArgumentMethod(argument,vector_cos);
}
template <class T>
HDTensor<T> HDTensor<T>::tan(const HDTensor<T> &argument)
{
    return applyOneArgumentMethod(argument,vector_tan);
}




template <class T>
HDTensor<T> HDTensor<T>::pow(const HDTensor<T> &argument,T power)
{
    HDTensor<T> retVal;
    retVal.copyStructure(argument);
    int dimension = argument.dimensionSizes.size();
    if(argument.nonstandardStrides == false)
    { /* quick method */
        vector_powScalar(retVal.data,1,argument.data,1,power,argument.totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(argument.dimensionSizes[i]>max)
            {
                max = argument.dimensionSizes[i];
                vectorIndex = i;
            }
        }
        for(iterator it = iterator(argument,vectorIndex,0);it!=argument.end();it++)
        {
            vector_powScalar(retVal.getElement(it),1,argument.getElement(it),argument.dimensionStrides[vectorIndex],power,max);
        }
    }
    return retVal;
}

template <class T>
int operator==(const HDTensor<T> s, const HDTensor<T> t)
{
    int retVal = 1;
    int dimension = s.dimensionSizes.size();
    int addDimension = t.dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(addDimension!=dimension)
    {
        printf("Tensors with different valences under comparison - dimension %d and %d",addDimension,dimension);
        printTrace(); exit(1);
    }
    
    for(int i=0;i<dimension;i++)
    {
        if(s.dimensionSizes[i]!=t.dimensionSizes[i])
        {
            printf("Tensors with different valences under comparison - in dimension %d size is %d or %d",i,s.dimensionSizes[i],t.dimensionSizes[i]);
            printTrace(); exit(1);
        }
    }
#endif


    for(typename HDTensor<T>::iterator it = s.begin();it!=s.end();it++)
    {
        if(t.getElement(it) != s.getElement(it)) retVal = 0;
    }
    
    return retVal;
}



//template <class T>
//HDTensor<T> HDTensor<T>::operator+(const HDMemoryModel &t)const
//{
//    if(t.getObjectType()==HDTypeDecimal) {
//        const HDPrimitiveEnvelope<T> &op = dynamic_cast<const HDPrimitiveEnvelope<T> &> (t);
//        return *this + *op;
//    }
//    if(t.getObjectType()==HDTypeInteger) {
//        const HDPrimitiveEnvelope<int> &op = dynamic_cast<const HDPrimitiveEnvelope<int> &> (t);
//        return *this + *op;
//    }
//    if(t.getObjectType()==HDTypeTensor) {
//        const HDTensor<T> &op = dynamic_cast<const HDTensor<T> &> (t);
//        return *this + op;
//    }
//
//    printf("Error: Trying to add type %d to tensor\n",t.getObjectType());
//    printTrace(); exit(1);
//}
//
//template <class T>
//HDTensor<T> HDTensor<T>::operator-(const HDMemoryModel &t)const
//{
//    if(t.getObjectType()==HDTypeDecimal) {
//        const HDPrimitiveEnvelope<T> &op = dynamic_cast<const HDPrimitiveEnvelope<T> &> (t);
//        return *this - *op;
//    }
//    if(t.getObjectType()==HDTypeInteger) {
//        const HDPrimitiveEnvelope<int> &op = dynamic_cast<const HDPrimitiveEnvelope<int> &> (t);
//        return *this - *op;
//    }
//    if(t.getObjectType()==HDTypeTensor) {
//        const HDTensor<T> &op = dynamic_cast<const HDTensor<T> &> (t);
//        return *this - op;
//    }
//    
//    printf("Error: Trying to sub type %d from tensor\n",t.getObjectType());
//    printTrace(); exit(1);
//}
//
//
//template <class T>
//HDTensor<T> HDTensor<T>::operator*(const HDMemoryModel &t)const
//{
//    if(t.getObjectType()==HDTypeDecimal) {
//        const HDPrimitiveEnvelope<T> &op = dynamic_cast<const HDPrimitiveEnvelope<T> &> (t);
//        return *this * *op;
//    }
//    if(t.getObjectType()==HDTypeInteger) {
//        const HDPrimitiveEnvelope<int> &op = dynamic_cast<const HDPrimitiveEnvelope<int> &> (t);
//        return *this * *op;
//    }
//    if(t.getObjectType()==HDTypeTensor) {
//        const HDTensor<T> &op = dynamic_cast<const HDTensor<T> &> (t);
//        return *this * op;
//    }
//    
//    printf("Error: Trying to mul type %d to tensor\n",t.getObjectType());
//    printTrace(); exit(1);
//}
//
//
//
//template <class T>
//HDTensor<T> HDTensor<T>::operator/(const HDMemoryModel &t)const
//{
//    if(t.getObjectType()==HDTypeDecimal) {
//        const HDPrimitiveEnvelope<T> &op = dynamic_cast<const HDPrimitiveEnvelope<T> &> (t);
//        return *this / *op;
//    }
//    if(t.getObjectType()==HDTypeInteger) {
//        const HDPrimitiveEnvelope<int> &op = dynamic_cast<const HDPrimitiveEnvelope<int> &> (t);
//        return *this / *op;
//    }
//    
//    printf("Error: Trying to divide tensor by %d\n",t.getObjectType());
//    printTrace(); exit(1);
//}


template <class T>
HDTensor<T> HDTensor<T>::operator+(const int &t)const
{
    return *this + (T)t;
}

template <class T>
HDTensor<T> HDTensor<T>::operator-(const int &t)const
{
    return *this - (T)t;
}

template <class T>
HDTensor<T> HDTensor<T>::operator*(const int &t)const
{
    return *this * (T)t;
}
template <class T>
HDTensor<T> HDTensor<T>::operator/(const int &t)const
{
    return *this / (T)t;
}


template <class T>
HDTensor<T>& HDTensor<T>::operator+=(const int &t)
{
    return *this += (T)t;
}

template <class T>
HDTensor<T>& HDTensor<T>::operator-=(const int &t)
{
    return *this -= (T)t;
}

template <class T>
HDTensor<T>& HDTensor<T>::operator*=(const int &t)
{
    return *this *= (T)t;
}
template <class T>
HDTensor<T>& HDTensor<T>::operator/=(const int &t)
{
    return *this /= (T)t;
}




template <class T>
HDTensor<T> HDTensor<T>::operator+(const T &t)const
{
//        printf(" &&& +T &&&\n");
    int dimension = dimensionSizes.size();
    HDTensor<T> retVal;
    
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false)
    { /* quick method */
        vector_addScalar(retVal.data,1,data,1,t,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_addScalar(retVal.data,1,getElement(it),dimensionStrides[vectorIndex],t,max );
        }
    }
    return retVal;
}


template <class T>
HDTensor<T> HDTensor<T>::operator-(const T &t)const
{
//    printf(" &&& -T &&&\n");
    int dimension = dimensionSizes.size();
    HDTensor<T> retVal;

    
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false)
    { /* quick method */
        vector_subScalar(retVal.data,1,data,1,t,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_subScalar(retVal.data,1,getElement(it),dimensionStrides[vectorIndex],t,max );
        }
    }
    return retVal;
}


template <class T>
HDTensor<T> HDTensor<T>::operator*(const T &t)const
{
//    printf(" &&& T * %f &&&\n",t);
    int dimension = dimensionSizes.size();
    HDTensor<T> retVal;
    
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false)
    { /* quick method */
        vector_mulScalar(retVal.data,1,data,1,t,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_mulScalar(retVal.data,1,getElement(it),dimensionStrides[vectorIndex],t,max );
        }
    }
    return retVal;
}


template <class T>
HDTensor<T> HDTensor<T>::operator/(const T &t)const
{
    int dimension = dimensionSizes.size();
    HDTensor<T> retVal;
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false)
    { /* quick method */
        vector_divScalar(retVal.data,1,data,1,t,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_divScalar(retVal.data,1,getElement(it),dimensionStrides[vectorIndex],t,max );
        }
    }
    return retVal;
}


template <class T>
HDTensor<T> HDTensor<T>::operator*(const HDTensor<T> &t)const
{  /* This is matrix mul */
   //return HDTensor<T>::tensorProduct(*this,t).contract(this->dimensionSizes.size()-1,this->dimensionSizes.size());
   // This is ordinary
    int dimension = dimensionSizes.size();
    int addDimension = t.dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(addDimension!=dimension)
    {
        printf("Tensors with different valences under multiplication - dimension %d and %d",addDimension,dimension);
        printTrace(); exit(1);
    }
    
    for(int i=0;i<dimension;i++)
    {
        if(dimensionSizes[i]!=t.dimensionSizes[i])
        {
            printf("Tensors with different valences under multiplication - in dimension %d size is %d or %d",i,dimensionSizes[i],t.dimensionSizes[i]);
            printTrace(); exit(1);
        }
    }
#endif
    HDTensor<T> retVal;
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false && t.nonstandardStrides == false)
    { /* quick method */
        vector_mul(retVal.data,1,data,1,t.data,1,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_mul(retVal.getElement(it),1,getElement(it),dimensionStrides[vectorIndex],t.getElement(it),t.dimensionStrides[vectorIndex],max );
        }
    }
    return retVal;

}

template <class T>
HDTensor<T> HDTensor<T>::operator/(const HDTensor<T> &t)const
{
    int dimension = dimensionSizes.size();
    int addDimension = t.dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(addDimension!=dimension)
    {
        printf("Tensors with different valences under division - dimension %d and %d",addDimension,dimension);
        printTrace(); exit(1);
    }
    
    for(int i=0;i<dimension;i++)
    {
        if(dimensionSizes[i]!=t.dimensionSizes[i])
        {
            printf("Tensors with different valences under division - in dimension %d size is %d or %d",i,dimensionSizes[i],t.dimensionSizes[i]);
            printTrace(); exit(1);
        }
    }
#endif
    HDTensor<T> retVal;
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false && t.nonstandardStrides == false)
    { /* quick method */
        vector_div(retVal.data,1,data,1,t.data,1,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_div(retVal.getElement(it),1,getElement(it),dimensionStrides[vectorIndex],t.getElement(it),t.dimensionStrides[vectorIndex],max );
        }
    }
    return retVal;
    
}



template <class T>
HDTensor<T> HDTensor<T>::operator+(const HDTensor<T> &t)const
{
    int dimension = dimensionSizes.size();
    int addDimension = t.dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(addDimension!=dimension)
    {
        printf("Tensors with different valences under addition - dimension %d and %d",addDimension,dimension);
        printTrace(); exit(1);
    }
    
    for(int i=0;i<dimension;i++)
    {
        if(dimensionSizes[i]!=t.dimensionSizes[i])
        {
            printf("Tensors with different valences under addition - in dimension %d size is %d or %d",i,dimensionSizes[i],t.dimensionSizes[i]);
            printTrace(); exit(1);
        }
    }
#endif
    HDTensor<T> retVal;
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false && t.nonstandardStrides == false)
    { /* quick method */
        vector_add(retVal.data,1,data,1,t.data,1,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_add(retVal.getElement(it),1,getElement(it),dimensionStrides[vectorIndex],t.getElement(it),t.dimensionStrides[vectorIndex],max );
        }
    }
    return retVal;
}

template <class T>
HDTensor<T> HDTensor<T>::operator-(const HDTensor<T> &t)const
{
//        printf(" &&& -Tens &&&\n");
    int dimension = dimensionSizes.size();
    int addDimension = t.dimensionSizes.size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(addDimension!=dimension)
    {
        printf("Tensors with different valences under addition - dimension %d and %d",addDimension,dimension);
        printTrace(); exit(1);
    }
    
    for(int i=0;i<dimension;i++)
    {
        if(dimensionSizes[i]!=t.dimensionSizes[i])
        {
            printf("Tensors with different valences under addition - in dimension %d size is %d or %d",i,dimensionSizes[i],t.dimensionSizes[i]);
            printTrace(); exit(1);
        }
    }
#endif
    HDTensor<T> retVal;
    retVal.copyStructure(*this);
    
    if(nonstandardStrides == false && t.nonstandardStrides == false)
    { /* quick method */
        vector_sub(retVal.data,1,data,1,t.data,1,totalSize);
    }
    else
    { /* slow method */
        
        int max = 0;
        int vectorIndex = -1;
        
        /* find the largest dimension */
        for(int i=dimension-1;i>=0;i--)
        {
            if(dimensionSizes[i]>max)
            {
                max = dimensionSizes[i];
                vectorIndex = i;
            }
        }
        
        for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
        {
            vector_sub(retVal.getElement(it),1,getElement(it),dimensionStrides[vectorIndex],t.getElement(it),t.dimensionStrides[vectorIndex],max );
        }
    }
    return retVal;
}


//template <class T>
//HDTensor<T>& HDTensor<T>::operator=(const HDMemoryModel &opa)
//{
//    if(opa.getObjectType()!=this->getObjectType()) {
//        printf("Error: Trying to substitute type %d for tensor\n",opa.getObjectType());
//        printTrace(); exit(1);
//    }
//    const HDTensor<T> &op = dynamic_cast<const HDTensor<T> &> (opa);
//    return *this = op;
//}


template <class T>
HDTensor<T>& HDTensor<T>::operator=(const T tt)
{
    if(!data) return *this;
    
    for(iterator it = this->begin();it!=this->end();it++)
        (*this->getElement(it)) = tt;
    return *this;
}



template <class T>
HDTensor<T>& HDTensor<T>::operator=(const HDTensor<T> &t)
{
    
#ifdef VERBOSE_MEMORY
    printf("--> %p Substitution operator called from %p\n",this,&t);
#endif
    int dimension = dimensionSizes.size();
    if(data)
    {
#ifdef VERBOSE_ARRAY_ACCESS
        if(t.dimensionSizes.size()!=dimensionSizes.size())
        {
            printf("Trying to assign tensor of dimension %lu to a tensor of dimension %lu",t.dimensionSizes.size(),dimensionSizes.size());
            printTrace(); exit(1);
        }
        for(int i=0;i<dimensionSizes.size();i++)
        {
            if(dimensionSizes[i]!=t.dimensionSizes[i])
            {
                printf("Error in assignment for dimension %d. Left hand tensor has size %d and right hand %d on this dimension.",i,dimensionSizes[i],t.dimensionSizes[i]);
                printTrace(); exit(1);
            }
        }
#endif
        if(t.nonstandardStrides == false && nonstandardStrides == false)
        {
            vector_copy(data,1,t.data,1,totalSize);
        }
        else
        {
            int max = 0;
            int vectorIndex = -1;
            
            /* find the largest dimension */
            for(int i=dimension-1;i>=0;i--)
            {
                if(dimensionSizes[i]>max)
                {
                    max = dimensionSizes[i];
                    vectorIndex = i;
                }
            }

            
            for(iterator it = iterator(*this,vectorIndex,0);it!=end();it++)
            {
//                printf("Copying strides %d %d\n",dimensionStrides[vectorIndex],t.dimensionStrides[vectorIndex]);
                
                vector_copy(getElement(it),dimensionStrides[vectorIndex],t.getElement(it),t.dimensionStrides[vectorIndex],max );

            }
        }
        
    }
    else
    {
        referenceForPointer(t.data)->retain();
        this->dimensionStrides = t.dimensionStrides;
        this->dimensionSizes = t.dimensionSizes;
        this->data = t.data;
        this->totalSize = t.totalSize;
        this->shift = t.shift;
        this->nonstandardStrides = t.nonstandardStrides;
    }
        
    
    return *this;
}


template <class T>
HDTensor<T>& HDTensor<T>::operator+=(const HDTensor<T> &t)
{
    return *this = *this + t;
}

//template <class T>
//HDTensor<T>& HDTensor<T>::operator+=(const HDMemoryModel &t)
//{
//    return *this = *this + t;
//}
//
template <class T>
HDTensor<T>& HDTensor<T>::operator+=(const T &t)
{
    return *this = *this + t;
}

template <class T>
HDTensor<T>& HDTensor<T>::operator-=(const HDTensor<T> &t)
{
    return *this = *this - t;
}

template <class T>
HDTensor<T>& HDTensor<T>::operator-=(const T &t)
{
    return *this = *this - t;
}
//template <class T>
//HDTensor<T>& HDTensor<T>::operator-=(const HDMemoryModel &t)
//{
//    return *this = *this - t;
//}




template <class T>
HDTensor<T>& HDTensor<T>::operator*=(const T &t)
{
    return *this = *this * t;
}

//template <class T>
//HDTensor<T>& HDTensor<T>::operator*=(const HDMemoryModel &t)
//{
//    return *this = *this * t;
//}



template <class T>
HDTensor<T>& HDTensor<T>::operator/=(const T &t)
{
    return *this = *this / t;
}

//template <class T>
//HDTensor<T>& HDTensor<T>::operator/=(const HDMemoryModel &t)
//{
//    return *this = *this / t;
//}



template <class T>
T& HDTensor<T>::operator[](HDTensor<T>::iterator &arg)
{
    return *getElement(arg);
}


#pragma mark - Friends

template<class T>
HDTensor<T> operator+(const T &t,const HDTensor<T> &arg)
{
    return arg+t;
}

template<class T>
HDTensor<T> operator-(const T &t,const HDTensor<T> &arg)
{
    return arg*(-1)+t;
}

template<class T>
HDTensor<T> operator*(const T &t,const HDTensor<T> &arg)
{
    return arg*t;
}


template<class T>
HDTensor<T> operator+(const int &t,const HDTensor<T> &arg)
{
    return arg+(T)t;
}

template<class T>
HDTensor<T> operator-(const int &t,const HDTensor<T> &arg)
{
    return arg*(-1)+(T)t;
}

template<class T>
HDTensor<T> operator*(const int &t,const HDTensor<T> &arg)
{
    return arg * (T)t;
}

template<class T>
void HDTensor<T>::printResults()
{
  printf("XXXXXXXXXXX %d %d\n",constructorCount,getElementCount);
}


//template<class T>
//HDTensor<T> operator+(const HDMemoryModel &t,const HDTensor<T> &arg)
//{
//    return arg+t;
//}
//
//template<class T>
//HDTensor<T> operator-(const HDMemoryModel &t,const HDTensor<T> &arg)
//{
//    return arg*(-1)+t;
//}
//
//template<class T>
//HDTensor<T> operator*(const HDMemoryModel &t,const HDTensor<T> &arg)
//{
//    return arg*t;
//}











