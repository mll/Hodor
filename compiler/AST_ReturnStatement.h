//
//  AST_ReturnStatement.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_ReturnStatement__
#define __Hodor__AST_ReturnStatement__

#include <iostream>
#include "AST_Statement.h"
#include "AST_Expression.h"

class AST_ReturnStatement : public AST_Statement
{
public:
    AST_Expression *expression;
    AST_ReturnStatement(HDParseSymbol *s,AST_Expression *w);
    
    virtual ~AST_ReturnStatement();
    virtual std::string constructOutputString(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_ReturnStatement__) */
