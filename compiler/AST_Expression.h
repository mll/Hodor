//
//  AST_Expression.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Expression__
#define __Hodor__AST_Expression__

#include <iostream>
#include "AST_Node.h"

class AST_VariableDeclaration;
class HDCompilerContext;

class AST_Expression : public AST_Node
{
public:

    AST_Expression(HDParseSymbol *s);
    AST_Expression(AST_Node *s);
    AST_Expression();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */
    
    virtual ~AST_Expression();
};


#endif /* defined(__Hodor__AST_Expression__) */
