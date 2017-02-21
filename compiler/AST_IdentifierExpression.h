//
//  AST_IdentifierExpression.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_IdentifierExpression__
#define __Hodor__AST_IdentifierExpression__

#include <iostream>
#include "AST_Expression.h"
#include "AST_Identifier.h"

class AST_IdentifierExpression : public AST_Expression
{
public:
    AST_IdentifierExpression(AST_Identifier *ident);
    AST_Identifier *ident;
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_IdentifierExpression();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */
};


#endif /* defined(__Hodor__AST_IdentifierExpression__) */
