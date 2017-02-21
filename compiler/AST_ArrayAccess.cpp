//
//  AST_ArrayAccess.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_ArrayAccess.h"
#include "AST_VariableDeclaration.h"
#include <sstream>
#include <stdio.h>

static AST_VariableDeclaration *varVoid = NULL;
static AST_VariableDeclaration *varString = NULL;
static AST_VariableDeclaration *varInt = NULL;
static AST_VariableDeclaration *varDec = NULL;

AST_ArrayAccess::AST_ArrayAccess(AST_Expression *where,vector<AST_ArrayAccessExpression *> *indices)
{
    this->where = where;
    this->indices = indices;
    compilerContext = where->compilerContext;
  
    children.push_back(where);
    
    for(vector<AST_ArrayAccessExpression *>::iterator i = indices->begin();i!=indices->end();i++)
    {
     children.push_back(*i);
    }
    typ = TYPE_ACCESS;
}

std::string AST_ArrayAccess::constructOutputString(HDExecutionContext *context)
{
    std::string accessString = where->constructOutputString(context);
    std::string out = "";
    
    if(indices->size() == 0)
    {
        printf("%d-%d Error - Array access has to have at least one index.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
        exit(1);
    }
  //  printf("Coming!\n");
//    fflush(stdout);
    if(!varInt) varInt = new AST_VariableDeclaration("HD int type",TYPE_DEF_INTEGER);
    AST_VariableDeclaration *initialObjectType = where->computeFinalType(context);
    AST_VariableDeclaration *finalType = computeFinalType(context);
    AST_ArrayAccessExpression *index = NULL;
    /* first - produce decimal array */
    switch (initialObjectType->type->tokenType)
    {
        case TYPE_DEF_LIST:
            if(indices->size()!=1) {
                printf("%d-%d Error - Lists are one-dimensional only.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
                exit(1);
            }
            index = indices->front();
            switch(index->compilerContext.tokenType)
            {
                case TYPE_COLON:
                    accessString+=".sublist("+index->constructOutputString(context)+")";
                    out = "("+accessString+")";
                    break;
                case TYPE_SEMICOLON:
                    if(index->left->computeFinalType(context)->type->tokenType == TYPE_DEF_ITERATOR)
                    {
                        accessString+="["+index->left->constructOutputString(context)+"]";
                    }
                    else
                    {
                        accessString+="["+index->left->constructOutputString(context)+"-1]";
                    }
                    out = accessString;
                    
                    break;
                case TYPE_DOT:
                    out = accessString;
                    break;
                default:
                    printf("%d-%d Error - Unknown array access expression while constructing array access.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
                    exit(1);
                    break;
            }
            break;
        case TYPE_DEF_DECIMAL:
        case TYPE_DEF_INTEGER:
            if(finalType->sizes == NULL)
            {
                if((*indices)[0]->left->computeFinalType(context)->type->tokenType == TYPE_DEF_ITERATOR)
                {

                    if(indices->size()>1) {
                        printf("%d-%d Error - When iterator is used for array access, only one element can be in the brackets.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
                        exit(1);
                    }
                    accessString+=".getElement("+(*indices)[0]->left->constructOutputString(context)+")";
                }
                else
                {

                    accessString+=".getElement("+static_cast<ostringstream*>( &(ostringstream() << indices->size()) )->str()+", ";
                    for(vector<AST_ArrayAccessExpression *>::iterator i = indices->begin();i!=indices->end();i++)
                    {
                        accessString+=context->constructSubstitutionString(varInt,(*i)->left,"array access")+"-1";
                        if(i+1 != indices->end()) accessString+=", ";
                    }
                    accessString+=")";
                }
                
                out = "(*"+accessString+")";
            }
            else
            {
              //  printf("Trying!!!");
                fflush(stdout);
                int numberOfArguments = 0;
                for(vector<AST_ArrayAccessExpression *>::iterator i = indices->begin();i!=indices->end();i++)
                {
                    if((*i)->compilerContext.tokenType != TYPE_DOT) numberOfArguments++;
                    else {
                        if(i+1 != indices->end()) {
                            printf("%d-%d Error - '...' array access element is not last.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
                            exit(1);
                        }
                    }
                }
                std::string tmp = "";
                bool hasThreeDots = false;
                tmp+="("+static_cast<ostringstream*>( &(ostringstream() << numberOfArguments) )->str()+", ";
                for(vector<AST_ArrayAccessExpression *>::iterator i = indices->begin();i!=indices->end();i++)
                {
                   if((*i)->compilerContext.tokenType != TYPE_DOT)
                   {
                       tmp+=(*i)->constructOutputString(context);
                   } else
                   {
                       hasThreeDots = true;
                   }
                   if(i+1 != indices->end() && (*(i+1))->compilerContext.tokenType != TYPE_DOT) tmp+=", ";
                }
                tmp+=")";
                accessString+= (hasThreeDots ? ".newSubTensor" : ".newReversedSubTensor") + tmp;
                out = accessString;
            }
            break;
        case TYPE_DEF_STRING:
            if(indices->size()!=1) {
                printf("%d-%d Error - Strings are one-dimensional only.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
                exit(1);
            }
            index = indices->front();
            switch(index->compilerContext.tokenType)
            {
            case TYPE_COLON:
                accessString+=".substring("+index->constructOutputString(context)+")";
                out = accessString;
                break;
            case TYPE_SEMICOLON:
                    if(index->left->computeFinalType(context)->type->tokenType == TYPE_DEF_ITERATOR)
                    {
                        accessString+="["+context->constructSubstitutionString(varInt,index->left,"array access")+"]";
                    }
                    else
                    {
                        accessString+="["+context->constructSubstitutionString(varInt,index->left,"array access")+"-1]";
                    }
                out = accessString;
                break;
            case TYPE_DOT:
                out = "("+accessString+")";
                break;
            default:
                printf("%d-%d Error - Unknown array access expression while constructing array access.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
                exit(1);
                break;
            }
            break;
            
        default:
            errorMessage("Unknown array access base expression");
            break;
    }
//    printf("Array access produced %s\n",out.c_str());
    return out;
}






AST_VariableDeclaration *AST_ArrayAccess::computeFinalType(HDExecutionContext *context)
{
    AST_VariableDeclaration *initialObjectType = where->computeFinalType(context);
    switch (initialObjectType->type->tokenType) {
        case TYPE_DEF_LIST:
        case TYPE_DEF_VOID:
            if(!varVoid) varVoid = new AST_VariableDeclaration("HD unknown type",TYPE_DEF_VOID);
            return varVoid;
            break;
        case TYPE_DEF_INTEGER:
        case TYPE_DEF_DECIMAL:
        {
            if(initialObjectType->sizes == NULL)
            {
                printf("%d-%d Error - The expression must be a list, array or string to access its subarray.",where->compilerContext.lineNumber,where->compilerContext.linePosition);
                exit(1);
            }
            bool hasSizes = false;
            for(vector<AST_ArrayAccessExpression *>::iterator i = indices->begin();i!=indices->end();i++)
                if((*i)->compilerContext.tokenType != TYPE_SEMICOLON) hasSizes = true;
            if(hasSizes) return initialObjectType;
            
            if(!varInt) varInt = new AST_VariableDeclaration("HD int type",TYPE_DEF_INTEGER);
            if(initialObjectType->type->tokenType == TYPE_DEF_INTEGER) return varInt;
            if(!varDec) varDec = new AST_VariableDeclaration("HD dec type",TYPE_DEF_DECIMAL);
            if(initialObjectType->type->tokenType == TYPE_DEF_DECIMAL) return varDec;
            assert(0);
        }
            break;
        case TYPE_DEF_STRING:
            if(!varString) varString = new AST_VariableDeclaration("HD string type",TYPE_DEF_STRING);
            return varString;
            break;
        default:
            printf("%d-%d Error - Cannot access elements of type %d",where->compilerContext.lineNumber,where->compilerContext.linePosition,initialObjectType->type->tokenType);
            exit(1);
    }
    
    assert(0);
    return NULL;
}


AST_ArrayAccess::~AST_ArrayAccess()
{
    for(vector<AST_ArrayAccessExpression *>::iterator i = indices->begin();i!=indices->end();i++)
    {
        delete *i;
    }
    
    delete where;
    delete indices;
}