//
//  AST_Identifier.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Identifier.h"
#include "AST_VariableDeclaration.h"


static AST_VariableDeclaration *funVar = NULL;

AST_Identifier::AST_Identifier(HDParseSymbol *s) : AST_Expression(s)
{
    this->name = new std::string(*(s->value));
    smartDefinesEnabled = false;
    typ = TYPE_IDENTIFIER;
}

AST_Identifier::AST_Identifier(const char *name)
{
    this->name = new std::string(name);
    smartDefinesEnabled = false;
    typ = TYPE_IDENTIFIER;
}

std::string AST_Identifier::constructOutputString(HDExecutionContext *context)
{
   // printf("Mamy do czynienia z identyfikatorem:%s\n",name->c_str());
    if(smartDefinesEnabled && computeSmartType(context)) return constructSmartOutputString(context);
    
    std::string out = "";
    out+=*name;
    return out;
}



std::string AST_Identifier::constructSmartOutputString(HDExecutionContext *context)
{
    std::string out = "";
    HDMetaContext &mt = HDMetaContext::sharedContext();
    AST_VariableDeclaration *var2 = mt.smartDefines[*name];
    if(var2) {
        out+=static_cast<ostringstream*>( &(ostringstream() <<  var2->smartDefineNumber  ) )->str();
        return out;
    }
    
    return *name;
}


bool AST_Identifier::operator!=(const AST_Identifier &op)
{
    return !((*this)==op);
}

bool AST_Identifier::operator==(const AST_Identifier &op)
{
    return name->compare(*(op.name)) == 0;
}



AST_VariableDeclaration *AST_Identifier::computeSmartType(HDExecutionContext *context)
{
        if(context->variablesDefinedInContext.find(name->c_str())!=context->variablesDefinedInContext.end()) return NULL;
    
        HDMetaContext &mt = HDMetaContext::sharedContext();
        AST_VariableDeclaration *var2 = mt.smartDefines[*name];
        if(!var2) {
            var2 = new AST_VariableDeclaration(name->c_str(),TYPE_DEF_INTEGER,(mt.highestSmartDefineTaken)++);
            mt.smartDefines[*name] = var2;
            return var2;
        } else {
            return var2;
        }
}



AST_VariableDeclaration *AST_Identifier::computeFinalType(HDExecutionContext *context) /* returned construct is borrowed for the caller */
{
  for(std::map<std::string,AST_VariableDeclaration *>::iterator it = context->variablesDefinedInContext.begin();it!=context->variablesDefinedInContext.end();it++)
  {
//    if((*it).second) {
//      printf("Variable defined in context: '%s' - '%s'\n",(*it).first.c_str(),(*it).second->name->name->c_str());
//    }
//    else
//    printf("Variable is null: %s\n",(*it).first.c_str());
  }
  AST_VariableDeclaration *var = (context->variablesDefinedInContext.find(*name))!=(context->variablesDefinedInContext.end()) ? context->variablesDefinedInContext[*name] : NULL;

//    printf("Have variable '%s', exists: %d class '%s' locus %d type %d context count %lu\n",name->c_str(),var != NULL,var? var->className.c_str():"",var?var->locus:-1,var?var->type->tokenType:-1,context->variablesDefinedInContext.size());
  
    if(!var) {
      AST_FunctionDefinition *fun = context->functionsDefinedInContext.find(*name) != context->functionsDefinedInContext.end()? context->functionsDefinedInContext.find(*name)->second : NULL;
        if(fun) {
            if(!funVar) funVar = new AST_VariableDeclaration("function",TYPE_DEF_FUNCTION);
            return funVar;
        }
        if(smartDefinesEnabled) return computeSmartType(context);
        errorMessage("Variable '%s' not defined in this scope (context: %p).\n",name->c_str(),context);
    }
    return var;
}
AST_Identifier::AST_Identifier(const AST_Identifier &c)
{
    (*this) = c;
}


AST_Identifier &AST_Identifier::operator=(const AST_Identifier& c)
{
   // printf("COpy constructor");
    if(name) delete name;
    this->compilerContext = c.compilerContext;
    name = new std::string(*(c.name));
    typ = TYPE_IDENTIFIER;
    return (*this);
}



AST_Identifier::~AST_Identifier()
{
    if(name) delete name;
}