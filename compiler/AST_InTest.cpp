//
//  AST_InTest.cpp
//  Hodor
//
//  Created by Test on 23.11.2013.
//
//

#include "AST_InTest.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *var = NULL;

AST_InTest::~AST_InTest()
{
    delete left;
    delete right;
}

AST_InTest::AST_InTest(AST_Expression *left,AST_Expression *right) : AST_Expression(left)
{
    this->left = left;
    this->right = right;
    children.push_back(left);
    children.push_back(right);
    
    typ = TYPE_IN_KEYWORD;
}

std::string AST_InTest::constructOutputString(HDExecutionContext *context)
{
    return std::string();
}


AST_VariableDeclaration *AST_InTest::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HD internal inTest logic result",TYPE_DEF_INTEGER);
    return var;
}
