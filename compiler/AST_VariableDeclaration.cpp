//
//  AST_VariableDeclaration.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_VariableDeclaration.h"
#include <iostream>
#include "AST_FunctionDefinition.h"
#include "AST_Substitution.h"

static AST_VariableDeclaration *intVar = NULL;
extern HDExecutionContext primaryContext;

bool AST_VariableDeclaration::operator==(const AST_VariableDeclaration &op)
{
    if(type->tokenType != op.type->tokenType) return false;
    if((!sizes && op.sizes) || (sizes && !op.sizes)) return false;
    return true;
}

bool AST_VariableDeclaration::operator!=(const AST_VariableDeclaration &op)
{
    return !((*this)==op);
}



HDExecutionContext *AST_VariableDeclaration::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output)
{
    switch(passNo)
    {
        case 2:
        {
            AST_VariableDeclaration *alreadyDefinedVar = NULL;
            AST_FunctionDefinition *alreadyDefinedFun = NULL;
//            printf("%p Binding for var '%s' with context %p, locus %d\n",this,name->name->c_str(),  context,locus);
            switch (locus) {
                case AST_VariableDeclarationLocusBlock:
                case AST_VariableDeclarationLocusForLoopSecondCounter:
                    context->locallyDeclaredVariables.push_back(this);
                case AST_VariableDeclarationLocusForLoop:
                case AST_VariableDeclarationLocusArgument:
                    if(type->tokenType != TYPE_DEF_FUNCTION)
                    {
                        map<std::string,AST_VariableDeclaration *>::iterator it = context->variablesDefinedInContext.find(*(name->name));
                      

                        if(it != context->variablesDefinedInContext.end())
                        {
                            errorMessage("Variable '%s' already defined in %d-%d.",name->name->c_str(),alreadyDefinedVar->type->lineNumber,alreadyDefinedVar->type->linePosition);
                        }
                        context->variablesDefinedInContext[*(name->name)] = this;
                        assert(context->variablesDefinedInContext[*(name->name)]);
                      //  printf("Declaring variable '%s' of type %d in context %p\n",name->name->c_str(),locus,context);
                    }
                    return NULL;
                    break;
                default:
                    printf("%d-%d Error - this locus type for variable declaration '%s' is not yet supported.",type->lineNumber,type->linePosition,name->name->c_str());
                    exit(1);
                    break;
            }
        }
            break;
        case 4:
        {  /* Binds function pointers */
            AST_VariableDeclaration *alreadyDefinedVar = NULL;
            AST_FunctionDefinition *alreadyDefinedFun = NULL;
            switch (locus) {
                case AST_VariableDeclarationLocusForLoop:
                case AST_VariableDeclarationLocusArgument:
                    if(type->tokenType == TYPE_DEF_FUNCTION)
                    {
                        alreadyDefinedFun = context->functionsDefinedInContext.find(*(name->name)) != context->functionsDefinedInContext.end()? context->functionsDefinedInContext.find(*(name->name))->second : NULL;
                        if(alreadyDefinedFun) {
                            errorMessage("Function '%s' already defined in %d-%d.",name->name->c_str(),alreadyDefinedVar->type->lineNumber,alreadyDefinedVar->type->linePosition);
                        }
                       alreadyDefinedFun = context->functionsDefinedInContext.find(functionNameForFunctionPointer) != context->functionsDefinedInContext.end()? context->functionsDefinedInContext.find(functionNameForFunctionPointer)->second : NULL;
                      
                        if(!alreadyDefinedFun)
                        {
                            this->errorMessage("Function %s not defined",functionNameForFunctionPointer.c_str());
                        }
                      
                      primaryContext.addFunctionDefinedInContext(alreadyDefinedFun,*(name->name));
                    }
                    break;
                default:
                    break;
            }
        }
        return NULL;
        default:
            break;
            
    }
    return NULL;
    
}





AST_VariableDeclaration::AST_VariableDeclaration(const char *name,int type, int smartDefineNumber,std::string className)
{
    HDParseSymbol *p = new HDParseSymbol();
    p->tokenType = type;
    typ = TYPE_VAR_DECLARATION;
    AST_Identifier *i = new AST_Identifier(name);
    this->type = p;
    this->sizes = NULL;
    this->name = i;
    this->defaultValue = NULL;
    this->locus = AST_VariableDeclarationLocusUnknown;
    this->smartDefineNumber = smartDefineNumber;
    this->className = className;
    

    
    switch(this->type->tokenType)
    {
        case TYPE_DEF_STRING:
            if(sizes) this->className = "HDlist";
            else this->className = "HDstring";
            break;
        case TYPE_DEF_DECIMAL:
        case TYPE_DEF_INTEGER:
            if(sizes) this->className = "HDarray";
            break;
        case TYPE_DEF_LIST:
            this->className = "HDlist";
            break;
    }
    //printf("Created var %d decl with class: '%s'\n",this->type->tokenType,this->className.c_str());
}



