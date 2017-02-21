//
//  HDStringEnvelope.cpp
//  Hodor
//
//  Created by Test on 24.11.2013.
//
//

#include "HDStringEnvelope.h"
#include "HDDataReference.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>




template <class T,class C>
HDMemoryModel *HDStringEnvelope<T,C>::createWeakCopy() const
{
    return new HDStringEnvelope<T,C>(*this);
}

template <class T,class C>
HDMemoryModel *HDStringEnvelope<T,C>::createStrongCopy() const
{
    return new HDStringEnvelope<T,C>(this->copy());
}



template <class T,class C>
int operator==(int t, HDStringEnvelope<T,C> s)
{
    return HDStringEnvelope<T,C>::stringFromInt(t) == s;
}

template <class T,class C>
int operator==(HDStringEnvelope<T,C> s,int t)
{
    return HDStringEnvelope<T,C>::stringFromInt(t) == s;
}


template <class T,class C>
int operator>(int t, HDStringEnvelope<T,C> s)
{
     return HDStringEnvelope<T,C>::stringFromInt(t) > s;
}

template <class T,class C>
int operator>(HDStringEnvelope<T,C> s,int t)
{
        return s > HDStringEnvelope<T,C>::stringFromInt(t) ;
}




template<class T,class C>
HDStringEnvelope<T,C> operator+(const int t,const HDStringEnvelope<T,C> &arg)
{
    return HDStringEnvelope<T,C>::stringFromInt(t) + arg;
}

template<class T,class C>
HDStringEnvelope<T,C> operator*(const int t,const HDStringEnvelope<T,C> &arg)
{
    return arg*t;
}



template <class T,class C>
HDStringEnvelope<T,C>::HDStringEnvelope(const HDStringEnvelope<T,C> &e) : HDMemoryModel(HDTypeString)
{
    info = e.info;
    offset = e.offset;
    endOfString = e.endOfString;
    referenceForPointer(info)->retain();
}

template <class T,class C>
HDStringEnvelope<T,C>::~HDStringEnvelope()
{
//  printf("Destroying string!!!\n");
    referenceForPointer(info)->release();
//  printf("Destroyed string!!!\n");
}

template <class T,class C>
void HDStringEnvelope<T,C>::createNewString(int multiply)
{
    info = new StringInfo<T,C>();
    info->data = (C *)malloc(multiply*INITIAL_BUFFER_LENGTH*sizeof(C));
    info->length = 0;
    info->bufferLength = multiply*INITIAL_BUFFER_LENGTH;
    offset = 0;
    endOfString = 0;
    new HDDataReference(info);
}


template <class T,class C>
HDStringEnvelope<T,C> HDStringEnvelope<T,C>::operator[](int index) const
{
    return HDStringEnvelope<T,C>(*this,index);
}

template <class T,class C>
HDStringEnvelope<T,C> HDStringEnvelope<T,C>::copy() const
{
    C *buff = (C *)malloc(sizeof(C)*length()+1);
    memcpy(buff,info->data+offset,(endOfString-offset)*sizeof(C));
    buff[(endOfString-offset)] = 0;
    const HDStringEnvelope<T,C> &bu = HDStringEnvelope<T,C>(buff);
    free(buff);
    return bu;
}



template <class T,class C>
HDStringEnvelope<T,C>::HDStringEnvelope(const HDStringEnvelope<T,C> &e,int position) : HDMemoryModel(HDTypeString)
{
    if(position>=e.endOfString)
    {
        createNewString(1);
        return;
    }
    info = e.info;
    referenceForPointer(info)->retain();
    offset = position;
    endOfString = position+1;
}

template <class T,class C>
HDStringEnvelope<T,C>::HDStringEnvelope(const HDStringEnvelope<T,C> &e,int positionLo,int positionHi) : HDMemoryModel(HDTypeString)
{
    positionHi++;
    if(positionHi>=e.endOfString)
    {
        printf("Error: Substring hi index %d out of bounds %d. ",positionHi,e.endOfString);
        printTrace(); exit(1);
    }
    
    if(positionLo>=positionHi)
    {
        printf("Error: String low index %d not smaller or equal to hi %d. ",positionLo+1,positionHi,e.endOfString);
        printTrace(); exit(1);
    }
    
    if(positionLo<0)
    {
        printf("Error: String low index %d not smaller or equal to hi %d. ",positionLo+1,positionHi,e.endOfString);
        printTrace(); exit(1);
    }
    
    info = e.info;
    referenceForPointer(info)->retain();
    offset = positionLo;
    endOfString = positionHi;
}



template <class T,class C>
HDStringEnvelope<T,C>::HDStringEnvelope(const C *source) : HDMemoryModel(HDTypeString)
{
    int len = strlen(source)+1;
    createNewString(len/INITIAL_BUFFER_LENGTH+1);
    memcpy(info->data,source,len*sizeof(C));
    info->length = len;
    offset = 0;
    endOfString = len-1;
}

template <class T,class C>
HDStringEnvelope<T,C> HDStringEnvelope<T,C>::stringFromInt(const int &source)
{
    HDStringEnvelope<T,C> retVal;
    snprintf(retVal.info->data,retVal.info->bufferLength,"%d",source);
    retVal.info->length = strlen(retVal.info->data)+1;
    retVal.endOfString = retVal.info->length-1;
    return retVal;
}

