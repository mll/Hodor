//
//  HDStringEnvelope.h
//  Hodor
//
//  Created by Test on 24.11.2013.
//
//

#ifndef __Hodor__HDStringEnvelope__
#define __Hodor__HDStringEnvelope__

#include <iostream>
#include "HDMemoryModel.h"

#define INITIAL_BUFFER_LENGTH 400//8192
using namespace std;

template <class T,class C>
class HDStringEnvelope;

template<class T,class C>
HDStringEnvelope<T,C> operator+(const int t,const HDStringEnvelope<T,C> &arg);

template<class T,class C>
HDStringEnvelope<T,C> operator*(const int t,const HDStringEnvelope<T,C> &arg);



template <class T,class C>
int operator==(int t, HDStringEnvelope<T,C> s);

template <class T,class C>
int operator==(HDStringEnvelope<T,C> s,int t);


template <class T,class C>
int operator>(int t, HDStringEnvelope<T,C> s);

template <class T,class C>
int operator>(HDStringEnvelope<T,C> s,int t);





template <class T,class C>
class HDList;




template <class T,class C>
class StringInfo : public HDMemoryModel
{
public:
    C *data;
    int length;
    int bufferLength;
    int charSize;
    StringInfo() : HDMemoryModel(HDTypeStringInfo)
    {
    
    }
  
    ~StringInfo()
    {
//      printf("Freeing string data: %d\n",bufferLength);
//      fflush(stdout);
      if(data) free(data);
    }
};

template <class T,class C>
class HDStringEnvelope : public HDMemoryModel
{
    private:
    void reallocateToNewSize(const int newSize);
    void createNewString(int multiply);
    int offset;
    int endOfString;
    
    StringInfo<T,C> *info;
    
    public:
    virtual ~HDStringEnvelope();
    
    friend HDStringEnvelope<T,C> operator*<>(const int t,const HDStringEnvelope<T,C> &arg);
    friend HDStringEnvelope<T,C> operator+<>(const int t,const HDStringEnvelope<T,C> &arg);
    friend int operator==(T t, const HDStringEnvelope<T,C> &s);
    friend int operator>(T t, const HDStringEnvelope<T,C> &s);

    virtual HDMemoryModel *createWeakCopy() const;
    virtual HDMemoryModel *createStrongCopy() const;

    
    static HDStringEnvelope<T,C> stringFromInt(const int &i);
    
    
    HDStringEnvelope(const HDStringEnvelope<T,C> &e);
    HDStringEnvelope(const HDStringEnvelope<T,C> &e,int position);
    HDStringEnvelope(const HDStringEnvelope<T,C> &e,int positionLo,int positionHi);
    HDStringEnvelope(const C *source);

    operator std::string() const {
        return c_str();
    }
    
    HDStringEnvelope();

    
    
    HDStringEnvelope<T,C> &operator=(const HDStringEnvelope<T,C> &op);
//    HDStringEnvelope<T,C> &operator=(const int op);
//    HDStringEnvelope<T,C> &operator=(const HDMemoryModel &opa);
    
    HDStringEnvelope<T,C> operator+(const HDStringEnvelope<T,C> &op) const;
    HDStringEnvelope<T,C> operator+(const int op) const;
    
    HDStringEnvelope<T,C> operator*(const int op) const;
    
    HDStringEnvelope<T,C> &operator+=(const HDStringEnvelope<T,C> &op);
    HDStringEnvelope<T,C> &operator+=(const int op);
    
    HDStringEnvelope<T,C> &operator*=(const int op);
    
    bool operator==(const HDStringEnvelope<T,C> &op) const;
    bool operator>(const HDStringEnvelope<T,C> &op) const;
    bool operator==(const int op) const;
    
    
    bool operator!=(const HDStringEnvelope<T,C> &op) const;
    bool operator!=(const int op) const;
    
    int length() const;
    
    HDStringEnvelope<T,C> operator[](int index) const;
    HDStringEnvelope<T,C> copy() const;
    std::string c_str() const
    {
        std::string out = "";
        for(int i=0;i<length();i++)
        {
            out+=info->data[i+offset];
        }
        return out;
    }
    
    class iterator
    {
    public:
        const HDStringEnvelope<T,C> * parent;
        int position;
        
        iterator(const HDStringEnvelope<T,C> &envelope)
        {
            position = envelope.offset;
            parent = &envelope;
        }
        
        HDStringEnvelope operator*()
        {
            return HDStringEnvelope(*parent,position);
        }
        iterator &operator++()
        {
            if(parent->endOfString > position) position++;
            return *this;
        }
        bool operator==(const iterator &it) const
        {
            return position == it.position;
        }
        bool operator!=(const iterator &it) const
        {
            return position != it.position;
        }
        iterator operator++(int)
        {
            iterator i = *this;
            ++(*this);
            return i;
        }
        iterator(const iterator &it)
        {
            position = it.position;
            parent = it.parent;
        }
    };
    iterator begin() const
    {
        return iterator(*this);
    }
    
    iterator end() const
    {
        iterator end = iterator(*this);
        end.position = endOfString;
        return end;
    }
    /* ********************** methods ********************* */
    
    HDStringEnvelope<T,C> HDFunction__copy() const
    {
        return copy();
    }
    
    int HDFunction__length() const
    {
        return length();
    }
    
    void HDFunction__p() const
    {
        cout<<c_str();
        fflush(stdout);
    }
    
    HDList<T,C> HDFunction__scanf(HDStringEnvelope<T,C> args) const
    { /* NOT YET */
        return HDList<double,C>();
    }
    
};

#endif /* defined(__Hodor__HDStringEnvelope__) */
