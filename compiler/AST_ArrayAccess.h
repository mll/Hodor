//
//  AST_ArrayAccess.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_ArrayAccess__
#define __Hodor__AST_ArrayAccess__

#include <iostream>
#include "AST_Expression.h"
#include "AST_ArrayAccessExpression.h"

class AST_ArrayAccess : public AST_Expression
{
private:
    AST_Expression *where;
    vector<AST_ArrayAccessExpression *> *indices;

public:
    AST_ArrayAccess(AST_Expression *where,vector<AST_ArrayAccessExpression *> *indices);
    
    virtual std::string constructOutputString(HDExecutionContext *context);
    AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
    virtual ~AST_ArrayAccess();
};


#endif /* defined(__Hodor__AST_ArrayAccess__) */
