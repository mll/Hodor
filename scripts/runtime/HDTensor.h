//
//  HDTensor.h
//  Hodor
//
//  Created by Marek Lipert on 03.11.2013.
//
//

#ifndef __Hodor__HDTensor__
#define __Hodor__HDTensor__

#include <iostream>
#include "HDMemoryModel.h"
#include <vector>
#include "HDTensorDouble.h"
#include "HDTensorFloat.h"




//#define VERBOSE_ARRAY_ACCESS // for checks

using namespace std;

template <class T>
class HDTensor;

template <class T,class C>
class HDList;

template<class T>
int operator==(const HDTensor<T> s, const HDTensor<T> t);

template<class T>
HDTensor<T> operator+(const T &t,const HDTensor<T> &arg);

template<class T>
HDTensor<T> operator-(const T &t,const HDTensor<T> &arg);

template<class T>
HDTensor<T> operator*(const T &t,const HDTensor<T> &arg);

template<class T>
HDTensor<T> operator+(const int &t,const HDTensor<T> &arg);

template<class T>
HDTensor<T> operator-(const int &t,const HDTensor<T> &arg);

template<class T>
HDTensor<T> operator*(const int &t,const HDTensor<T> &arg);






template <class T>
class HDTensor : public HDMemoryModel
{
   protected:
    T* data;
    std::vector<int> dimensionSizes;
    std::vector<int> dimensionStrides;
    int shift;
    int totalSize;
    bool nonstandardStrides;
    static HDTensor<T> applyOneArgumentMethod(const HDTensor<T> &argument,void (*fun)(T *,int,T*,int,int));
    
   public:
    /* friends */
    friend HDTensor<T> operator*<>(const T &t,const HDTensor<T> &arg);
    friend HDTensor<T> operator+<>(const T &t,const HDTensor<T> &arg);
    friend HDTensor<T> operator-<>(const T &t,const HDTensor<T> &arg);

    friend HDTensor<T> operator*<>(const int &t,const HDTensor<T> &arg);
    friend HDTensor<T> operator+<>(const int &t,const HDTensor<T> &arg);
    friend HDTensor<T> operator-<>(const int &t,const HDTensor<T> &arg);
    friend int operator==<>(const HDTensor<T> s, const HDTensor<T> t);


    class iterator
    {
        public:
        std::vector<int> access;
        int excludedIndex;
        int excludedIndex2;
        
        HDTensor<T> parent;
        
        
        int size() const
        {
            return access.size();
        }
        
        
        iterator(const HDTensor<T> &p)
        {
            this->parent = p;
            for(int i=0;i<p.dimensionSizes.size();i++) access.push_back(0);
            excludedIndex = -1;
            excludedIndex2 = -1;
        }
        iterator(const HDTensor<T> &p, int excludedIndex,int excludedValue)
        {
            this->parent = p;
            for(int i=0;i<p.dimensionSizes.size();i++) access.push_back(0);
            this->excludedIndex = excludedIndex;
            access[excludedIndex] = excludedValue;
            excludedIndex2 = -1;
        }
        
        iterator(const HDTensor<T> &p,vector<int> &initialAccess)
        {
            this->parent = p;
            for(int i=0;i<p.dimensionSizes.size();i++) access.push_back(0);
            excludedIndex = -1;
            access = initialAccess;
            excludedIndex2 = -1;
        }
        iterator(const HDTensor<T> &p, int excludedIndex,int excludedValue,int excludedIndex2, int excludedValue2)
        {
            this->parent = p;
            for(int i=0;i<p.dimensionSizes.size();i++) access.push_back(0);
            this->excludedIndex = excludedIndex;
            access[excludedIndex] = excludedValue;
            this->excludedIndex2 = excludedIndex2;
            access[excludedIndex2] = excludedValue2;

        }


        T & operator*()
        {
            return *(parent.getElement(*this));
        }
        
        iterator &operator++()
        {
                int current = 0;
                int dimension = access.size();
                
                while(current<dimension)
                {
                    if(current!=excludedIndex && current!=excludedIndex2)
                    {
                        if(access[current]+1<this->parent.dimensionSizes[current])
                        {
                            access[current]++;
                            return *this;
                        } 
                        else access[current++] = 0;
                    }
                    else current++;
                }
            for(int i=0;i<dimension;i++) access[i] = -1;
            return *this;
        }
        iterator operator++(int)
        {
            iterator i = *this;
            ++(*this);
            return i;
        }
        
        bool operator==(const iterator &it) const
        {
            bool value = true;
            
            if(size()!=it.size()) value = false;
            for(int i=0;i<size();i++) if(access[i]!=it.access[i]) value = false;
            return value;
        }
        bool operator!=(const iterator &it) const
        {
            return !(*this == it);
        }
        
        void print() const
        {
            for(int i=0;i<size();i++) printf("%d ",access[i]);
            printf("\n");
        }
        
    };
    
    
    iterator begin() const
    {
        return iterator(*this);
    }
    
