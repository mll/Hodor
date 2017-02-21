//
//  AST_Dictionary.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Dictionary__
#define __Hodor__AST_Dictionary__

#include <iostream>
#include <vector>
#include "AST_Expression.h"

class AST_Dictionary : public AST_Expression
{
public:
    AST_Dictionary(std::vector<AST_Expression*> *args);
    std::vector<AST_Expression*> *args;
    
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_Dictionary();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_Dictionary__) */
