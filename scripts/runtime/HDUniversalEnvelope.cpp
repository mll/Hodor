//
//  HDUniversalEnvelope.cpp
//  Hodor
//
//  Created by Test on 22.12.2013.
//
//

#include "HDUniversalEnvelope.h"
#include <assert.h>




template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const HDUniversalEnvelope &o) : HDMemoryModel(HDTypeEnvelope)
{
    tensor = NULL;
    list = NULL;
    decimal = NULL;
    integer = NULL;
    string = NULL;
    envelopeContent = HDTypeUnknown;
    envelopeContent = o.getObjectType();
    	    const HDUniversalEnvelope &mm = dynamic_cast<const HDUniversalEnvelope<T,C> &>(o);
    	    envelopeContent = mm.envelopeContent;
//    	    printf("********* Kopiuje envelope");
            switch(mm.envelopeContent)
            {
               case HDTypeDecimal:
                {
                    decimal = dynamic_cast <HDPrimitiveEnvelope<T> *>(mm.decimal->createWeakCopy());
                    break;
                }
                case HDTypeInteger:
                    integer = dynamic_cast <HDPrimitiveEnvelope<int> *>(mm.integer->createWeakCopy());
                    break;
                case HDTypeTensor:
                    tensor = dynamic_cast <HDTensor<T> *>(mm.tensor->createWeakCopy());
                    break;
                case HDTypeString:
                    string = dynamic_cast <HDStringEnvelope<T,C> *>(mm.string->createWeakCopy());
                    break;
                case HDTypeList:
                    list = dynamic_cast <HDList<T,C> *>(mm.list->createWeakCopy());
                    break;
                default:
                    printf("Zapetlenie\n");
                    printTrace(); exit(1);
                    break;
            }



}





template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const HDMemoryModel &o) : HDMemoryModel(HDTypeEnvelope)
{
    tensor = NULL;
    list = NULL;
    decimal = NULL;
    integer = NULL;
    string = NULL;
    envelopeContent = HDTypeUnknown;
    envelopeContent = o.getObjectType();
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Copy constructor called %d\n",envelopeContent);
    switch (o.objectType) {
        case HDTypeDecimal:
        {        
            HDMemoryModel *mm = o.createWeakCopy();
            decimal = dynamic_cast <HDPrimitiveEnvelope<T> *>(mm);
            break;
        }
        case HDTypeInteger:
        {
            HDMemoryModel *mm = o.createWeakCopy();
            integer = dynamic_cast <HDPrimitiveEnvelope<int> *>(mm);
            break;
        }
        case HDTypeTensor:
        {
            HDMemoryModel *mm = o.createWeakCopy();
            tensor = dynamic_cast <HDTensor<T> *>(mm);
            break;
        }
        case HDTypeString:
        {
            HDMemoryModel *mm = o.createWeakCopy();
            string = dynamic_cast <HDStringEnvelope<T,C> *>(mm);
            break;
        }
        case HDTypeList:
        {
            HDMemoryModel *mm = o.createWeakCopy();
            list = dynamic_cast <HDList<T,C> *>(mm);
            break;
        }
        case HDTypeEnvelope:
            {
    	    const HDUniversalEnvelope &mm = dynamic_cast<const HDUniversalEnvelope<T,C> &>(o);
    	    envelopeContent = mm.envelopeContent;
    	    //printf("********* Kopiuje envelope");
            switch(mm.envelopeContent)
            {
               case HDTypeDecimal:
                {
                    decimal = dynamic_cast <HDPrimitiveEnvelope<T> *>(mm.decimal->createWeakCopy());
                    break;
                }
                case HDTypeInteger:
                    integer = dynamic_cast <HDPrimitiveEnvelope<int> *>(mm.integer->createWeakCopy());
                    break;
                case HDTypeTensor:
                    tensor = dynamic_cast <HDTensor<T> *>(mm.tensor->createWeakCopy());
                    break;
                case HDTypeString:
                    string = dynamic_cast <HDStringEnvelope<T,C> *>(mm.string->createWeakCopy());
                    break;
                case HDTypeList:
                    list = dynamic_cast <HDList<T,C> *>(mm.list->createWeakCopy());
                    break;
                default:
                    printf("Zapetlenie\n");
                    printTrace(); exit(1);
                    break;
            }
            }
    	    break;
        default:
    	printf("Zapetlenie\n");
    	printTrace(); exit(1);

            break;
    }
    assert(decimal || integer || tensor || string || list);

  
}