AST_VariableDeclaration::AST_VariableDeclaration(AST_Identifier *name,HDParseSymbol *t,std::vector<AST_Expression*> *sizes,AST_Expression *defaultValue,AST_VariableDeclarationLocus locus,std::string className) : AST_Statement(t)
{
    typ = TYPE_VAR_DECLARATION;
    this->type = t;
    this->sizes = sizes;
    this->name = name;
    this->defaultValue = defaultValue;
    this->locus = locus;
    
    children.push_back(name);
//    if(strcmp("i",name->name->c_str())==0) printf("defined variable %s type %d addr %p\n",name->name->c_str(),type->tokenType,this);
    
    if(defaultValue) children.push_back(defaultValue);
    
    if(sizes)
        for(std::vector<AST_Expression *>::iterator i = sizes->begin();i!=sizes->end();i++)
            children.push_back(*i);
    this->className = className;
    
    switch(type->tokenType)
    {
        case TYPE_DEF_STRING:
            if(sizes) this->className = "HDlist";
            else this->className = "HDstring";
            break;
        case TYPE_DEF_DECIMAL:
        case TYPE_DEF_INTEGER:
            if(sizes) this->className = "HDarray";
            break;
        case TYPE_DEF_LIST:
            this->className = "HDlist";
            break;


    }
   // printf("Created var %d decl with class: '%s' pointer %p \n",this->type->tokenType,this->className.c_str(),this);
     //printf("Created var decl %s with class: '%s'\n",name->name->c_str(),  className.c_str());
}




std::string AST_VariableDeclaration::constructOutputString(HDExecutionContext *context)
{
  if(!defaultValue) return std::string("");
  AST_Substitution *subs = new AST_Substitution(name,defaultValue,&compilerContext);
  std::string out = subs->constructOutputString(context);
  subs->left = NULL;
  subs->right = NULL;
  /* what about children - it is safe to forget about them in current setup, butr beware in the future */
  delete subs;
  return out;
}


std::string AST_VariableDeclaration::constructDeclarationString(HDExecutionContext *context)
{
    switch (locus) {
        case AST_VariableDeclarationLocusBlock:
            return constructOutputStringForBlockLocus(context);
            break;
        case AST_VariableDeclarationLocusArgument:
        case AST_VariableDeclarationLocusReturnList:
        case AST_VariableDeclarationLocusClassVariable:
            return constructOutputStringForArgumentLocus(context);
            break;
        case AST_VariableDeclarationLocusForLoopSecondCounter:
            break;
        default:
            printf("%d-%d Error - this locus type for variable declaration is not yet supported.",type->lineNumber,type->linePosition);
            exit(1);
            
            break;
    }
    return std::string("");
}


std::string AST_VariableDeclaration::typeString(HDExecutionContext *context,std::string n,AST_Node *motherObject)
{
    std::string out;
    switch(type->tokenType)
    {
        case TYPE_DEF_VOID:
            motherObject->errorMessage("cannot declare void-type variable %s",n.c_str());
            break;
        case TYPE_DEF_STRING:
            if(sizes)
            {
                if(sizes->size() > 1)
                {
                    motherObject->errorMessage("This version of Hodor allows string arrays to have at most one dimension: %s",n.c_str());
                }
                out+="  HDList<"+context->floatingPointTypeName+",char> ";
            }
            else
            {
                out+="  HDStringEnvelope<"+context->floatingPointTypeName+",char> ";
            }
            
            break;
        case TYPE_DEF_DECIMAL:
            if(sizes == NULL) out+="  "+context->floatingPointTypeName+" ";
            else out+= "  HDTensor<"+context->floatingPointTypeName+"> ";
            break;
        case TYPE_DEF_INTEGER:
            if(sizes == NULL) out+="  int ";
            else out+= "  HDTensor<int> ";
            break;
        case TYPE_DEF_LIST:
            if(sizes) {
                motherObject->errorMessage("Error - Arrays of lists not supported: %s",n.c_str());
            }
            out+="  HDList<"+context->floatingPointTypeName+",char> ";
            break;
        case TYPE_DEF_DICTIONARY:
            if(sizes) {
                motherObject->errorMessage("Arrays of dictionaries not supported: %s",n.c_str());
            }
            
            out+="  HDDict ";
            break;
        default:
            motherObject->errorMessage("Unallowed variable type.");
    }

    return out;
}



