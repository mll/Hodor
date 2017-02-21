//
//  HDDataReference.h
//  Hodor
//
//  Created by Test on 11.11.2013.
//
//

#ifndef __Hodor__HDDataReference__
#define __Hodor__HDDataReference__

#include <iostream>
#include "HDMemoryModel.h"

//#define VERBOSE_MEMORY

class HDDataReference
{
private:
    void * data;
    HDMemoryModel *objectData;
public:
    int refCount;
    HDDataReference(void *d);
    HDDataReference(HDMemoryModel *d);
    HDDataReference(const HDDataReference &t);
    void retain();
    void release();
    ~HDDataReference();
    friend HDDataReference *referenceForPointer(void *p);
    friend HDDataReference *referenceForPointer(HDMemoryModel *p);
};


HDDataReference *referenceForPointer(void *p);
HDDataReference *referenceForPointer(HDMemoryModel *p);

void printReferenceState();


#endif /* defined(__Hodor__HDDataReference__) */
