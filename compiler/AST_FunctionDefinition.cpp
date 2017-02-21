//
//  AST_FunctionDefinition.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_FunctionDefinition.h"
#include <string>
#include <map>
#include "AST_Statement.h"
#include "AST_ReturnStatement.h"
#include "AST_FunctionCall.h"

extern bool hasMain;

AST_FunctionDefinition::AST_FunctionDefinition(AST_Identifier *id, std::vector<AST_VariableDeclaration*> *args,AST_Block *cialo,AST_VariableDeclaration *returnType, std::vector<AST_VariableDeclaration*> *returnList) : AST_Definition(id)
{
    this->ident = id;
    this->returnType = returnType;
    this->args = args;
    this->cialo = cialo;
    this->returnList = returnList;
    isExternalCppFunction = false;
    children.push_back(id);
    for(std::vector<AST_VariableDeclaration *>::iterator i = args->begin();i!=args->end();i++) children.push_back(*i);

    if(cialo)
    {
        children.push_back(cialo);
    }
    
    typ = TYPE_FUN_DEFINITION;
}


std::string AST_FunctionDefinition::defName()
{
    return *(ident->name) +" class: "+encompassingClass;

}


bool AST_FunctionDefinition::operator!=(const AST_FunctionDefinition &op)
{
    return !((*this)==op);
}

bool AST_FunctionDefinition::operator==(const AST_FunctionDefinition &op)
{
    if(*ident != *(op.ident)) return false;
    if(args->size()!=op.args->size()) return false;
    
    vector<AST_VariableDeclaration *>::iterator it1 = args->begin();
    vector<AST_VariableDeclaration *>::iterator it2 = op.args->begin();
    
    while(it1!=args->end())
    {
        AST_VariableDeclaration *var1 = (*it1);
        AST_VariableDeclaration *var2 = (*it2);
        if((*var1)!=(*var2)) return false;
        
        it1++;
        it2++;
    }
    return true;
}


bool AST_FunctionDefinition::functionCallMatches(const AST_FunctionCall &op,HDExecutionContext *context)
{
    if(args->size()!=op.args->size()) return false;
    vector<AST_VariableDeclaration *>::iterator varIt = args->begin();
    vector<AST_Expression *>::iterator expIt = op.args->begin();
    while(varIt!=args->end())
    {
        if(context->checkVariableApplicability(*varIt,*expIt)!=true) return false;
        varIt++;
        expIt++;
    }
    return true;
}

HDExecutionContext *AST_FunctionDefinition::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */
{
    switch(passNo)
    {
        case 1:
        {
            string prefix = encompassingClass.length() ? "_HDClass_"+encompassingClass+"__" : "";
            string fullNames = (prefix + *(ident->name));
            //    for(std::vector<AST_VariableDeclaration*>::iterator it = args->begin();it!=args->end();it++)
            //    {
            //        fullName+="__";
            //        if((*it)->className.length()) fullName+=(*it)->className;
            //        else fullName += intToString((*it)->type->tokenType)+"_"+ ((*it)->sizes ? "S" : "N");
            //    }
            
            assert(context);
            //    printf("First pass for fun %s\n",fullNames.c_str());
            AST_FunctionDefinition *alreadyDefinedVar = context->findMatchingFunction(fullNames,this);
            
            if(alreadyDefinedVar && alreadyDefinedVar->cialo && cialo) {
                printf("%d-%d Error - function '%s' already defined in %d-%d. Internal representation is '%s'\n",ident->compilerContext.lineNumber,ident->compilerContext.linePosition,ident->name->c_str(),alreadyDefinedVar->ident->compilerContext.lineNumber,alreadyDefinedVar->ident->compilerContext.linePosition,alreadyDefinedVar->ident->name->c_str());
                exit(1);
            }
            if(!alreadyDefinedVar || alreadyDefinedVar->cialo)
            {
                context->functionsDefinedInContext.insert(pair<string,AST_FunctionDefinition *>(fullNames,this));
                 if(fullNames.compare(std::string("main"))==0) hasMain = true;
            }
            return NULL;
        }
        case 2:
        {
                executionContext = *context;
                executionContext.encompassingFunction = this;
                if(returnType) returnType->locus = AST_VariableDeclarationLocusReturnList;
                
                for(std::vector<AST_VariableDeclaration*> :: iterator i = this->args->begin();i!=args->end();i++)
                {
                   (*i)->locus = AST_VariableDeclarationLocusArgument;
                }
                return &executionContext;
        }
        case 3:
            return &executionContext;
        case 4:
            return &executionContext;
            
            break;
        case 5:
            if(encompassingClass.length()==0) *output =  constructDefinitionString(context)+";\n";
            return &executionContext;
            break;
        default:
            break;
    }
    return NULL;
}


