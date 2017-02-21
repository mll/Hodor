//
//  AST_WhileStatement.h
//  Hodor
//
//  Created by Marek Lipert on 18.04.2014.
//
//

#ifndef __Hodor__AST_WhileStatement__
#define __Hodor__AST_WhileStatement__

#include <iostream>



#include <iostream>
#include "AST_Statement.h"
#include "AST_VariableDeclaration.h"
#include "AST_Block.h"

class AST_WhileStatement :public  AST_Statement
{
public:
    AST_Expression *condition;
    AST_Block *execBlock;
    
    AST_WhileStatement(AST_Expression *condition, AST_Block *execBlock);
    
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output); /* returns context for deeper progress */
    
    virtual ~AST_WhileStatement();
    virtual std::string constructOutputString(HDExecutionContext *context);
};

#endif /* defined(__Hodor__AST_WhileStatement__) */
