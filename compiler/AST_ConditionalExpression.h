//
//  AST_ConditionalExpression.h
//  Hodor
//
//  Created by Test on 26.12.2013.
//
//

#ifndef __Hodor__AST_ConditionalExpression__
#define __Hodor__AST_ConditionalExpression__

#include <iostream>
#include "AST_Expression.h"

class AST_ConditionalExpression : public AST_Expression
{
public:
    AST_Expression *yesExpression;
    AST_Expression *condition;
    AST_Expression *noExpression;
    
    AST_ConditionalExpression(AST_Expression *condition,AST_Expression *yesExpression,AST_Expression *noExpression);

    
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_ConditionalExpression();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);

    
    
    
};


#endif /* defined(__Hodor__AST_ConditionalExpression__) */
