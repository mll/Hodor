//
//  AST_IdentifierExpression.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_IdentifierExpression.h"
#include "AST_VariableDeclaration.h"

AST_IdentifierExpression::AST_IdentifierExpression(AST_Identifier *ident) : AST_Expression(ident)
{
    this->ident = ident;
    typ = TYPE_EXPR_IDENT;
    children.push_back(ident);
}

std::string AST_IdentifierExpression::constructOutputString(HDExecutionContext *context)
{
    std::string out = "";
    AST_VariableDeclaration *var = computeFinalType(context);
    
    if(var->locus == AST_VariableDeclarationLocusForLoop)
    {
        out += "(*"+ident->constructOutputString(context)+")";
    }
    else
    out+= ident->constructOutputString(context);
    
//        printf("Mamy do czynienia z expressionm bedacym identyfikatorem: '%s'\n",out.c_str());
    return out;
}




AST_IdentifierExpression::~AST_IdentifierExpression()
{
    delete ident;
}

AST_VariableDeclaration *AST_IdentifierExpression::computeFinalType(HDExecutionContext *context)
{
    return ident->computeFinalType(context);
}