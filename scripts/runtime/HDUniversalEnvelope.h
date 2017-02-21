//
//  HDUniversalEnvelope.h
//  Hodor
//
//  Created by Test on 22.12.2013.
//
//

#ifndef __Hodor__HDUniversalEnvelope__
#define __Hodor__HDUniversalEnvelope__

#include <iostream>
#include "HDMemoryModel.h"
#include "HDTensor.h"
#include "HDPrimitiveEnvelope.h"

template <class T,class C>
class HDList;

template <class T,class C>
class HDUniversalEnvelope;





template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const int &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const T &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op);

template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const int &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const T &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op);

template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const int &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const T &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op);

template <class T,class C>
HDUniversalEnvelope<T,C> operator/(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator/(const int &t,const HDUniversalEnvelope<T,C> &op);
template <class T,class C>
HDUniversalEnvelope<T,C> operator/(const T &t,const HDUniversalEnvelope<T,C> &op);









template <class T,class C>
class HDUniversalEnvelope : public HDMemoryModel
{
public:
    HDList<T,C> *list;
    HDTensor<T> *tensor;
    HDPrimitiveEnvelope<T> *decimal;
    HDPrimitiveEnvelope<int> *integer;
    HDStringEnvelope<T,C> *string;

    HDType envelopeContent;

    HDUniversalEnvelope();
    
    HDUniversalEnvelope(const HDMemoryModel &o);

    HDUniversalEnvelope(const HDUniversalEnvelope &o);
    
    HDUniversalEnvelope(const HDTensor<T> &o);
    
    HDUniversalEnvelope(const HDList<T,C> &o);
    HDUniversalEnvelope(const HDPrimitiveEnvelope<T> &o);
    HDUniversalEnvelope(const HDPrimitiveEnvelope<int> &o);
    HDUniversalEnvelope(const HDStringEnvelope<T,C> &o);
    
    HDUniversalEnvelope(const T &o);
    virtual ~HDUniversalEnvelope();
    HDUniversalEnvelope(const int &o);
    
    HDType getEnvelopeType()
    {
        return envelopeContent;
    }

    /* casts */
    static int explicitIntCast(const HDUniversalEnvelope<T,C> &op);
    static HDTensor<T> explicitHDTensorCast(const HDUniversalEnvelope<T,C> &op);
    static HDStringEnvelope<T,C> explicitHDStringEnvelopeCast(const HDUniversalEnvelope<T,C> &op);
    static T explicitDecimalCast(const HDUniversalEnvelope<T,C> &op);
    static HDList<T,C> explicitHDListCast(const HDUniversalEnvelope<T,C> &op);

    
    
    
    friend HDUniversalEnvelope<T,C> operator+<>(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator+<>(const int &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator+<>(const T &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator+<>(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op);
    

    friend HDUniversalEnvelope<T,C> operator-<>(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator-<>(const int &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator-<>(const T &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator-<>(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op);
    

    friend HDUniversalEnvelope<T,C> operator*<>(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator*<>(const int &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator*<>(const T &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator*<>(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op);
    

    friend HDUniversalEnvelope<T,C> operator/<>(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator/<>(const int &t,const HDUniversalEnvelope<T,C> &op);
    friend HDUniversalEnvelope<T,C> operator/<>(const T &t,const HDUniversalEnvelope<T,C> &op);

    
    
    int operator==(const HDUniversalEnvelope<T,C> &t) const;
    int operator!=(const HDUniversalEnvelope<T,C> &t) const;
    int operator<=(const HDUniversalEnvelope<T,C> &t) const;
    int operator>=(const HDUniversalEnvelope<T,C> &t) const;
    int operator>(const HDUniversalEnvelope<T,C> &t) const;
    int operator<(const HDUniversalEnvelope<T,C> &t) const;
    
    HDUniversalEnvelope<T,C> operator+(const HDUniversalEnvelope<T,C> &t) const;
    HDUniversalEnvelope<T,C> operator+(const HDTensor<T> &t) const;
    HDUniversalEnvelope<T,C> operator+(const int &t) const;
    HDUniversalEnvelope<T,C> operator+(const T &t) const;
    HDUniversalEnvelope<T,C> operator+(const HDStringEnvelope<T,C> &t) const;
    
    HDUniversalEnvelope<T,C> operator-(const HDUniversalEnvelope<T,C> &t) const;
    HDUniversalEnvelope<T,C> operator-(const HDTensor<T> &t) const;
    HDUniversalEnvelope<T,C> operator-(const int &t) const;
    HDUniversalEnvelope<T,C> operator-(const T &t) const;
    HDUniversalEnvelope<T,C> operator-(const HDStringEnvelope<T,C> &t) const;
    
    HDUniversalEnvelope<T,C> operator*(const HDUniversalEnvelope<T,C> &t) const;
    HDUniversalEnvelope<T,C> operator*(const HDTensor<T> &t) const;
    HDUniversalEnvelope<T,C> operator*(const int &t) const;
    HDUniversalEnvelope<T,C> operator*(const T &t) const;
    HDUniversalEnvelope<T,C> operator*(const HDStringEnvelope<T,C> &t) const;
    
    HDUniversalEnvelope<T,C> operator/(const HDUniversalEnvelope<T,C> &t) const;
    HDUniversalEnvelope<T,C> operator/(const HDTensor<T> &t) const;
    HDUniversalEnvelope<T,C> operator/(const int &t) const;
    HDUniversalEnvelope<T,C> operator/(const T &t) const;
    
    virtual HDMemoryModel *createWeakCopy()const;
    virtual HDMemoryModel *createStrongCopy() const;
};

#endif /* defined(__Hodor__HDUniversalEnvelope__) */
