//
//  HDList.h
//  Hodor
//
//  Created by Test on 28.11.2013.
//
//

#ifndef __Hodor__HDList__
#define __Hodor__HDList__

#include <iostream>
#include "HDMemoryModel.h"
#include <list>
#include "HDUniversalEnvelope.h"
#include <unordered_map>
#include <string.h>

using namespace std;

struct eqstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    }
};

template <class T,class C>
class HDListContent : public HDMemoryModel
{
  public:
      list<HDUniversalEnvelope<T,C> > content;
      HDListContent() : HDMemoryModel(HDTypeListContent)
      {
      }
};


template <class T,class C>
class HDList : public HDMemoryModel
{
private:
    HDListContent<T,C> *content;
    unordered_map<std::string, int> hashes;
    
    
public:
    
    class iterator
    {
    public:
        typename list<HDUniversalEnvelope<T,C> >::iterator position;
        
        iterator(const HDList &envelope)
        {
            position = envelope.content->content.begin();
        }
        
        HDUniversalEnvelope<T,C> operator*()
        {
            return *position;
        }
        
        iterator &operator++()
        {
            ++position;
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
        }
    };
    iterator begin() const
    {
        return iterator(*this);
    }
    
    iterator end() const
    {
        iterator end = iterator(*this);
        end.position = this->content->content.end();
        return end;
    }
    
    HDList(const HDList<T,C> &list);
    HDList();
    HDList(int argNo,...);
    HDList& append(const HDMemoryModel &object);
    
    HDList& append(const T t);
    HDList& append(const int t);

    
    HDList<T,C>& operator=(const HDList<T,C> &list);
    
    void verifyAndAnnotateOperands(int opNo, ...);
    
    
    HDUniversalEnvelope<T,C> operator[](const int address) const;
    HDUniversalEnvelope<T,C> operator[](const std::string ss) const;
    virtual ~HDList();
    HDList copy() const;
    int size() const;
    HDList subList(int low, int high) const;
    virtual HDMemoryModel *createWeakCopy() const;
    virtual HDMemoryModel *createStrongCopy() const;
    
    
    /* runtime methods */
    
    int HDFunction__count() const
    {
        return size();
    }
    
    void HDFunction__append(const HDMemoryModel &m) 
    {
        append(m);
    }
    
    void HDFunction__append(T t) 
    {
        append(t);
    }

    void HDFunction__append(int t) 
    {
        append(t);
    }

    
    
    HDList HDFunction__copy() const
    {
        return copy();
    }

    
    
};

#endif /* defined(__Hodor__HDList__) */
