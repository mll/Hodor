//
//  AST_WhileStatement.cpp
//  Hodor
//
//  Created by Marek Lipert on 18.04.2014.
//
//

#include "AST_WhileStatement.h"


AST_WhileStatement::AST_WhileStatement(AST_Expression *condition, AST_Block *execBlock) : AST_Statement(&(condition->compilerContext))
{
    this->execBlock = execBlock;
    this->condition = condition;
    
    if(execBlock) children.push_back(execBlock);
    if(condition) children.push_back(condition);
    
    typ = TYPE_WHILE_KEYWORD;
}

AST_WhileStatement::~AST_WhileStatement()
{
    if(execBlock) delete execBlock;
    if(condition) delete condition;
}



HDExecutionContext *AST_WhileStatement::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */
{
    switch (passNo) {
        case 2:
            executionContext = *context;
            break;
        case 5:
            
            break;
        default:
            break;
    }
    return &executionContext;
}







std::string AST_WhileStatement::constructOutputString(HDExecutionContext *context)
{
    return std::string("while(")+condition->constructOutputString(context)+")"+execBlock->constructOutputString(&executionContext);
}
