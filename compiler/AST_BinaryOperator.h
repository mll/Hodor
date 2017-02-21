//
//  AST_BinaryOperator.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_BinaryOperator__
#define __Hodor__AST_BinaryOperator__

#include <iostream>
#include "AST_Expression.h"

class AST_BinaryOperator : public AST_Expression
{
private:
    bool allSubnodesPlus();
    bool seekForNameAndExpel(std::string name);
public:
    AST_Expression *left;
    AST_Expression *right;
    int op;
    
    AST_BinaryOperator(AST_Expression *l, AST_Expression *r, HDParseSymbol *s);
    virtual std::string constructOutputString(HDExecutionContext *context);

    bool findPlusSubstitution(std::string name,HDExecutionContext *context);
  
  
    virtual ~AST_BinaryOperator();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */

};


#endif /* defined(__Hodor__AST_BinaryOperator__) */
