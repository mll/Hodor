//
//  AST_IfStatement.cpp
//  Hodor
//
//  Created by Test on 23.11.2013.
//
//

#include "AST_IfStatement.h"
#include "AST_VariableDeclaration.h"

AST_IfStatement::AST_IfStatement(AST_Expression *condition,AST_Block *yesBlock, AST_Block *noBlock) : AST_Statement(&(condition->compilerContext))
{
    this->condition = condition;
    this->yesBlock = yesBlock;
    this->noBlock = noBlock;
    if(yesBlock) children.push_back(yesBlock);
    if(noBlock) children.push_back(noBlock);
    if(condition) children.push_back(condition);
    typ = TYPE_IF_KEYWORD;
}

AST_IfStatement::~AST_IfStatement()
{
    if(yesBlock) delete yesBlock;
    if(noBlock) delete noBlock;
    if(condition) delete condition;
}


HDExecutionContext *AST_IfStatement::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */
{
    switch (passNo) {
        case 2:
            executionContext = *context;
            break;
        case 3:
            
        default:
            break;
    }
    return &executionContext;
}





std::string AST_IfStatement::constructOutputString(HDExecutionContext *context)
{
    AST_VariableDeclaration *type = condition->computeFinalType(context);
    if(type->sizes) {
        printf("%d-%d Error - array cannot be condition in if statement.",compilerContext.lineNumber,compilerContext.linePosition);
        exit(1);
    }
    
    if(type->type->tokenType != TYPE_DEF_DECIMAL && type->type->tokenType != TYPE_DEF_INTEGER)
    {
        printf("%d-%d Error - only integers/decimals can be a condition in if statement.(%d)\n",compilerContext.lineNumber,compilerContext.linePosition,type->type->tokenType);
        exit(1);
    }
    std::string retVal = "";
    retVal+="if("+condition->constructOutputString(context)+")\n"+yesBlock->constructOutputString(context);
    if(noBlock) retVal+="else\n"+noBlock->constructOutputString(context);
    
    return retVal;
}
