//
//  AST_ForStatement.h
//  Hodor
//
//  Created by Test on 23.11.2013.
//
//

#ifndef __Hodor__AST_ForStatement__
#define __Hodor__AST_ForStatement__

#include <iostream>
#include "AST_Statement.h"
#include "AST_VariableDeclaration.h"
#include "AST_Block.h"

class AST_ForStatement :public  AST_Statement
{
public:
    AST_VariableDeclaration *counter;
    AST_VariableDeclaration *secondCounter;
    AST_Expression *set;
    AST_Block *execBlock;
    bool hasOptimizedRange;
    
    AST_ForStatement(AST_VariableDeclaration *var, AST_Expression *set, AST_Block *execBlock,AST_VariableDeclaration *secondVar = NULL);

    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output); /* returns context for deeper progress */
    
    virtual ~AST_ForStatement();
    virtual std::string constructOutputString(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_ForStatement__) */
