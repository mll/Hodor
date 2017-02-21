//
//  AST_ConditionalExpression.cpp
//  Hodor
//
//  Created by Test on 26.12.2013.
//
//

#include "AST_ConditionalExpression.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *var = NULL;


AST_ConditionalExpression::AST_ConditionalExpression(AST_Expression *condition,AST_Expression *yesExpression,AST_Expression *noExpression) : AST_Expression(condition)
{
    this->condition = condition;
    this->yesExpression = yesExpression;
    this->noExpression = noExpression;
    if(condition) children.push_back(condition);
    if(yesExpression) children.push_back(yesExpression);
        if(noExpression) children.push_back(noExpression);

    typ = TYPE_CONDITIONAL_EXPRESSION;
}



std::string AST_ConditionalExpression::constructOutputString(HDExecutionContext *context)
{
    AST_VariableDeclaration *returnType = computeFinalType(context);
    std::string yesText = yesExpression->constructOutputString(context);
    std::string noText = noExpression->constructOutputString(context);
    std::string conditionText = condition->constructOutputString(context);
    
    AST_VariableDeclaration *left = yesExpression->computeFinalType(context);
    AST_VariableDeclaration *right = noExpression->computeFinalType(context);
    AST_VariableDeclaration *conditionType = condition->computeFinalType(context);
    
    if(!var) var = new AST_VariableDeclaration("HD integer",TYPE_DEF_INTEGER);
    if(conditionType->type->tokenType == TYPE_DEF_VOID)
    {
        conditionText = context->constructSubstitutionString(var,condition,"? operator's condition");
    }
    if(returnType->type->tokenType != TYPE_DEF_VOID)
    {
        if(left->type->tokenType == TYPE_DEF_VOID) yesText = context->constructSubstitutionString(returnType,yesExpression,"? operator's yes clause");
        if(right->type->tokenType == TYPE_DEF_VOID) noText = context->constructSubstitutionString(returnType,noExpression,"? operator's no clause");
    }
    return "("+conditionText+" ? "+yesText+" : "+noText+")";
}


AST_ConditionalExpression::~AST_ConditionalExpression()
{
    delete condition;
    delete yesExpression;
    delete noExpression;
}

AST_VariableDeclaration *AST_ConditionalExpression::computeFinalType(HDExecutionContext *context)
{
    AST_VariableDeclaration *left = yesExpression->computeFinalType(context);
    AST_VariableDeclaration *right = noExpression->computeFinalType(context);
    if(right->type->tokenType == TYPE_DEF_VOID) return left;
    if(left->type->tokenType == TYPE_DEF_VOID) return right;
       
    
    if(left->type->tokenType != right->type->tokenType)
    {
        errorMessage("? operator has to have the same types, instead got %d and %d",left->type->tokenType,right->type->tokenType);
    }
    if((left->sizes == NULL && right->sizes != NULL) || (left->sizes != NULL && right->sizes == NULL))
    {
        errorMessage("? operator has to have the same types, instead got arrays and single types");
    }
    assert(condition);
    AST_VariableDeclaration *conditionType = condition->computeFinalType(context);
    if((conditionType->type->tokenType != TYPE_DEF_INTEGER && conditionType->type->tokenType != TYPE_DEF_VOID) || conditionType->sizes)
    {
        errorMessage("? operator has been given improper type as a condition: %d",conditionType->type->tokenType);
    }

    
    
    return left;
}
