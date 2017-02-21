#include "nodes.h"
#include "parser.hpp"

#include <iostream>
#include <sstream>
#include <assert.h>
#include <execinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AST_FunctionDefinition.h"
#include "AST_FunctionCall.h"

FILE *outFile;


void print_trace (void)
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;
  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);
  printf ("Obtained %zd stack frames.\n", size);
  for (i = 0; i < size; i++)
    printf ("%s\n", strings[i]);
  free (strings);
}


bool HDExecutionContext::checkVariableApplicability(AST_VariableDeclaration *expected, AST_Expression *got)
{
    AST_VariableDeclaration *ourType = expected;
    AST_VariableDeclaration *rightType = got->computeFinalType(this);
    bool ok = rightType->type->tokenType == TYPE_DEF_VOID ? true : false;
    bool hasVoid = ok;
    
    if(rightType->type->tokenType == TYPE_DEF_ITERATOR)
    {
        return false;
    }
    
    
    if(((ourType->sizes && rightType->sizes) || (ourType->sizes == NULL && rightType->sizes == NULL)) )
    {
        if( ourType->type->tokenType == rightType->type->tokenType)
            ok = true;
        
        if(((ourType->type->tokenType == TYPE_DEF_STRING && ourType->sizes == NULL) || (ourType->type->tokenType == TYPE_DEF_INTEGER && ourType->sizes == NULL) || (ourType->type->tokenType == TYPE_DEF_DECIMAL && ourType->sizes == NULL)) && (rightType->type->tokenType == TYPE_DEF_DECIMAL || rightType->type->tokenType == TYPE_DEF_INTEGER))
            ok = true;
    }
    
    if(!ok)
    {  /* maybe we expect void? If so, the method has to support float, int and HDMemoryModel &*/
        if(ourType->type->tokenType == TYPE_DEF_VOID)
        {
            ok = true;
        }
    }
    return ok;
}


std::string HDExecutionContext::constructSubstitutionString(AST_VariableDeclaration *expected, AST_Expression *got,const char *callType)
{

    
    
    AST_VariableDeclaration *ourType = expected;
    AST_VariableDeclaration *rightType = got->computeFinalType(this);
    bool ok = rightType->type->tokenType == TYPE_DEF_VOID ? true : false;
    bool hasVoid = ok;
    
    if(rightType->type->tokenType == TYPE_DEF_ITERATOR)
    {
        printf("%d-%d Error - Conflicting types in %s. Cannot pass iterator.",got->compilerContext.lineNumber,got->compilerContext.linePosition,callType);
        exit(1);
    }
    
    
    if(((ourType->sizes && rightType->sizes) || (ourType->sizes == NULL && rightType->sizes == NULL)) )
    {
        if( ourType->type->tokenType == rightType->type->tokenType)
            ok = true;
        
        if(((ourType->type->tokenType == TYPE_DEF_STRING && ourType->sizes == NULL) || (ourType->type->tokenType == TYPE_DEF_INTEGER && ourType->sizes == NULL) || (ourType->type->tokenType == TYPE_DEF_DECIMAL && ourType->sizes == NULL)) && (rightType->type->tokenType == TYPE_DEF_DECIMAL || rightType->type->tokenType == TYPE_DEF_INTEGER))
            ok = true;
    }
    
    if(!ok)
    {  /* maybe we expect void? If so, the method has to support float, int and HDMemoryModel &*/
        if(ourType->type->tokenType == TYPE_DEF_VOID)
        {
            ok = true;
        }
    }
    
    
    if(!ok) {
        got->errorMessage("Conflicting types in %s for '%s' (%d; sizes = %d,%d; sizes = %d)",callType,ourType->name->name->c_str(),ourType->type->tokenType,ourType->sizes != NULL,   rightType->type->tokenType,rightType->sizes != NULL);
    }
    
    string partial = "";
    
    if(ourType->type->tokenType == TYPE_DEF_STRING && (rightType->type->tokenType == TYPE_DEF_INTEGER || rightType->type->tokenType == TYPE_DEF_DECIMAL))
        partial = "HDStringEnvelope<"+this->floatingPointTypeName+",char>::stringFromInt((int)"+got->constructOutputString(this)+")";
    if(ourType->type->tokenType == TYPE_DEF_DECIMAL && rightType->type->tokenType == TYPE_DEF_INTEGER)
        partial = "("+this->floatingPointTypeName+")"+got->constructOutputString(this);
    if(ourType->type->tokenType == TYPE_DEF_INTEGER && rightType->type->tokenType == TYPE_DEF_DECIMAL)
        partial = "(int)"+got->constructOutputString(this);
    
    
    
    if(rightType->type->tokenType == TYPE_DEF_VOID)
    {
        switch (ourType->type->tokenType) {
            case TYPE_DEF_INTEGER:
                if(ourType->sizes == NULL)
                    partial = "HDUniversalEnvelope<"+this->floatingPointTypeName+",char>::explicitIntCast("+ got->constructOutputString(this)+")";
                else partial = "HDUniversalEnvelope<"+this->floatingPointTypeName+",char>::explicitHDTensorCast("+ got->constructOutputString(this)+")";
                break;
            case TYPE_DEF_DECIMAL:
                if(ourType->sizes == NULL)
                    partial = "HDUniversalEnvelope<"+this->floatingPointTypeName+",char>::explicitDecimalCast("+ got->constructOutputString(this)+")";
                else partial = "HDUniversalEnvelope<"+this->floatingPointTypeName+",char>::explicitHDTensorCast("+ got->constructOutputString(this)+")";
                break;
            case TYPE_DEF_STRING:
                partial = "HDUniversalEnvelope<"+this->floatingPointTypeName+",char>::explicitHDStringEnvelopeCast("+ got->constructOutputString(this)+")";
                break;
            case TYPE_DEF_LIST:
                partial = "HDUniversalEnvelope<"+this->floatingPointTypeName+",char>::explicitHDListCast("+ got->constructOutputString(this)+")";
                break;
                
            default:
                break;
        }
    }

    
    if(partial.length()==0)
    {
        partial = (expected->type->tokenType == TYPE_DEF_FUNCTION ? "HDFunction__"  : "") +got->constructOutputString(this);
//        printf("Partial: %s\n",partial.c_str());
    }
    
//    if(strcmp(got->constructOutputString(this).c_str(),"i")==0)
//    {
//        printf("Substituting %s: %s %d\n",got->constructOutputString(this).c_str(),partial.c_str(),rightType->type->tokenType);
//    }

    
    return partial;
}



