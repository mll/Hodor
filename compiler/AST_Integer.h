//
//  AST_Integer.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Integer__
#define __Hodor__AST_Integer__

#include <iostream>
#include "AST_Numeric.h"

class AST_Integer : public AST_Numeric
{
public:
    int value;

    AST_Integer(HDParseSymbol *s);
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_Integer();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_Integer__) */
