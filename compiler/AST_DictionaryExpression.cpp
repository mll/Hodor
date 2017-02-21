//
//  AST_DictionaryExpression.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_DictionaryExpression.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *var = NULL;


std::string AST_DictionaryExpression::constructOutputString(HDExecutionContext *context)
{
   // printf("expression slownikowe zawiera ponizsze elementy:");
   // printf("first : ");
    first->constructOutputString(context);
   // printf("second : ");
    second->constructOutputString(context);
    return std::string();
}



AST_DictionaryExpression::AST_DictionaryExpression(AST_Expression *first,AST_Expression *second)
{
    this->first = first;
    this->second = second;
    children.push_back(first);
    children.push_back(second);
    typ = TYPE_DICT_EXPR;
}



AST_DictionaryExpression::~AST_DictionaryExpression()
{
    delete first;
    delete second;
}

AST_VariableDeclaration *AST_DictionaryExpression::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HD internal dictionary",TYPE_DEF_DICTIONARY);
    return var;
}