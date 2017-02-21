//
//  AST_Substitution.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Substitution.h"
#include "AST_Identifier.h"
#include "AST_VariableDeclaration.h"
#include "AST_BinaryOperator.h"

AST_Substitution::~AST_Substitution()
{
    if(left) delete left;
    if(leftAccess) delete leftAccess;
    if(right) delete right;
}

AST_Substitution::AST_Substitution(AST_Identifier *left,AST_Expression *right,HDParseSymbol *s) : AST_Expression(s)
{
    this->left = left;
    this->right = right;
    this->leftAccess = NULL;
    children.push_back(left);
    children.push_back(right);
    typ = TYPE_SUBSTITUTION;
}

AST_Substitution::AST_Substitution(AST_ArrayAccess *left,AST_Expression *right,HDParseSymbol *s) : AST_Expression(s)
{
    this->leftAccess = left;
    this->right = right;
    this->left = NULL;
    children.push_back(left);
    children.push_back(right);
    
    typ = TYPE_SUBSTITUTION;
}




std::string AST_Substitution::constructOutputString(HDExecutionContext *context)
{
  //  printf("Mamy do czynienia z instrukcja przypisania:\n");
    AST_VariableDeclaration *ourType = this->computeFinalType(context);
    AST_VariableDeclaration *rightType = right->computeFinalType(context);
    std::string leftOut = "";
    
  std::string operatorMark = computeOperatorMark(context);
  
    AST_VariableDeclaration *var = computeFinalType(context);
    
    
    if(left)
    {
    if(var->locus == AST_VariableDeclarationLocusForLoop)
    {
        if(var->type->tokenType != TYPE_DEF_ITERATOR) leftOut = "(*"+left->constructOutputString(context)+")";
        else
        {
            printf("%d-%d Error - Cannot substitute for iterator '%s' ",left->compilerContext.lineNumber,left->compilerContext.linePosition,left->name->c_str());
            exit(1);
            
        }
    }  else leftOut = left->constructOutputString(context);
    }
    else
    {
        leftOut = leftAccess->constructOutputString(context);
    }
    
    
    
    if(rightType->type->tokenType == TYPE_DEF_VOID)
    {
        if(ourType->type->tokenType == TYPE_DEF_INTEGER && ourType->sizes == NULL)
             return leftOut+" "+operatorMark+" HDUniversalEnvelope<"+context->floatingPointTypeName+",char>::explicitIntCast("+ right->constructOutputString(context)+")";
        if(ourType->type->tokenType == TYPE_DEF_DECIMAL && ourType->sizes == NULL)
             return leftOut+" "+operatorMark+" HDUniversalEnvelope<"+context->floatingPointTypeName+",char>::explicitDecimalCast("+ right->constructOutputString(context)+")";
        if(ourType->type->tokenType == TYPE_DEF_STRING)
             return leftOut+" "+operatorMark+" HDUniversalEnvelope<"+context->floatingPointTypeName+",char>::explicitHDStringEnvelopeCast("+ right->constructOutputString(context)+")";
        if(ourType->type->tokenType == TYPE_DEF_DECIMAL && ourType->sizes != NULL)
            return leftOut+" "+operatorMark+" HDUniversalEnvelope<"+context->floatingPointTypeName+",char>::explicitHDTensorCast("+ right->constructOutputString(context)+")";
      if(ourType->type->tokenType == TYPE_DEF_LIST && ourType->sizes == NULL)
        return leftOut+" "+operatorMark+" HDUniversalEnvelope<"+context->floatingPointTypeName+",char>::explicitHDListCast("+ right->constructOutputString(context)+")";
      
            
    }
    if(((ourType->sizes && rightType->sizes) || (ourType->sizes == NULL && rightType->sizes == NULL)) )
    {

        if( ourType->type->tokenType == rightType->type->tokenType)
            return leftOut+" "+operatorMark+" "+ right->constructOutputString(context);
        if(( (ourType->type->tokenType == TYPE_DEF_INTEGER && ourType->sizes == NULL) || (ourType->type->tokenType == TYPE_DEF_DECIMAL && ourType->sizes == NULL)) && (rightType->type->tokenType == TYPE_DEF_DECIMAL || rightType->type->tokenType == TYPE_DEF_INTEGER))
            return leftOut+" "+operatorMark+" "+ right->constructOutputString(context);
        if(ourType->type->tokenType == TYPE_DEF_STRING && ourType->sizes == NULL && (rightType->type->tokenType == TYPE_DEF_DECIMAL || rightType->type->tokenType == TYPE_DEF_INTEGER))
            return leftOut+" "+operatorMark+" HDStringEnvelope<"+context->floatingPointTypeName+",char>::stringFromInt("+ right->constructOutputString(context)+")";
    }
    
    if(ourType->sizes && !(rightType->sizes) && (ourType->type->tokenType == TYPE_DEF_DECIMAL || ourType->type->tokenType == TYPE_DEF_INTEGER) && (rightType->type->tokenType == TYPE_DEF_DECIMAL || rightType->type->tokenType == TYPE_DEF_INTEGER))
    {
           return leftOut+" "+operatorMark+" "+ right->constructOutputString(context);
    }
    
    
    errorMessage("Conflicting types in substution: %d and %d ",ourType->type->tokenType,rightType->type->tokenType);
    return "";
}


std::string AST_Substitution::computeOperatorMark(HDExecutionContext *context)
{
  if(!left) return std::string("=");
  if(right->typ != TYPE_BINARY_OPERATOR) return std::string("=");
  AST_BinaryOperator *binaryOp = dynamic_cast<AST_BinaryOperator *>(right);
  assert(binaryOp);
  if(binaryOp->findPlusSubstitution(*(left->name),context)) return std::string("+=");
  return std::string("=");
}


AST_VariableDeclaration *AST_Substitution::computeFinalType(HDExecutionContext *context)
{
    if(this->leftAccess)
    {
        return leftAccess->computeFinalType(context);
    }
    
    
  map<std::string,AST_VariableDeclaration *>::iterator it = context->variablesDefinedInContext.find(left->name->c_str());
    if(it == context->variablesDefinedInContext.end()) {
        errorMessage("Variable '%s' not declared in this scope.",left->name->c_str());
    }
    AST_VariableDeclaration *var = (*it).second;
  
    return var;
}
