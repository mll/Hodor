//
//  AST_ArrayAccessExpression.h
//  Hodor
//
//  Created by Test on 30.11.2013.
//
//

#ifndef __Hodor__AST_ArrayAccessExpression__
#define __Hodor__AST_ArrayAccessExpression__

#include <iostream>
#include "AST_Expression.h"


class AST_ArrayAccessExpression : public AST_Expression
{
public:
    AST_Expression *left;
    AST_Expression *right;
    
    AST_ArrayAccessExpression(HDParseSymbol *symbol, AST_Expression *left, AST_Expression *right);
  
    virtual std::string constructOutputString(HDExecutionContext *context);
    AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
    virtual ~AST_ArrayAccessExpression();
};


#endif /* defined(__Hodor__AST_ArrayAccessExpression__) */
