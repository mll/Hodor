//
//  AST_ArrayAccessExpression.cpp
//  Hodor
//
//  Created by Test on 30.11.2013.
//
//

#include "AST_ArrayAccessExpression.h"
#include "AST_VariableDeclaration.h"
#include "AST_IdentifierExpression.h"

static AST_VariableDeclaration *var = NULL;


AST_ArrayAccessExpression::AST_ArrayAccessExpression(HDParseSymbol *symbol, AST_Expression *left, AST_Expression *right) : AST_Expression(symbol)
{
    this->left = left;
    this->right = right;
    if(left) children.push_back(left);
    if(right) children.push_back(right);
   // printf("Smart type? %d",left->typ);
    if(left && left->typ == TYPE_EXPR_IDENT)
    {
        /* enable smart defines */
        AST_IdentifierExpression *smart = dynamic_cast<AST_IdentifierExpression *>(left);
        smart->ident->smartDefinesEnabled = true;
        
    }
    delete symbol;
}

std::string AST_ArrayAccessExpression::constructOutputString(HDExecutionContext *context)
{
    std::string leftNumber;
    std::string rightNumber;
    
    if(!var) var = new AST_VariableDeclaration("HD integer for array access",TYPE_DEF_INTEGER);
    
    switch(compilerContext.tokenType)
    {
        case TYPE_COLON:
            if(left) leftNumber = context->constructSubstitutionString(var,left,"array access")+"-1";
            else leftNumber = "-2";
            if(right) rightNumber = context->constructSubstitutionString(var,right,"array access")+"-1";
            else rightNumber = "-2";
            break;
        case TYPE_SEMICOLON: /* this means lone expression */
            if(left->computeFinalType(context)->type->tokenType == TYPE_DEF_ITERATOR)
            {
                leftNumber = left->constructOutputString(context);
                rightNumber = left->constructOutputString(context);
            }
            else
            {
                leftNumber = context->constructSubstitutionString(var,left,"array access")+"-1";
                rightNumber = context->constructSubstitutionString(var,left,"array access")+"-1";
            }
            break;
        case TYPE_DOT: /* this means three magic dots */
            printf("%d-%d Error - Trying to construct output string out of [...] array access expression. This is a bug in the compiler.",compilerContext.lineNumber,compilerContext.linePosition);
            exit(1);
            break;
        default:
            printf("%d-%d Error - Array access operator received unknown AST node type. This is a bug in the compiler.",compilerContext.lineNumber,compilerContext.linePosition);
            exit(1);
            break;
    }
    return leftNumber+", "+rightNumber;
}

AST_VariableDeclaration *AST_ArrayAccessExpression::computeFinalType(HDExecutionContext *context)
{
    printf("%d-%d Error - trying to compute final type of array access type. This is a bug in the compiler.",compilerContext.lineNumber,compilerContext.linePosition);
    exit(1);
    return NULL;
}

AST_ArrayAccessExpression::~AST_ArrayAccessExpression()
{
    if(left) delete left;
    if(right) delete right;
}
