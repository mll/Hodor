//
//  AST_UnaryOperator.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_UnaryOperator.h"
#include "AST_VariableDeclaration.h"




AST_UnaryOperator::AST_UnaryOperator(AST_Expression *r, HDParseSymbol*s) : AST_Expression(s)
{
    right=r;
    this->op=s->tokenType;
    children.push_back(r);
    typ = TYPE_UNARY_OPERATOR;
}

std::string AST_UnaryOperator::constructOutputString(HDExecutionContext *context)
{
   // printf("Operator unarny dostal operacje %d dla ponizszego wyrazenia\n",op);


    AST_VariableDeclaration *var = right->computeFinalType(context);
    if(var->type->tokenType != TYPE_DEF_DECIMAL && var->type->tokenType != TYPE_DEF_INTEGER)
    {
        printf("%d-%d Error - unsupported type for unary operator %d.",compilerContext.lineNumber,compilerContext.linePosition,op);
        exit(1);
    }
    
    
    std::string out = "(";
    switch(op)
    {
        case TYPE_NOT:
            if(var->sizes)
            {
                printf("%d-%d Error - negation unary operator does not support arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            out+="! ";
            break;
        case TYPE_MINUS:

            out+="(-1)*";
            break;
    }
    out+= right->constructOutputString(context);
    out+=")";
    return out;
}


AST_VariableDeclaration *AST_UnaryOperator::computeFinalType(HDExecutionContext *context) /* returned construct is borrowed for the caller */
{
    return right->computeFinalType(context);
}


AST_UnaryOperator::~AST_UnaryOperator()
{
    delete right;
}