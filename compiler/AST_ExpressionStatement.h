//
//  AST_ExpressionStatement.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_ExpressionStatement__
#define __Hodor__AST_ExpressionStatement__

#include <iostream>
#include "AST_Statement.h"
#include "AST_Expression.h"

class AST_ExpressionStatement : public AST_Statement
{
public:
    AST_ExpressionStatement(AST_Expression *expression);
    AST_Expression *expression;
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_ExpressionStatement();
};


#endif /* defined(__Hodor__AST_ExpressionStatement__) */