template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope() : HDMemoryModel(HDTypeEnvelope)
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = NULL;
    list = NULL;
    decimal = NULL;
    integer = NULL;
    string = NULL;
    envelopeContent = HDTypeUnknown;
}

template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const HDTensor<T> &o) : HDMemoryModel(HDTypeEnvelope)
{
//      printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = new HDTensor<T>(o);
    list  = NULL;
    decimal = NULL;
    integer = NULL;
    string = NULL;
    envelopeContent = HDTypeTensor;
}
template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const HDList<T,C> &o) : HDMemoryModel(HDTypeEnvelope)
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = NULL;
    list  = new HDList<T,C>(o);
    decimal = NULL;
    integer = NULL;
    string = NULL;
    envelopeContent = HDTypeList;
}
template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const HDPrimitiveEnvelope<T> &o) : HDMemoryModel(HDTypeEnvelope)
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = NULL;
    list  = NULL;
    decimal = new HDPrimitiveEnvelope<T>(o);
    integer = NULL;
    string = NULL;
    envelopeContent = HDTypeDecimal;
}
template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const HDPrimitiveEnvelope<int> &o) : HDMemoryModel(HDTypeEnvelope)
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = NULL;
    list  = NULL;
    decimal = NULL;
    integer = new HDPrimitiveEnvelope<int> (o);
    string = NULL;
    envelopeContent = HDTypeInteger;
}

template<class T,class C>
HDMemoryModel *HDUniversalEnvelope<T,C>::createWeakCopy() const
{
//    printf("UUUUUUUUUUUU Creating weak copy\n");
    HDUniversalEnvelope<T,C> *tmpEnv = new HDUniversalEnvelope<T,C>();
    if(list) tmpEnv->list = dynamic_cast <HDList<T,C> *>(list->createWeakCopy());
    if(string) tmpEnv->string = dynamic_cast <HDStringEnvelope<T,C> *>(string->createWeakCopy());
    if(tensor) tmpEnv->tensor = dynamic_cast <HDTensor<T> *>(tensor->createWeakCopy());
    if(integer) tmpEnv->integer = dynamic_cast <HDPrimitiveEnvelope<int> *>(integer->createWeakCopy());
    if(decimal) tmpEnv->decimal = dynamic_cast <HDPrimitiveEnvelope<T> *>(decimal->createWeakCopy());
    tmpEnv->envelopeContent = envelopeContent;
    return tmpEnv;
}


template<class T,class C>
HDMemoryModel *HDUniversalEnvelope<T,C>::createStrongCopy() const
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Strong copy\n");
    HDUniversalEnvelope<T,C> *tmpEnv = new HDUniversalEnvelope<T,C>();
    
    if(list) tmpEnv->list = dynamic_cast <HDList<T,C> *>(list->createStrongCopy());
    if(string) tmpEnv->string = dynamic_cast <HDStringEnvelope<T,C> *>(string->createStrongCopy());
    if(tensor) tmpEnv->tensor = dynamic_cast <HDTensor<T> *>(tensor->createStrongCopy());
    if(integer) tmpEnv->integer = dynamic_cast <HDPrimitiveEnvelope<int> *>(integer->createStrongCopy());
    if(decimal) tmpEnv->decimal = dynamic_cast <HDPrimitiveEnvelope<T> *>(decimal->createStrongCopy());
    tmpEnv->envelopeContent = envelopeContent;
    return tmpEnv;
}


template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const T &o) : HDMemoryModel(HDTypeEnvelope)
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = NULL;
    list  = NULL;
    decimal = new HDPrimitiveEnvelope<T>(o,HDTypeDecimal);
    integer = NULL;
    string = NULL;
    envelopeContent = HDTypeDecimal;
}
template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const int &o) : HDMemoryModel(HDTypeEnvelope)
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = NULL;
    list  = NULL;
    decimal = NULL;
    integer = new HDPrimitiveEnvelope<int> (o,HDTypeInteger);
    string = NULL;
    envelopeContent = HDTypeInteger;
}


