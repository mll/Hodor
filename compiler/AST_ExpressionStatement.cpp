//
//  AST_ExpressionStatement.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_ExpressionStatement.h"




AST_ExpressionStatement::AST_ExpressionStatement(AST_Expression *expression) : AST_Statement(expression)
{
    this->expression = expression;
    children.push_back(expression);
    typ = TYPE_EXPR_INSTR;
}

std::string AST_ExpressionStatement::constructOutputString(HDExecutionContext *context)
{
    //printf("Mamy do czynienia z expressionm bedacym instrukcja\n");
    return expression->constructOutputString(context);
}




AST_ExpressionStatement::~AST_ExpressionStatement()
{
    delete expression;
}