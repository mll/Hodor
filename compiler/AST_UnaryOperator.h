//
//  AST_UnaryOperator.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_UnaryOperator__
#define __Hodor__AST_UnaryOperator__

#include <iostream>
#include "AST_Expression.h"

class AST_UnaryOperator : public AST_Expression
{
public:
    int op;
    AST_Expression *right;
    
    AST_UnaryOperator(AST_Expression *r,HDParseSymbol *s );
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_UnaryOperator();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */

};


#endif /* defined(__Hodor__AST_UnaryOperator__) */
