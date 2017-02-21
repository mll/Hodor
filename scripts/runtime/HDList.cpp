//
//  HDList.cpp
//  Hodor
//
//  Created by Test on 28.11.2013.
//
//

#include "HDList.h"
#include "HDDataReference.h"
#include <assert.h>
#include <stdarg.h>


template <class T, class C>
void HDList<T,C>::verifyAndAnnotateOperands(int opNo,...)
{
    va_list ap;
    assert(opNo > 0);
    
    char *name;
    HDType type;
    
#ifdef VERBOSE_ARRAY_ACCESS
    if(opNo != size())
    {
        printf("Error: Trying to return list of size %d where size %d is required.\n",opNo,size());
        printTrace(); exit(1);
    }
#endif
    int i = 0;
    
    va_start(ap,opNo);
    
    for(HDList<T,C>::iterator ii = begin();ii!=end();ii++)
    {
        name = va_arg(ap,char *);
        type = (HDType) va_arg(ap,int);
#ifdef VERBOSE_ARRAY_ACCESS
//        printf("***************** Hello %d\n",i);
        const HDUniversalEnvelope<T,C> & data = *ii;
//        printf("***************** Bye %d\n",i);

        if(data.envelopeContent != type)
        {
            printf("Error: Position %d in the list of size %d should be of type %d and is of type %d.\n",i+1,opNo,type,data.envelopeContent);
            printTrace(); exit(1);
        }
#endif
        if(strlen(name)) hashes[std::string(name)] = i;
        i++;
//        printf("***************** Bye bye %d\n",i-1);
    }
    
    va_end(ap);
}




template<class T,class C>
HDMemoryModel *HDList<T,C>::createWeakCopy() const
{
    return new HDList<T,C>(*this);
}

template<class T,class C>
HDMemoryModel *HDList<T,C>::createStrongCopy() const
{
    return new HDList<T,C>(this->copy());
}

template<class T,class C>
HDList<T,C>::HDList(int argNo,...) : HDMemoryModel(HDTypeList)
{
    va_list ap;
    assert(argNo > 0);
    HDMemoryModel *tmp;
    va_start(ap,argNo);
    content = new HDListContent<T,C>();
    new HDDataReference(content);

    for(int i=0;i<argNo;i++)
    {
        tmp = va_arg(ap,HDMemoryModel *);
        const HDUniversalEnvelope<T,C> &ref = HDUniversalEnvelope<T,C>(*tmp);
        content->content.push_back(ref);
        delete tmp;
    }
    va_end(ap);
}

template<class T,class C>
HDList<T,C>::HDList(const HDList<T,C> &list) : HDMemoryModel(HDTypeList)
{
//    printf("Copying %d",list.size());
    content = list.content;
    hashes = list.hashes;
    referenceForPointer(content)->retain();
}

template<class T,class C>
HDList<T,C>& HDList<T,C>::operator=(const HDList<T,C> &list)
{
    referenceForPointer(list.content)->retain();
    referenceForPointer(content)->release();
    content = list.content;
    hashes  = list.hashes;
    return *this;
}




template<class T,class C>
HDList<T,C>::HDList() : HDMemoryModel(HDTypeList)
{
    content = new HDListContent<T,C>();
    new HDDataReference(content);
}

template<class T,class C>
HDList<T,C>& HDList<T,C>::append(const HDMemoryModel &object)
{
    content->content.push_back(HDUniversalEnvelope<T,C>(object));
    return *this;
}


template<class T,class C>
int HDList<T,C>::size() const
{
    return content->content.size();
}



template<class T,class C>
HDUniversalEnvelope<T,C> HDList<T,C>::operator[](const int address) const
{
    int s = size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(address>=s)
    {
        printf("Error: Trying to access element no. %d of a list of size %d.\n",address+1,s);
        if(s>0) printf("Hint: First element on the list is of type %d.\n",(*this)[0].getEnvelopeType());
        printTrace(); exit(1);
    }
#endif
    iterator it = this->begin();
    for(int i=0;i<address;i++) it++;
    return *it;
}


template<class T,class C>
HDUniversalEnvelope<T,C> HDList<T,C>::operator[](const std::string ss) const
{
    unordered_map<std::string,int>::const_iterator ita = hashes.find(ss);
    
    int s = size();
#ifdef VERBOSE_ARRAY_ACCESS
    if(ita == hashes.end())
    {
        printf("Error: Trying to access element with key '%s' in a container which does not have it.\n",ss.c_str());
        printTrace();
        exit(1);
    }
#endif
    int address = (*ita).second;
#ifdef VERBOSE_ARRAY_ACCESS
    if(address>=s)
    {
        printf("Error: Trying to access element no. %d of a list of size %d.\n",address+1,s);
        if(s>0) printf("Hint: First element on the list is of type %d.\n",(*this)[0].getEnvelopeType());
        printTrace(); exit(1);
    }
#endif
    iterator it = this->begin();
    for(int i=0;i<address;i++) it++;
    return *it;
}




template<class T,class C>
HDList<T,C>::~HDList()
{
    HDDataReference *ref =  referenceForPointer(content);
//    printf("Deleting list with %d elements and ref count:%d \n",size(),ref->refCount);

    ref->release();
//    printf("Deleted list elements\n",size());

}

template<class T,class C>
HDList<T,C>& HDList<T,C>::append(const T t)
{
    HDPrimitiveEnvelope<T> p = HDPrimitiveEnvelope<T>(t,HDTypeDecimal);
    append(p);
    return *this;
}


template<class T,class C>
HDList<T,C>& HDList<T,C>::append(const int t)
{
    HDPrimitiveEnvelope<int> p = HDPrimitiveEnvelope<int>(t,HDTypeInteger);
    append(p);
    return (*this);
}


template<class T,class C>
HDList<T,C> HDList<T,C>::subList(int low, int high) const
{
    if(low<0) low = 0;
    if(high<0) high = content->size()-1;
    if(high>content->size()-1) {
        printf("Error: Trying to access nonexistent element of the list. size %d ement to be accessed %d.\n",high,content->size()-1);
        printTrace(); exit(1);
    }
    
    HDList newList;
    referenceForPointer(newList.content)->release();
    newList.content = new HDListContent<T,C>();
    new HDDataReference(newList.content);
    int i = 0;
    for(typename list<HDUniversalEnvelope<T,C> >::iterator it = content->content.begin();it!=content->content.end();it++)
    {
        if(i>=low && i<=high) newList.content->content.push_back((*it));
        i++;
    }
    return newList;
}

template<class T,class C>
HDList<T,C> HDList<T,C>::copy() const
{
 //   printf("Copy");
    HDList newList;
    referenceForPointer(newList.content)->release();
    newList.content = new HDListContent<T,C>();
    new HDDataReference(newList.content);
    for(typename list<HDUniversalEnvelope<T,C> >::iterator it = content->content.begin();it!=content->content.end();it++)
    {
        newList.content->content.push_back(*it);
    }
    return newList;
}
