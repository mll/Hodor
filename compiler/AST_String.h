//
//  AST_String.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_String__
#define __Hodor__AST_String__

#include <iostream>
#include "AST_Expression.h"


class AST_String : public AST_Expression
{
public:
    std::string *value;
    
    virtual ~AST_String();
    AST_String(HDParseSymbol *s);
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_String__) */
