//
//  HDMemoryModel.h
//  Hodor
//
//  Created by Marek Lipert on 03.11.2013.
//
//

#ifndef __Hodor__HDMemoryModel__
#define __Hodor__HDMemoryModel__

#include <iostream>


template <class T>
class HDTensor;

template <class T,class C>
class HDStringEnvelope;

template <class T>
class HDPrimitiveEnvelope;



enum HDType {
    HDTypeUnknown = 0,
    HDTypeTensor,
    HDTypeString,
    HDTypeList,
    HDTypeDictionary,
    HDTypeClass,
    HDTypeDecimal,
    HDTypeInteger,
    HDTypeEnvelope,
    HDTypeStringInfo,
    HDTypeListContent
};

    void printTrace();


class HDMemoryModel
{
    private:
     int referenceCount;
     int testForDoubleFree;
    public:
     HDType objectType;
    
    HDMemoryModel(HDType type);
    void release();
    void retain();
    HDType getObjectType() const;
    
    virtual HDMemoryModel *createWeakCopy() const {return NULL;}
    virtual HDMemoryModel *createStrongCopy() const { return NULL;}
  
    
    virtual ~HDMemoryModel();
};

#endif /* defined(__Hodor__HDMemoryModel__) */