template<class T,class C>
HDUniversalEnvelope<T,C>::HDUniversalEnvelope(const HDStringEnvelope<T,C> &o) : HDMemoryModel(HDTypeEnvelope)
{
//    printf("UUUUUUUUUUUUUUUUUUUUUUUUU Constructor\n");
    tensor = NULL;
    list  = NULL;
    decimal = NULL;
    integer = NULL;
    string = new HDStringEnvelope<T,C>(o);
    envelopeContent = HDTypeString;
}



template<class T,class C>
HDUniversalEnvelope<T,C>::~HDUniversalEnvelope()
{
//    printf("UUUUUUUUUUUUUUUUU Destorying %p with tensor %p\n",this,tensor);

    if(tensor) delete tensor;
    if(list) delete list;
    if(decimal) delete decimal;
    if(integer) delete integer;
    if(string) delete string;
}




//template<class T,class C>
//HDUniversalEnvelope<T,C>::operator HDList<T,C>()
//{
//    if(envelopeContent == HDTypeList) {
//        return *list;
//    }
//    printf("Error: Trying to convert type %d to list\n",envelopeContent);
//    printTrace(); exit(1);
//}
//
//
//
//
//template<class T,class C>
//HDUniversalEnvelope<T,C>::operator T()
//{
//    if(envelopeContent == HDTypeInteger) {
//        return (T)**integer;
//    }
//    
//    if(envelopeContent == HDTypeDecimal) {
//        return **decimal;
//    }
//    printf("Error: Trying to convert type %d to double\n",envelopeContent);
//    printTrace(); exit(1);
//    
//}
//
//
//template<class T,class C>
//HDUniversalEnvelope<T,C>::operator HDStringEnvelope<T,C> ()
//{
//    if(envelopeContent == HDTypeString) {
//        return *string;
//    }
//    printf("Error: Trying to convert type %d to string\n",envelopeContent);
//    printTrace(); exit(1);
//    
//}
//template<class T,class C>
//HDUniversalEnvelope<T,C>::operator HDTensor<T> ()
//{
//    if(envelopeContent == HDTypeTensor) {
//        return *tensor;
//    }
//    printf("Error: Trying to convert type %d to tensor\n",envelopeContent);
//    printTrace(); exit(1);
//}
//

/* ******************************* ++++++++++++++++++++++++++++ ******************************* */

