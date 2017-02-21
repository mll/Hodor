//
//  HDDecimalEnvelope.cpp
//  Hodor
//
//  Created by Test on 29.11.2013.
//
//

#include "HDPrimitiveEnvelope.h"

template <class T>
HDPrimitiveEnvelope<T>::HDPrimitiveEnvelope(const T &val,HDType type) : HDMemoryModel(type)
{
    content = val;
}

template <class T>
HDPrimitiveEnvelope<T>::HDPrimitiveEnvelope(const HDPrimitiveEnvelope<T> &val) : HDMemoryModel(val.objectType)
{
//    std::cout<<"Creating"<<val.content<<std::endl;
    content = val.content;
}

template <class T>
T & HDPrimitiveEnvelope<T>::operator*()
{
    return content;
}

template <class T>
T HDPrimitiveEnvelope<T>::operator*() const
{
    return content;
}

template <class T>
HDPrimitiveEnvelope<T>::operator T()
{
    return content;
}



template <class T>
HDPrimitiveEnvelope<T> HDPrimitiveEnvelope<T>::copy() const
{
    return HDPrimitiveEnvelope(*this);
}


template <class T>
HDMemoryModel *HDPrimitiveEnvelope<T>::createWeakCopy() const
{
    HDPrimitiveEnvelope *e = new HDPrimitiveEnvelope(*this);
//    std::cout << e->content << "!!!"<< std::endl;
    return e;
}

template <class T>
HDMemoryModel *HDPrimitiveEnvelope<T>::createStrongCopy() const
{
    return new HDPrimitiveEnvelope(this->copy());
}
