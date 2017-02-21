//
//  AST_ClassDefinition.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_ClassDefinition.h"
#include "AST_VariableDeclaration.h"
#include "AST_Identifier.h"
#include "AST_Definitions.h"

using namespace std;



HDExecutionContext *AST_ClassDefinition::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */
{
    switch(passNo)
    {
        case 1:
        {
            AST_ClassDefinition *alreadyDefinedVar = context->classesDefinedInContext[*(ident->name)];
            if(alreadyDefinedVar)
            {
                this->errorMessage("Class %s already defined in %d-%d",ident->name->c_str(),alreadyDefinedVar->compilerContext.lineNumber,alreadyDefinedVar->compilerContext.linePosition);
            }
            context->classesDefinedInContext[*(ident->name)] = this;
            
            for(std::vector<AST_Node *>::iterator it = methods->definitions.begin();it!=methods->definitions.end();it++)
            {
                if((*it)->typ != TYPE_FUN_DEFINITION)
                {
                    (*it)->errorMessage("Only functions can be declared inside a class");
                }
            }
            return NULL;
        }
            break;
        case 2:
            executionContext = *context;
            return &executionContext;
            break;
        case 5:
        {
            std::string out = "\nclass " + ident->constructOutputString(&executionContext);
            
            if(inherits->size()) out+=" : public ";
            
            
            for(std::vector<AST_Identifier*>::iterator i = inherits->begin();i!=inherits->end();i++)
            {
                out+=(*i)->constructOutputString(&executionContext);
                if(i+1!=inherits->end()) out+=", ";
            }
            out+="\n{\npublic:\n";
            
            for(std::vector<AST_VariableDeclaration*>::iterator i = fields->begin();i!=fields->end();i++)
            {
                (*i)->locus = AST_VariableDeclarationLocusClassVariable;
                out+=(*i)->constructOutputString(&executionContext);
                out+=";\n";
            }


            
            for(vector<AST_Node *>::iterator i=methods->definitions.begin();i!=methods->definitions.end();i++)
            {
                AST_FunctionDefinition *d = dynamic_cast<AST_FunctionDefinition *>(*i);
                if(d->cialo) out+=(d)->constructOutputString(&executionContext);
                else out+= d->constructDefinitionString(&executionContext)+";\n";
            }
            
        
            out+="};\n\n";
            
            *output = out;
        }
        default:
            break;
    }
    return NULL;
}

std::string AST_ClassDefinition::defName()
{
    return *(ident->name);
}



AST_ClassDefinition::AST_ClassDefinition(AST_Identifier *id, std::vector<AST_Identifier*> *inherits,std::vector<AST_Node*> *body) : AST_Definition(id)
{
    ident = id;
    this->inherits = inherits;
    
    
    
    typ = TYPE_DEF_CLASS;
    this->fields = new vector<AST_VariableDeclaration *>();
    this->methods = new AST_Definitions();
    
    
    for(vector<AST_Node *>::iterator it = body->begin();it!=body->end();it++)
    {
        if((*it)->typ == TYPE_FUN_DEFINITION)
        {
            AST_FunctionDefinition *f = dynamic_cast<AST_FunctionDefinition *>((*it));
            f->encompassingClass = *(ident->name);
            methods->addDefinition(f);
            continue;
        }
        if((*it)->typ == TYPE_VAR_DECLARATION)
        {
            fields->push_back(dynamic_cast<AST_VariableDeclaration *>((*it)));
            continue;
        }
        this->errorMessage("Unpermitted definition of type %d in class definition",(*it)->typ);
    }
    delete body;
    children.push_back(id);
    for(std::vector<AST_Identifier*>::iterator i = inherits->begin();i!=inherits->end();i++) children.push_back(*i);
    for(std::vector<AST_VariableDeclaration*>::iterator i = fields->begin();i!=fields->end();i++) children.push_back(*i);
    children.push_back(methods);
}

std::string AST_ClassDefinition::constructOutputString(HDExecutionContext *context)
{
    return std::string();
}









AST_ClassDefinition::~AST_ClassDefinition()
{
    delete fields;
    delete methods;
    delete ident;
    delete inherits;
}