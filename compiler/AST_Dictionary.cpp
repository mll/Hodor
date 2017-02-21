//
//  AST_Dictionary.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Dictionary.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *var = NULL;

AST_Dictionary::AST_Dictionary(std::vector<AST_Expression*> *args)
{
    this->args = args;
    typ = TYPE_DICTIONARY;
    for(std::vector<AST_Expression *>::iterator i = args->begin();i!=args->end();i++) children.push_back(*i);
}


std::string AST_Dictionary::constructOutputString(HDExecutionContext *context)
{
  //  printf("Slownik zawiera ponizsze elementy\n");
    for(std::vector<AST_Expression*>::iterator i = args->begin();i!=args->end();i++) (*i)->constructOutputString(context);
    return std::string();
}


AST_VariableDeclaration *AST_Dictionary::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HD internal dictionary",TYPE_DEF_DICTIONARY);
    return var;
}

AST_Dictionary::~AST_Dictionary()
{
    delete args;
}