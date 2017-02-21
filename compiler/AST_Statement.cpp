//
//  AST_Statement.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Statement.h"
AST_Statement::AST_Statement(HDParseSymbol *s) : AST_Node(s)
{
}

AST_Statement::AST_Statement(AST_Node *s) : AST_Node(s)
{
}


AST_Statement::~AST_Statement()
{
}