template <class T,class C>
HDStringEnvelope<T,C>::HDStringEnvelope() : HDMemoryModel(HDTypeString)
{
    createNewString(1);
}


template <class T,class C>
HDStringEnvelope<T,C> &HDStringEnvelope<T,C>::operator=(const HDStringEnvelope<T,C> &op)
{
    if(length()==1 && op.length() == 1)
    {
        memcpy(info->data+offset,op.info->data+op.offset,sizeof(C));
        return *this;
    }
    StringInfo<T,C> *i = info;
    info = op.info;
    offset = op.offset;
    endOfString = op.endOfString;
    referenceForPointer(info)->retain();
    referenceForPointer(i)->release();
    return *this;
}

//template <class T,class C>
//HDStringEnvelope<T,C> &HDStringEnvelope<T,C>::operator=(const HDMemoryModel &opa)
//{
//    if(opa.getObjectType()!=HDTypeString) {
//        printf("Error: Trying to substitute type %d for string\n",opa.getObjectType());
//        printTrace(); exit(1);
//    }
//    const HDStringEnvelope<T,C> &op = dynamic_cast<const HDStringEnvelope<T,C> &> (opa);
//    return (*this = op);
//}


//template <class T,class C>
//HDStringEnvelope<T,C> &HDStringEnvelope<T,C>::operator=(const int op)
//{
//    *this = HDStringEnvelope<T,C>::stringFromInt(op);
//}

template <class T,class C>
HDStringEnvelope<T,C> HDStringEnvelope<T,C>::operator+(const HDStringEnvelope<T,C> &op) const
{
    HDStringEnvelope<T,C> retVal = this->copy();
    return retVal+=op;
}

template <class T,class C>
HDStringEnvelope<T,C> HDStringEnvelope<T,C>::operator+(const int op) const
{
    HDStringEnvelope<T,C> retVal = this->copy();
    return retVal+=op;
}


template <class T,class C>
HDStringEnvelope<T,C> HDStringEnvelope<T,C>::operator*(const int op) const
{
    HDStringEnvelope<T,C> retVal = this->copy();
    return retVal*=op;
}

template <class T,class C>
void HDStringEnvelope<T,C>::reallocateToNewSize(const int newSize)
{
    info->data = (C *)realloc(info->data,(newSize/INITIAL_BUFFER_LENGTH+1)*INITIAL_BUFFER_LENGTH*sizeof(C));
    info->bufferLength = (newSize/INITIAL_BUFFER_LENGTH+1)*INITIAL_BUFFER_LENGTH;
}

template <class T,class C>
HDStringEnvelope<T,C> &HDStringEnvelope<T,C>::operator+=(const HDStringEnvelope<T,C> &op)
{
    int newSize = op.endOfString-op.offset + endOfString;
    if(newSize>info->bufferLength)
    {
        reallocateToNewSize(newSize);
    }
    memcpy(info->data+endOfString,op.info->data+op.offset,(op.endOfString-op.offset)*sizeof(C));
    info->length = endOfString + (op.endOfString-op.offset)+1;
    endOfString += op.endOfString-op.offset;
    return (*this);
}


template <class T,class C>
HDStringEnvelope<T,C> &HDStringEnvelope<T,C>::operator+=(const int op)
{
    return (*this)+=HDStringEnvelope<T,C>::stringFromInt(op);
}

template <class T,class C>
HDStringEnvelope<T,C> &HDStringEnvelope<T,C>::operator*=(const int op)
{
    HDStringEnvelope<T,C> tmp = this->copy();
    for(int i=1;i<op;i++)
      (*this)+=tmp;
    return *this;
}
template <class T,class C>
bool HDStringEnvelope<T,C>::operator==(const HDStringEnvelope<T,C> &op) const
{
    std::string s = op.c_str();
    return this->c_str().compare(s) == 0;
//    int len = endOfString- offset;
//    if(op.endOfString - op.offset!= len) return false;
//    len*=sizeof(C);
//    int i = offset*sizeof(C);
//    int j = op.offset*sizeof(C);
//    char *data1 = (char *)info->data;
//    char *data2 = (char *)op.info->data;
//    for(int cnt = 0;cnt<len;cnt++)
//    {
//        if(data1[i++]!=data2[j++]) return false;
//    }
//    return true;
}


template <class T,class C>
bool HDStringEnvelope<T,C>::operator>(const HDStringEnvelope<T,C> &op) const
{
    std::string s = op.c_str();
    return this->c_str().compare(s) < 0;
}





template <class T,class C>
bool HDStringEnvelope<T,C>::operator==(const int op) const
{
    return *this == HDStringEnvelope<T,C>::stringFromInt(op);
}


template <class T,class C>
bool HDStringEnvelope<T,C>::HDStringEnvelope<T,C>::operator!=(const HDStringEnvelope<T,C> &op) const
{
    return !(*this == op);
}
template <class T,class C>
bool HDStringEnvelope<T,C>::HDStringEnvelope<T,C>::operator!=(const int op) const
{
    return !(*this == op);
}

template <class T,class C>
int HDStringEnvelope<T,C>::length() const
{
    return endOfString-offset;
}