template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator+(const HDUniversalEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeString)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return HDUniversalEnvelope<T,C>(*string+*(t.string));
        }
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(*string+**(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(*string+**(t.integer));
        }
        
        printf("Error: Trying to convert type %d for string addition\n",t.envelopeContent);
        printTrace(); exit(1);
    }

    if(envelopeContent == HDTypeDecimal)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return HDUniversalEnvelope<T,C>(**decimal+*(t.string));
        }
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**decimal+**(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**decimal+**(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(**decimal+*(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for decimal addition\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeInteger)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return HDUniversalEnvelope<T,C>(**integer+*(t.string));
        }
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**integer+**(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**integer+**(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(  (**integer) + *(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for integer addition\n",t.envelopeContent);
        printTrace(); exit(1);
    }

    if(envelopeContent == HDTypeTensor)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(*tensor+**(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(*tensor+**(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(*tensor+*(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for tensor addition\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    printf("Error: Trying to add unaddable type %d\n",envelopeContent);
    printTrace(); exit(1);
}


template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator+(const HDTensor<T> &t) const
{
    
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal+t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer+t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor+t);
    }
    
    printf("Error: Trying to convert type %d for tensor addition\n",envelopeContent);
    printTrace(); exit(1);
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator+(const int &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal+t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer+t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor+t);
    }
    
    if(envelopeContent == HDTypeString)
    {
        return HDUniversalEnvelope<T,C>(*string+t);
    }

    
    printf("Error: Trying to convert type %d for integer addition\n",envelopeContent);
    printTrace(); exit(1);
    
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator+(const T &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal+t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer+t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor+t);
    }
    
    if(envelopeContent == HDTypeString)
    {
        return HDUniversalEnvelope<T,C>(*string+t);
    }
    
    
    printf("Error: Trying to convert type %d for decimal addition\n",envelopeContent);
    printTrace(); exit(1);
    
    
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator+(const HDStringEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal+t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer+t);
    }
    
    if(envelopeContent == HDTypeString)
    {
        return HDUniversalEnvelope<T,C>(*string+t);
    }
    
    
    printf("Error: Trying to convert type %d for string addition\n",envelopeContent);
    printTrace(); exit(1);
    
    
}

/* ******************** ----------------- ********************************/

template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator-(const HDUniversalEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeString)
    {
        printf("Error: Trying to convert type %d for string subtraction\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeDecimal)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**decimal-**(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**decimal-**(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(**decimal-*(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for decimal subtraction\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeInteger)
    {
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**integer-**(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**integer-**(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(**integer-*(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for integer subtraction\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(*tensor-**(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(*tensor-**(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(*tensor-*(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for tensor subtraction\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    printf("Error: Trying to add unaddable type %d\n",envelopeContent);
    printTrace(); exit(1);
}


template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator-(const HDTensor<T> &t) const
{
    
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal-t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer-t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor-t);
    }
    
    printf("Error: Trying to convert type %d for tensor subtraction\n",envelopeContent);
    printTrace(); exit(1);
}

template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator-(const int &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal-t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer-t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor-t);
    }
    
    
    printf("Error: Trying to convert type %d for integer subtraction\n",envelopeContent);
    printTrace(); exit(1);
    
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator-(const T &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal-t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer-t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor-t);
    }
    
    printf("Error: Trying to convert type %d for decimal subtraction\n",envelopeContent);
    printTrace(); exit(1);
    
    
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator-(const HDStringEnvelope<T,C> &t) const
{
    printf("Error: Trying to convert type %d for string subtraction\n",envelopeContent);
    printTrace(); exit(1);
}



/* ******************* /////// ********************* */

template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator/(const HDUniversalEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeString)
    {
        
        printf("Error: Trying to convert type %d for string div\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeDecimal)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**decimal / **(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**decimal /  **(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(**decimal / *(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for decimal div\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeInteger)
    {
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**integer / **(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**integer / **(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(**integer / *(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for integer div\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(*tensor / **(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(*tensor / **(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(*tensor / *(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for tensor div\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    printf("Error: Trying to add unaddable type %d\n",envelopeContent);
    printTrace(); exit(1);
}


template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator/(const HDTensor<T> &t) const
{
    
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal / t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer / t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor / t);
    }
    
    printf("Error: Trying to convert type %d for tensor div\n",envelopeContent);
    printTrace(); exit(1);
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator/(const int &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal / t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer / t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor / t);
    }
    
    
    
    printf("Error: Trying to convert type %d for integer div\n",envelopeContent);
    printTrace(); exit(1);
    
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator/(const T &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal  / t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer / t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor / t);
    }
    
    
    
    printf("Error: Trying to convert type %d for decimal div\n",envelopeContent);
    printTrace(); exit(1);
    
    
}





/* ******************** ************************ ***************** */
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator*(const HDUniversalEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeString)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(*string * (int) **(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(*string * **(t.integer));
        }
        
        printf("Error: Trying to convert type %d for string mul\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeDecimal)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return HDUniversalEnvelope<T,C>((int)(**decimal) * *(t.string));
        }
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**decimal * **(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**decimal *  **(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(**decimal * *(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for decimal mul\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeInteger)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return HDUniversalEnvelope<T,C>(**integer * *(t.string));
        }
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(**integer * **(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(**integer * **(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(**integer * *(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for integer mul\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return HDUniversalEnvelope<T,C>(*tensor * **(t.decimal));
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return HDUniversalEnvelope<T,C>(*tensor * **(t.integer));
        }
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return HDUniversalEnvelope<T,C>(*tensor * *(t.tensor));
        }
        
        printf("Error: Trying to convert type %d for tensor mul\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    printf("Error: Trying to add unaddable type %d\n",envelopeContent);
    printTrace(); exit(1);
}


template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator*(const HDTensor<T> &t) const
{
    
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal * t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer * t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor * t);
    }
    
    printf("Error: Trying to convert type %d for tensor mul\n",envelopeContent);
    printTrace(); exit(1);
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator*(const int &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal * t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer * t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor * t);
    }
    
    if(envelopeContent == HDTypeString)
    {
        return HDUniversalEnvelope<T,C>(*string * t);
    }
    
    
    printf("Error: Trying to convert type %d for integer mul\n",envelopeContent);
    printTrace(); exit(1);
    
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator*(const T &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal  * t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer * t);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        return HDUniversalEnvelope<T,C>(*tensor * t);
    }
    
    if(envelopeContent == HDTypeString)
    {
        return HDUniversalEnvelope<T,C>(*string * t);
    }
    
    
    printf("Error: Trying to convert type %d for decimal addition\n",envelopeContent);
    printTrace(); exit(1);
    
    
}
template<class T,class C>
HDUniversalEnvelope<T,C> HDUniversalEnvelope<T,C>::operator*(const HDStringEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(**decimal * t);
    }
    if(envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(**integer+t);
    }
    
    if(envelopeContent == HDTypeString)
    {
        return HDUniversalEnvelope<T,C>(*string+t);
    }
    
    
    printf("Error: Trying to convert type %d for string addition\n",envelopeContent);
    printTrace(); exit(1);
}



#pragma mark - Friends

template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op)
{
    return op+t;
}



template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const int &t,const HDUniversalEnvelope<T,C> &op)
{
    return op+t;
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const T &t,const HDUniversalEnvelope<T,C> &op)
{
    return op+t;
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator+(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op)
{
    return op+t;
}

template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op)
{
    return ((-1)*t +op)*(-1);
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const int &t,const HDUniversalEnvelope<T,C> &op)
{
    return ((-1)*t +op)*(-1);
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const T &t,const HDUniversalEnvelope<T,C> &op)
{
    return ((-1)*t +op)*(-1);
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator-(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op)
{
    printf("Error: Unable to subtract from string!\n");
    printTrace(); exit(1);
}

template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op)
{
    return op*t;
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const int &t,const HDUniversalEnvelope<T,C> &op)
{
    return op*t;
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const T &t,const HDUniversalEnvelope<T,C> &op)
{
    return op*t;
}

template <class T,class C>
HDUniversalEnvelope<T,C> operator*(const HDStringEnvelope<T,C> &t,const HDUniversalEnvelope<T,C> &op)
{
    return op*t;
}

template <class T,class C>
HDUniversalEnvelope<T,C> operator/(const HDTensor<T> &t,const HDUniversalEnvelope<T,C> &op) 
{
    return (1.0/op)*t;
}
template <class T,class C>
HDUniversalEnvelope<T,C> operator/(const int &t,const HDUniversalEnvelope<T,C> &op)
{
    if(op.envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(t / **(op.decimal) );
    }
    if(op.envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(t / **(op.integer) );
    }
    
    
    
    printf("Error: Trying to convert type %d for integer div\n",op.envelopeContent);
    printTrace(); exit(1);

}
template <class T,class C>
HDUniversalEnvelope<T,C> operator/(const T &t,const HDUniversalEnvelope<T,C> &op)
{
    if(op.envelopeContent == HDTypeDecimal)
    {
        return HDUniversalEnvelope<T,C>(t / **(op.decimal));
    }
    if(op.envelopeContent == HDTypeInteger)
    {
        return HDUniversalEnvelope<T,C>(t / **(op.integer));
    }
    
    
    printf("Error: Trying to convert type %d for decimal div\n",op.envelopeContent);
    printTrace(); exit(1);
    
}

#pragma mark - Casts

template <class T,class C>
int HDUniversalEnvelope<T,C>::explicitIntCast(const HDUniversalEnvelope<T,C> &op)
{
    if(op.envelopeContent == HDTypeInteger)
    {
        return *(*op.integer);
    }
    if(op.envelopeContent == HDTypeDecimal)
    {
        return (int) *(*op.decimal);
    }
    if(op.envelopeContent == HDTypeString)
    {
        return atoi( (*(op.string)).c_str().c_str());
    }
    printf("Error: Trying to convert type %d to int\n",op.envelopeContent);
    printTrace(); exit(1);
}
template <class T,class C>
HDTensor<T> HDUniversalEnvelope<T,C>::explicitHDTensorCast(const HDUniversalEnvelope<T,C> &op)
{
    if(op.envelopeContent == HDTypeTensor)
    {
        return *(op.tensor);
    }
    printf("Error: Trying to convert type %d to tensor\n",op.envelopeContent);
    printTrace(); exit(1);
}

template <class T,class C>
int HDUniversalEnvelope<T,C>::operator==(const HDUniversalEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeString)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return *string == *(t.string);
        }
        
        if(t.envelopeContent == HDTypeInteger)
        {
            return *string == **(t.integer);
        }
        
        printf("Error: Trying to convert type %d for string comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeDecimal)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return **decimal == **(t.decimal);
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return **decimal == **(t.integer);
        }
        
        
        printf("Error: Trying to convert type %d for decimal comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeInteger)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return **integer == *(t.string);
        }
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return **integer == **(t.decimal);
        }
        if(t.envelopeContent == HDTypeInteger)
        {
//            std::cout << "Porownujemy: "<<**integer << " - " << **(t.integer)<<std::endl;
            return **integer == **(t.integer);
        }
        
        
        printf("Error: Trying to convert type %d for integer comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return  *tensor == *(t.tensor);
        }
        
        printf("Error: Trying to convert type %d for tensor comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    printf("Error: Trying to compare uncomparable type %d\n",envelopeContent);
    printTrace(); exit(1);
}


template <class T,class C>
int HDUniversalEnvelope<T,C>::operator>(const HDUniversalEnvelope<T,C> &t) const
{
    if(envelopeContent == HDTypeString)
    {
        if(t.envelopeContent == HDTypeString)
        {
            return *string > *(t.string);
        }
        
        
        printf("Error: Trying to convert type %d for string comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeDecimal)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return **decimal > **(t.decimal);
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return **decimal > **(t.integer);
        }
        
        
        printf("Error: Trying to convert type %d for decimal comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeInteger)
    {
        
        if(t.envelopeContent == HDTypeDecimal)
        {
            return **integer > **(t.decimal);
        }
        if(t.envelopeContent == HDTypeInteger)
        {
            return **integer > **(t.integer);
        }
        
        
        printf("Error: Trying to convert type %d for integer comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    
    if(envelopeContent == HDTypeTensor)
    {
        
        if(t.envelopeContent == HDTypeTensor)
        {
            return  *tensor > *(t.tensor);
        }
        
        printf("Error: Trying to convert type %d for tensor comparison\n",t.envelopeContent);
        printTrace(); exit(1);
    }
    printf("Error: Trying to compare uncomparable type %d\n",envelopeContent);
    printTrace(); exit(1);
}





template <class T,class C>
int HDUniversalEnvelope<T,C>::operator<(const HDUniversalEnvelope<T,C> &t) const
{
    return !( (*this) > t || (*this) == t);
}


template <class T,class C>
int HDUniversalEnvelope<T,C>::operator!=(const HDUniversalEnvelope<T,C> &t) const
{
    return !(*this == t);
}

template <class T,class C>
int HDUniversalEnvelope<T,C>::operator<=(const HDUniversalEnvelope<T,C> &t) const
{
    return (*this) < t || (*this) == t;
}

template <class T,class C>
int HDUniversalEnvelope<T,C>::operator>=(const HDUniversalEnvelope<T,C> &t) const
{
    return (*this) > t || (*this) == t;
}









template <class T,class C>
HDStringEnvelope<T,C> HDUniversalEnvelope<T,C>::explicitHDStringEnvelopeCast(const HDUniversalEnvelope<T,C> &op) 
{
    if(op.envelopeContent == HDTypeString)
    {
        return *(op.string);
    }
    if(op.envelopeContent == HDTypeInteger)
    {
        return  HDStringEnvelope<T,C>::stringFromInt(*(*(op.integer)));
    }
    
    if(op.envelopeContent == HDTypeDecimal)
    {
        return HDStringEnvelope<T,C>::stringFromInt((int)*(*(op.decimal)));
    }
    
    printf("Error: Trying to convert type %d to string\n",op.envelopeContent);
    printTrace(); exit(1);

}
template <class T,class C>
T HDUniversalEnvelope<T,C>::explicitDecimalCast(const HDUniversalEnvelope<T,C> &op)
{
    if(op.envelopeContent == HDTypeInteger)
    {
        return (T) *(*op.integer);
    }
    if(op.envelopeContent == HDTypeDecimal)
    {
        return *(*op.decimal);
    }
    if(op.envelopeContent == HDTypeString)
    {
        return atof( (*(op.string)).c_str().c_str());
    }
    printf("Error: Trying to convert type %d to decimal\n",op.envelopeContent);
    printTrace(); exit(1);
}
template <class T,class C>
HDList<T,C> HDUniversalEnvelope<T,C>::explicitHDListCast(const HDUniversalEnvelope<T,C> &op) 
{
    if(op.envelopeContent == HDTypeList)
    {
        return *(op.list);
    }
    printf("Error: Trying to convert type %d to list\n",op.envelopeContent);
    printTrace(); exit(1);
}