    iterator end() const
    {
        vector<int> access;
        for(int i=0;i<dimensionSizes.size();i++)
            access.push_back(-1);
        iterator it = iterator(*this,access);
        return it;
    }
    
    HDTensor();
    HDTensor(const HDTensor<T> &t);
    HDTensor<T>& operator=(const T tt);

    
    
    HDTensor(const char *special,const HDTensor<T> &t);

    HDTensor(vector<int> &dimensions);
    HDTensor(int dimension,...);
    
    void setup(vector<int> &dimensions);
    
    
    void copyStructure(const HDTensor<T> &t);
    
    virtual HDMemoryModel *createWeakCopy() const;
    virtual HDMemoryModel *createStrongCopy() const;

    
    
    HDTensor<T> newSubTensor(int dim,int firstLo,int firstHi,...) const;
    HDTensor<T> newReversedSubTensor(int dim,int firstLo,int firstHi,...) const;
    
    HDTensor<T> newSubTensor(const vector<int> &los,const vector<int> &his) const;
    
    HDTensor<T> copy() const;
    HDTensor<T> newCopy() const;
    
    virtual ~HDTensor();
    void print() const;
    
    T* getElement(int count,int access1) const;
    T* getElement(int count,int access1,int access2) const;
    T* getElement(int count,int access1,int access2, int access3) const;
    T* getElement(int count,int access1,int access2,int access3, int access4) const;
    T* getElement(int count,int access1,int access2,int access3, int access4,int access5) const;
    T* getElement(int count,int access1,int access2,int access3, int access4,int access5,int access6) const;
    
    T* getElement(std::vector<int> &access) const;
    T* getElement(iterator &access) const;
    T& operator[](HDTensor<T>::iterator &arg);
    
    /* Operators */
    
    static HDTensor<T> tensorProduct(const HDTensor<T> &t1,const HDTensor<T> &t2);
    
    HDTensor contract(int left, int right) const;
    
    
    static HDTensor<T> pow(const HDTensor<T> &argument,T power);
    static HDTensor<T> exp(const HDTensor<T> &argument);
    static HDTensor<T> log(const HDTensor<T> &argument);
    static HDTensor<T> log1p(const HDTensor<T> &argument);
    static HDTensor<T> sin(const HDTensor<T> &argument);
    static HDTensor<T> cos(const HDTensor<T> &argument);
    static HDTensor<T> tan(const HDTensor<T> &argument);

    HDTensor operator+(const HDTensor &t)const;
    HDTensor operator-(const HDTensor &t)const;
    HDTensor operator*(const HDTensor &t)const;
    HDTensor operator/(const HDTensor &t)const;

    
    
    HDTensor operator+(const T &t)const;
    HDTensor operator-(const T &t)const;
    HDTensor operator*(const T &t)const;

    HDTensor operator+(const int &t)const;
    HDTensor operator-(const int &t)const;
    HDTensor operator*(const int &t)const;

    
    
    
    
    HDTensor operator/(const T &t)const;
    HDTensor operator/(const int &t)const;
    
    
    
//    HDTensor<T> operator+(const HDMemoryModel &t) const;
//    HDTensor<T> operator-(const HDMemoryModel &t) const;
//    HDTensor<T> operator*(const HDMemoryModel &t) const;
//    HDTensor<T> operator/(const HDMemoryModel &t) const;
    
    HDTensor<T>& operator+=(const HDTensor<T> &t);
    HDTensor<T>& operator+=(const T &t);
    HDTensor<T>& operator+=(const int &t);
//    HDTensor<T>& operator+=(const HDMemoryModel &t);

    HDTensor<T>& operator-=(const HDTensor<T> &t);
    HDTensor<T>& operator-=(const T &t);
    HDTensor<T>& operator-=(const int &t);
//    HDTensor<T>& operator-=(const HDMemoryModel &t);
    
    HDTensor<T>& operator*=(const T &t);
//    HDTensor<T>& operator*=(const HDMemoryModel &t);
    HDTensor<T>& operator*=(const int &t);
    
    HDTensor<T>& operator/=(const T &t);
//    HDTensor<T>& operator/=(const HDMemoryModel &t);
    HDTensor<T>& operator/=(const int &t);
    
    HDTensor<T>& operator=(const HDTensor<T> &t);
    
    
    /* runtime methods */
    
    HDList<T,char> HDFunction__size() const
    {
        HDList<T,char> retVal;
        for(int i=0;i<dimensionSizes.size();i++) retVal.append(dimensionSizes[i]);
        return retVal;
    }
    
    HDTensor HDFunction__contract(int axisA, int axisB) const
    {
        return contract(axisA,axisB);
    }

    HDTensor HDFunction__copy() const
    {
        return newCopy();
    }

    void HDFunction__p() const
    {
        print();
    }
    static void printResults();
    
    
//    HDTensor<T>& operator=(const HDMemoryModel &opa);
};

#endif /* defined(__Hodor__HDTensor__) */
