//
//  AST_Numeric.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Numeric.h"

AST_Numeric::AST_Numeric(HDParseSymbol *s) : AST_Expression(s)
{
}


AST_Numeric::~AST_Numeric()
{
}

AST_VariableDeclaration *AST_Numeric::computeFinalType(HDExecutionContext *context)
{
    printf("Not implemented");
    exit(1);
}