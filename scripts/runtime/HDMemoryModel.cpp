//
//  HDMemoryModel.cpp
//  Hodor
//
//  Created by Marek Lipert on 03.11.2013.
//
//

#include "HDMemoryModel.h"
#include <execinfo.h>
#include <unistd.h>
#include <string.h>

int objectsAllocated = 0;

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



void HDMemoryModel::retain()
{
    this->referenceCount++;
}

void HDMemoryModel::release()
{
    if(--(this->referenceCount) == 0)
    {
        delete this;
    }
}


HDMemoryModel::HDMemoryModel(HDType type)
{
    referenceCount = 1;
    objectsAllocated++;
    this->objectType = type;
    testForDoubleFree = 1;
#ifdef VERBOSE_MEMORY
    printf("===> Allocating object of type %d pointer %p objects left: %d\n",objectType,this,objectsAllocated);
    //print_trace();
#endif
}

HDType HDMemoryModel::getObjectType() const
{
    return objectType;
}

HDMemoryModel::~HDMemoryModel()
{
  objectsAllocated--;
  if(--testForDoubleFree != 0)
  {
    printf("Error! Double free on object of type %d pointer %p\n",objectType,this);
    exit(1);
  }
#ifdef VERBOSE_MEMORY
    printf("===> Free on object of type %d pointer %p objects left: %d\n",objectType,this,objectsAllocated);
    //print_trace();
#endif
}


void printTrace()
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