std::string AST_VariableDeclaration::constructOutputStringForBlockLocus(HDExecutionContext *context)
{
    std::string out = "";
    std::string n;
    //printf("Deklaracja zmiennej o nazwie ");
    n=name->constructOutputString(context);
    //printf("%s ",n.c_str());
    out+= typeString(context,n,this)+n;
    if(sizes && sizes->size())
    {
        /* special case - size cloning */
        bool sizesFilled = false;
        if(sizes->size() == 1)
        {
            AST_Expression *e = *(sizes->begin());
            AST_VariableDeclaration *expressionType = e->computeFinalType(context);
            if(expressionType->sizes && e->compilerContext.tokenType == TYPE_IDENTIFIER)
            {
                out+="(\"c\","+e->constructOutputString(context)+")";
                sizesFilled = true;
            }
        }
        if(!sizesFilled)
        {
        out+=string("(")+static_cast<ostringstream*>( &(ostringstream() << sizes->size()) )->str()+string(", ");
        for(vector<AST_Expression *>::iterator it = sizes->begin();it!=sizes->end();it++)
        {
            AST_Expression *e = *it;
            AST_VariableDeclaration *expressionType = e->computeFinalType(context);
          if(!intVar) intVar = new AST_VariableDeclaration("Integer var requirement for declaration",TYPE_DEF_INTEGER);
          
//            if(expressionType->type->tokenType != TYPE_DEF_INTEGER || expressionType->sizes) {
//                printf("%d-%d Error - Array sizes can be only integer numbers. %s:.",type->lineNumber,type->linePosition,n.c_str());
//                exit(1);
//            }
          
            out+=context->constructSubstitutionString(intVar,e,"variable declaration");
            if(it+1!=sizes->end()) out+=", ";
        }
        out += ")";
        }
    }
    
    out+=";\n";
    return out;
}


std::string AST_VariableDeclaration::constructOutputStringForArgumentLocus(HDExecutionContext *context)
{
    std::string out = "";
    std::string n;
    //printf("Deklaracja argumentu funkcji o nazwie ");
    n=name->constructOutputString(context);
    //printf("%s ",n.c_str());
    
    switch(type->tokenType)
    {
        case TYPE_DEF_VOID:
            out+="HDMemoryModel &";
//            printf("%d-%d Error - cannot declare void-type argument %s:.",type->lineNumber,type->linePosition,n.c_str());
//            exit(1);
            break;
        case TYPE_DEF_STRING:
            if(sizes)
            {
                if(sizes->size() > 1)
                {
                    printf("%d-%d Error - This version of Hodor allows string arrays to have at most one dimension. %s:.",type->lineNumber,type->linePosition,n.c_str());
                    exit(1);
                }
                out+="HDList<"+context->floatingPointTypeName+",char> ";
            }
            else
            {
                out+="HDStringEnvelope<"+context->floatingPointTypeName+",char> ";
            }
            
            break;
        case TYPE_DEF_DECIMAL:
            if(sizes == NULL) out+=context->floatingPointTypeName+" ";
            else out+= "HDTensor<"+context->floatingPointTypeName+"> ";
            break;
        case TYPE_DEF_INTEGER:
            if(sizes == NULL) out+="int ";
            else out+= "HDTensor<int> ";
            break;
        case TYPE_DEF_LIST:
            if(sizes) {
                printf("%d-%d Error - Arrays of lists not supported. %s:.",type->lineNumber,type->linePosition,n.c_str());
                exit(1);
            }
            out+="HDList<"+context->floatingPointTypeName+",char> ";
            break;
        case TYPE_DEF_DICTIONARY:
            if(sizes) {
                printf("%d-%d Error - Arrays of dictionaries not supported. %s:.",type->lineNumber,type->linePosition,n.c_str());
                exit(1);
            }
            
            out+="HDDict ";
            break;
        case TYPE_DEF_FUNCTION:
        {
          AST_FunctionDefinition *fun = context->functionsDefinedInContext.find(functionNameForFunctionPointer)!=context->functionsDefinedInContext.end() ? context->functionsDefinedInContext.find(functionNameForFunctionPointer)->second : NULL;
            if(!fun)
            {
                this->errorMessage("You have to call function '%s' at least once for type system to infer function argument type",n.c_str());
            }
            
            out+=fun->constructDefinitionWithName("(*HDFunction__"+n+")",context);
            
            
        }
            break;
            
        default:
            printf("%d-%d Error - unallowed variable type.",type->lineNumber,type->linePosition);
            exit(1);
    }
    if(type->tokenType != TYPE_DEF_FUNCTION)
                out+=n;
    if(sizes && sizes->size())
    {
        for(vector<AST_Expression *>::iterator it = sizes->begin();it!=sizes->end();it++)
        {
            AST_Expression *e = *it;
            AST_VariableDeclaration *expressionType = e->computeFinalType(context);
            if(expressionType->type->tokenType != TYPE_DEF_INTEGER || expressionType->sizes) {
                printf("%d-%d Error - Array sizes can be only integer numbers. %s:.",type->lineNumber,type->linePosition,n.c_str());
                exit(1);
            }
        }
    }
    /* default vaule not yet supported */
/*    if(defaultValue)
    {
        AST_VariableDeclaration *expressionType = defaultValue->computeFinalType(context);
        if(expressionType->type->tokenType != type->tokenType) {
            printf("%d-%d Error - Incorrect types in substitution. %s:.",type->lineNumber,type->linePosition,n.c_str());
            exit(1);
        }
        
        
    }*/
   // printf("I typie %s\n",type->value->c_str());
    return out;
}







AST_VariableDeclaration::~AST_VariableDeclaration()
{
    delete name; /* typ jest dealokowany w klasie bazowej */
    if(defaultValue) delete defaultValue;
    if(sizes)
    {
        for(std::vector<AST_Expression*>::iterator i = sizes->begin();i!=sizes->end();i++) delete (*i);
        delete sizes;
    }
}
