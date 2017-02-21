//
//  AST_Numeric.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Numeric__
#define __Hodor__AST_Numeric__

#include <iostream>
#include "AST_Expression.h"

class AST_Numeric : public AST_Expression
{
public:
    AST_Numeric(HDParseSymbol *s);
    virtual ~AST_Numeric();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_Numeric__) */
