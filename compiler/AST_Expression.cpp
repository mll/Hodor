//
//  AST_Expression.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Expression.h"
#include "AST_VariableDeclaration.h"

AST_Expression::AST_Expression(HDParseSymbol *s) : AST_Node(s)
{

}

AST_Expression::AST_Expression(AST_Node *s) : AST_Node(s)
{

}

AST_Expression::AST_Expression()
{

}

AST_Expression::~AST_Expression()
{

}

AST_VariableDeclaration *AST_Expression::computeFinalType(HDExecutionContext *context)
{
    printf("Not implemented");
    exit(1);
}