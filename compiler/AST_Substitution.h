//
//  AST_Substitution.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Substitution__
#define __Hodor__AST_Substitution__

#include <iostream>
#include "AST_Expression.h"
#include "AST_Node.h"
#include "AST_ArrayAccess.h"

class AST_Substitution :public  AST_Expression
{
public:
    AST_Identifier *left;
    AST_Expression *right;
    AST_ArrayAccess *leftAccess;
  
    AST_Substitution(AST_Identifier *left,AST_Expression *right,HDParseSymbol *s);
    AST_Substitution(AST_ArrayAccess *left,AST_Expression *right,HDParseSymbol *s);
    
    
    std::string computeOperatorMark(HDExecutionContext *context);
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */

    virtual ~AST_Substitution();
};


#endif /* defined(__Hodor__AST_Substitution__) */
