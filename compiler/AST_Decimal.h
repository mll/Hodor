//
//  AST_Decimal.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Decimal__
#define __Hodor__AST_Decimal__

#include <iostream>
#include "AST_Numeric.h"

class AST_Decimal : public AST_Numeric
{
public:
    double value;
    AST_Decimal(HDParseSymbol *s);

    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_Decimal();
};


#endif /* defined(__Hodor__AST_Decimal__) */