std::string AST_FunctionDefinition::constructOutputString(HDExecutionContext *context)
{
    std::string out = "";
    if(cialo)
    {
     if((returnType && returnType->type->tokenType != TYPE_DEF_VOID) || returnList)
     {
         if(this->cialo->statements.size() < 1) this->errorMessage("Function with return type different than void has no return statement.");
         AST_Statement *ret = *(this->cialo->statements.end()-1);
         if(ret->typ != TYPE_STATEMENT_RETURN) ret->errorMessage("Function with return type different than void has to end with a valid return statement.");
         AST_ReturnStatement *ret2 = dynamic_cast<AST_ReturnStatement *>(ret);
         if(ret2->expression == NULL) ret2->errorMessage("Function with return type different than void has to end with a valid return statement.");
     }
        
     if(!cialo)
     {
         printf("Building body-less method %s",ident->name->c_str());
         
     }
        
        
     out+= constructDefinitionString(&executionContext)+"\n";
     out+= cialo->constructOutputString(&executionContext);
    }
    return out;
}


std::string AST_FunctionDefinition::constructDefinitionWithName(std::string name,HDExecutionContext *context)
{
    std::string out = "";
    if(!returnList)
        switch(returnType->type->tokenType)
    {
        case TYPE_DEF_VOID:
            out+="void ";
            if(returnType->sizes) {
                printf("%d-%d Error - cannot return array of voids.",returnType->type->lineNumber,returnType->type->linePosition);
                exit(1);
            }
            break;
        case TYPE_DEF_STRING:
            if(returnType->sizes) out+="HDList<"+context->floatingPointTypeName+",char> ";
            else out+="HDStringEnvelope<"+context->floatingPointTypeName+",char> ";
            break;
        case TYPE_DEF_DECIMAL:
            if(returnType->sizes) out+="HDTensor<"+context->floatingPointTypeName+"> ";
            else out+=context->floatingPointTypeName+" ";
            break;
        case TYPE_DEF_INTEGER:
            if(returnType->sizes) out+="HDTensor<int> ";
            else out+="int ";
            break;
        case TYPE_DEF_LIST:
            if(returnType->sizes) {
                printf("%d-%d Error - cannot return array of lists.",returnType->type->lineNumber,returnType->type->linePosition);
                exit(1);
            }
            out+="HDList<"+context->floatingPointTypeName+",char> ";
            break;
        case TYPE_DEF_DICTIONARY:
            if(returnType->sizes) {
                printf("%d-%d Error - cannot return array of dictionaries.",returnType->type->lineNumber,returnType->type->linePosition);
                exit(1);
            }
            out+="HDDict ";
            break;
        default:
            printf("%d-%d Error - unknown return type.",returnType->type->lineNumber,returnType->type->linePosition);
            exit(1);
    }
    else
    { /* returns a list */
        out+="HDList<"+context->floatingPointTypeName+",char> ";
    }
    
    //   printf("DEF Deklaracja funkcji o nazwie:\n");
    out+= name+"(";
    // printf("DEF Deklaracja funkcji zwracajacej %s\n",returnType->type->value->c_str());
    
    // printf("DEF posiada nastepujace argumenty: \n");
    
    for(std::vector<AST_VariableDeclaration*>::iterator i = args->begin();i!=args->end();i++)
    {
        (*i)->locus = AST_VariableDeclarationLocusArgument;
        out+=(*i)->constructDeclarationString(context);
        if(i+1 != args->end()) out+=", ";
    }
    out+=")";
    // printf("DEF ta fukcja posiada cialo: \n");
    //  printf("DEF sie konczy\n");
    return out;
    
}


std::string AST_FunctionDefinition::constructDefinitionString(HDExecutionContext *context) {
    return constructDefinitionWithName("HDFunction__"+*(ident->name),context);
}



AST_FunctionDefinition::~AST_FunctionDefinition()
{
    if(cialo) delete cialo;
    delete ident;
    delete args;
    delete returnType;
}
