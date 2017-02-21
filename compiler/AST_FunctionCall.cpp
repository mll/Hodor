//
//  AST_FunctionCall.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_FunctionCall.h"
#include <vector>
#include "AST_FunctionDefinition.h"
#include "AST_IdentifierExpression.h"
#include "AST_Definitions.h"


static AST_VariableDeclaration *var = NULL;
static AST_VariableDeclaration *var2 = NULL;


HDExecutionContext *AST_FunctionCall::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */
{
   switch(passNo)
    {
        case 3:
        { /* this just fills in functionNameForFunctionPointer on functional argument declaration */
            std::string name = *(ident->name);
            AST_FunctionDefinition *fun = getFunctionDefinition(context);
            if(!fun) {
                return NULL; /* We must skip all calls to function pointers */
            }
            if(fun->args->size()!=args->size())
            {
                errorMessage("Function '%s' called with wrong number of arguments.",ident->name->c_str());
            }
            int i = 0;
            for(std::vector<AST_VariableDeclaration *>::iterator it = fun->args->begin();it!=fun->args->end();it++)
            {
                AST_IdentifierExpression *arg = dynamic_cast<AST_IdentifierExpression *>((*args)[i++]);
                
                if(!arg || (*it)->type->tokenType != TYPE_DEF_FUNCTION) continue;
                
                (*it)->functionNameForFunctionPointer = *(arg->ident->name);
              printf("Bound variable %s to pointer %s\n",(*it)->name->name->c_str(),(*it)->functionNameForFunctionPointer.c_str());
            }
        }
            break;
    }
    return NULL;
}






AST_FunctionDefinition *AST_FunctionCall::getFunctionDefinition(HDExecutionContext *context)
{
    std::string name = *(ident->name);
  
    
    if(objectOfCall)
    {
        int d = random();
//        printf("%d- Calling CFT\n",d);
        AST_VariableDeclaration *var = objectOfCall->computeFinalType(context);
        assert(var);
  //      printf("%d- Returned from CFT\n",d);
    //    printf("%d- Final type computed: for OOC %d - %s - %s %d classname: %s\n",d,objectOfCall->typ,   name.c_str(),  var->name->name->c_str(),var->type->tokenType,var->className.c_str());
      
      //  printf("%d - %p\n",d,var);
      
        AST_ClassDefinition *cl = context->classesDefinedInContext[var->className];
        if(var->className.length()==0 || cl == NULL)
        {
            if(var->className.length()==0)
                this->errorMessage("%d Trying to call method '%s' of non-object variable '%s' of class %s. Var c;lass '%s'",d,name.c_str(),objectOfCall->constructOutputString(context).c_str(),objectOfCall->classNameFromAbi().c_str(),var->className.c_str());
            
            this->errorMessage("%d Trying to call method '%s' from unknown class '%s'",d,name.c_str(),var->className.c_str());
        }
        AST_FunctionDefinition *foundFun = NULL;
        AST_FunctionDefinition *fun = NULL;
        for(vector<AST_Node *>::iterator it =  cl->methods->definitions.begin(); it !=  cl->methods->definitions.end();it++)
        {
            fun = dynamic_cast<AST_FunctionDefinition *>(*it);
            if(*(fun->ident->name)==name)
            {
                foundFun = fun;
            }
        }
        fun = foundFun;
      return fun;
    }
    else
    {
        return context->findMatchingFunction(name,this);
    }
  assert(false);
  return NULL;
}





AST_VariableDeclaration *AST_FunctionCall::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HDType List",TYPE_DEF_LIST);
    if(!var2) var2 = new AST_VariableDeclaration("HDType Void",TYPE_DEF_VOID);
    
    AST_FunctionDefinition *fun = getFunctionDefinition(context);
//    printf("Hooo");
    if(!fun) {
        /* special case - extracting name from the list */
        if(objectOfCall)
        {
            AST_VariableDeclaration *callType = objectOfCall->computeFinalType(context);
            if(callType->className == "HDlist")
            {
                return var2;
            }
            
        }
        
        printf("%d-%d Error - Function '%s' not declared in this scope.\n",ident->compilerContext.lineNumber,ident->compilerContext.linePosition,ident->name->c_str());
        exit(1);
    }
   // printf("*** Computed type %d for function %s\n",fun->returnType?fun->returnType->type->tokenType:-1,fun->ident->name->c_str());
    return fun->returnType?:(fun->returnList?var:var2);
}


AST_FunctionCall::AST_FunctionCall(AST_Identifier *id, std::vector<AST_Expression*> *args,AST_Expression *objectOfCall) : AST_Expression(id)
{
    this->objectOfCall = objectOfCall;
    ident = id;
    this->args = args;
    typ = TYPE_FUNCTION_CALL;
    children.push_back(id);
    for(std::vector<AST_Expression *>::iterator i = args->begin();i!=args->end();i++)
        children.push_back(*i);
    
}

std::string AST_FunctionCall::constructOutputString(HDExecutionContext *context)
{
  //  printf("WYW Wywolanie funkcji o nazwie:\n");
    std::string out = "";
    AST_FunctionDefinition *fun = getFunctionDefinition(context);
  
    if(!fun) {
        /* special case - extracting name from the list */
        if(objectOfCall && (!args || args->size()==0))
        {
            AST_VariableDeclaration *callType = objectOfCall->computeFinalType(context);
            if(callType->className == "HDlist")
            {
                out+=objectOfCall->constructOutputString(context)+"[std::string(\"";
                out+=ident->constructOutputString(context);
                out+="\")]";
            //    printf("Special case: %s\n",out.c_str());
                return out;
            }
            
        }
        printf("%d-%d Error - Function '%s' not declared in this scope.\n",ident->compilerContext.lineNumber,ident->compilerContext.linePosition,ident->name->c_str());
        exit(1);
    }
    if(fun->args->size()!=args->size())
    {
        printf("%d-%d Error - Function '%s' called with wrong number of arguments.",ident->compilerContext.lineNumber,ident->compilerContext.linePosition,ident->name->c_str());
        exit(1);
    }
    
    if(objectOfCall) out+=objectOfCall->constructOutputString(context)+".";
  
  std::string name = ident->constructOutputString(context);
  out+="HDFunction__"+name;
  /* override for range */
  if(fun->isExternalCppFunction)
  {
    out+="<"+context->floatingPointTypeName+", char>";
  }

    out+="(";
    
   // printf("WYW posiada nAST_epujace argumenty: \n");
    
    std::vector<AST_Expression*>::iterator last;
    std::vector<AST_VariableDeclaration *>::iterator declaration = fun->args->begin();
    int ii = 1;
    for(std::vector<AST_Expression*>::iterator i = args->begin();i!=args->end();i++)
    {
     //   printf("First argument: %d %s, %d %s\n",(*declaration)->type->tokenType,(*declaration)->name->name->c_str(),(*i)->typ,(*i)->constructOutputString(context).c_str());
        out+=context->constructSubstitutionString(*(declaration++),(*i),"function call");
        if(i+1!=args->end()) out+=",";
        ii++;
    }
    out+=")";
    
   // printf("WYW sie konczy: %s\n",out.c_str());
    return out;
}


AST_FunctionCall::~AST_FunctionCall()
{
    delete ident;
    for(std::vector<AST_Expression *>::iterator i = args->begin();i!=args->end();i++)
        delete *i;
    if(objectOfCall) delete objectOfCall;
    delete args;
}