void HDExecutionContext::addFunctionDefinedInContext(AST_FunctionDefinition *fun, std::string name)
{
  functionsDefinedInContext.insert(pair<string,AST_FunctionDefinition *>(name,fun));
  for(int i=0;i<children.size();i++) children[i]->addFunctionDefinedInContext(fun,name);
}

HDExecutionContext &HDExecutionContext::operator=(HDExecutionContext &k)
{
    variablesDefinedInContext = k.variablesDefinedInContext;
    functionsDefinedInContext = k.functionsDefinedInContext;
    classesDefinedInContext   = k.classesDefinedInContext;
    floatingPointTypeName     = k.floatingPointTypeName;
    encompassingFunction      = k.encompassingFunction;

    for(int i=0;i<k.children.size();i++)
    {
      if(k.children[i]==this) return (*this);
    }
  
    k.children.push_back(this);
  
    return *this;
}


AST_FunctionDefinition *HDExecutionContext::findMatchingFunction(std::string name,AST_FunctionDefinition *fun)
{
    std::pair <std::multimap<std::string,AST_FunctionDefinition *>::iterator, std::multimap<std::string,AST_FunctionDefinition *>::iterator> ret;
    ret = functionsDefinedInContext.equal_range(name);
    for (std::multimap<std::string,AST_FunctionDefinition *>::iterator it=ret.first; it!=ret.second; it++) if(*(it->second) == *fun) return it->second;
    return NULL;
}

AST_FunctionDefinition *HDExecutionContext::findMatchingFunction(std::string name,AST_FunctionCall *fun)
{
    std::pair <std::multimap<std::string,AST_FunctionDefinition *>::iterator, std::multimap<std::string,AST_FunctionDefinition *>::iterator> ret;
    ret = functionsDefinedInContext.equal_range(name);
    for (std::multimap<std::string,AST_FunctionDefinition *>::iterator it=ret.first; it!=ret.second; it++) if(  (*(it->second)).functionCallMatches(*fun,this)) return it->second;
    return NULL;
}








































  














