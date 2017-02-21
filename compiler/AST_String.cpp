//
//  AST_String.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_String.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *var = NULL;


AST_String::AST_String(HDParseSymbol *s) : AST_Expression(s)
{
    this->value = new std::string(*(s->value));
    typ = TYPE_STRING;
}

std::string AST_String::constructOutputString(HDExecutionContext *context)
{
   // printf("Mamy do czynienia z ciagiem znakow:'%s'\n",value->c_str());
    std::string out = "";
    out+="HDStringEnvelope<"+context->floatingPointTypeName+",char>(\"";
    out+=*value;
    out+="\")";
    return out;
}

AST_VariableDeclaration *AST_String::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HD internal string",TYPE_DEF_STRING);
    return var;
}


AST_String::~AST_String()
{
    delete value;
}