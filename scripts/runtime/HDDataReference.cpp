//
//  HDDataReference.cpp
//  Hodor
//
//  Created by Test on 11.11.2013.
//
//

#include "HDDataReference.h"
#include <vector>
#include <assert.h>
#include <unordered_map>
#include "HDDataReference.h"

using namespace std;

static unordered_map<void *,HDDataReference *> dataReferences;
static int allocatedData = 0;
static int allocatedObjects = 0;

int ad()
{
  return allocatedData;
}

int ao()
{
  return allocatedObjects;
}


HDDataReference::HDDataReference(void *d)
{
    data = d;
    objectData = NULL;
    refCount = 1;
#ifdef VERBOSE_MEMORY
    printf("*** %p HDDataReference CREATE constructor called %p\n",this,d);
    printReferenceState();
#endif
    if(d==NULL)
    {
      printf("KKKK\n");
    }
    assert(d != NULL);
    allocatedData++;
    dataReferences[d] = this;
}


HDDataReference::HDDataReference(HDMemoryModel *d)
{
    data = NULL;
    objectData = d;
    refCount = 1;
    assert(d != NULL);
#ifdef VERBOSE_MEMORY
    printf("*** %p HDDataReference CREATE constructor called %p\n",this,(void *)d);
#endif
    dataReferences[(void *)d] = this;
    allocatedObjects++;
}



HDDataReference::HDDataReference(const HDDataReference &t)
{
#ifdef VERBOSE_MEMORY
    printf("*** %p HDDataReference copy constructor called %p\n",this,t.data);
#endif
    data = t.data;
    objectData = t.objectData;
    refCount = t.refCount;
}

void HDDataReference::retain()
{
#ifdef VERBOSE_MEMORY
    printf("*** %p HDDataReference retain called for %p, refCount after: %d\n",this,data,refCount+1);
#endif
    refCount++;
}

void HDDataReference::release()
{
#ifdef VERBOSE_MEMORY
    printf("*** %p HDDataReference release called for %p, refCount after: %d\n",this,data,refCount-1);
    printReferenceState();
#endif
    if(--refCount == 0)
    {
      dataReferences.erase(objectData?(void *)objectData:data);
      delete this;
    }
}


void printReferenceState()
{
  printf("-------------------> Data references remaining: %lu (object+pointer)\n",dataReferences.size());
}

HDDataReference::~HDDataReference()
{
#ifdef VERBOSE_MEMORY
  printf("================== %p Destructor called for %s reference %p. Objects left: %lu\n",this,data ? "data" : "object",data?data:objectData,dataReferences.size());
#endif
    if(objectData) 
    {
      allocatedObjects--;
      delete objectData;
    }
    else
    {
      allocatedData--;
      free(data);
    }
  
}



HDDataReference *referenceForPointer(void *p)
{
    HDDataReference *retVal = dataReferences[(void *)p];
#ifdef VERBOSE_MEMORY
    printf("================== Trying to get reference for pointer %p --> %p\n",p,retVal);
#endif

    return retVal;
}

HDDataReference *referenceForPointer(HDMemoryModel *p)
{
    HDDataReference *retVal = dataReferences[(void *)p];
#ifdef VERBOSE_MEMORY
    printf("================== Trying to get reference for pointer %p --> %p\n",p,retVal);
#endif
    return retVal;
}

