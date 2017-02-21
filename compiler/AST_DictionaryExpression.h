//
//  AST_DictionaryExpression.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_DictionaryExpression__
#define __Hodor__AST_DictionaryExpression__

#include <iostream>
#include "AST_Expression.h"


class AST_DictionaryExpression : public AST_Expression
{
public:
    AST_DictionaryExpression(AST_Expression *first,AST_Expression *second);
    AST_Expression *first;
    AST_Expression *second;
    
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_DictionaryExpression();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */
};


#endif /* defined(__Hodor__AST_DictionaryExpression__) */
