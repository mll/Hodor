//
//  HDPrimitiveEnvelope.h
//  Hodor
//
//  Created by Test on 29.11.2013.
//
//

#ifndef __Hodor__HDDecimalEnvelope__
#define __Hodor__HDDecimalEnvelope__

#include <iostream>
#include "HDMemoryModel.h"

template <class T>
class HDPrimitiveEnvelope : public HDMemoryModel
{
  private:
        T content;
  public:
    HDPrimitiveEnvelope(const T &val,HDType type);
    HDPrimitiveEnvelope(const HDPrimitiveEnvelope<T> &val);
    HDPrimitiveEnvelope<T> copy() const;
    virtual ~HDPrimitiveEnvelope() {}
    T &operator*();
    T operator*()const;
    operator T();
    virtual HDMemoryModel *createWeakCopy() const;
    virtual HDMemoryModel *createStrongCopy() const;

};

#endif /* defined(__Hodor__HDDecimalEnvelope__) */
