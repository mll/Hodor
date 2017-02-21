//
//  AST_IfStatement.h
//  Hodor
//
//  Created by Test on 23.11.2013.
//
//

#ifndef __Hodor__AST_IfStatement__
#define __Hodor__AST_IfStatement__

#include <iostream>
#include "AST_Statement.h"
#include "AST_Expression.h"
#include "AST_Block.h"

class AST_IfStatement :public  AST_Statement
{
public:
    AST_Expression *condition;
    AST_Block *yesBlock;
    AST_Block *noBlock;
    
    AST_IfStatement(AST_Expression *condition,AST_Block *yesBlock, AST_Block *noBlock);
    virtual ~AST_IfStatement();
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output); /* returns context for */
};


#endif /* defined(__Hodor__AST_IfStatement__) */
