//
//  AST_ReturnStatement.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_ReturnStatement.h"
#include <assert.h>
#include "AST_VariableDeclaration.h"
#include "AST_FunctionDefinition.h"
#include <sstream>

static AST_VariableDeclaration *var = NULL;

AST_ReturnStatement::AST_ReturnStatement(HDParseSymbol *s,AST_Expression *w) : AST_Statement(s)
{
    expression = w;
    if(expression) children.push_back(expression);
    typ = TYPE_STATEMENT_RETURN;
}

AST_ReturnStatement::~AST_ReturnStatement()
{
    if(expression) delete expression;
}




std::string AST_ReturnStatement::constructOutputString(HDExecutionContext *context)
{
    std::string out;
    AST_FunctionDefinition *fun = context->encompassingFunction;
    
    if(!fun)
    {
        this->errorMessage("Function encompassing return statement not found!");
        
        
    }
    
    
    assert(fun);
    if(!var) var = new AST_VariableDeclaration("HD internal list",TYPE_DEF_LIST);
    var->locus = AST_VariableDeclarationLocusArgument;
    AST_VariableDeclaration *returnType = fun->returnType ?: (fun->returnList? var : NULL);
    assert(returnType);
    if(returnType->type->tokenType == TYPE_DEF_VOID) returnType = NULL;

    
    if(expression && returnType == NULL)
    {
        printf("%d-%d Error - Function with void return type got %s instead ",compilerContext.lineNumber,compilerContext.linePosition,returnType->constructOutputString(context).c_str());
        exit(1);
    }
    
    
    
    if(expression)
    {
        if(returnType == var)
        {
            map<std::string,int> check;
            out+="HDList<"+context->floatingPointTypeName+",char> listToVerify = "+context->constructSubstitutionString(returnType,expression,"return statement")+";\n";
            out+="  listToVerify.verifyAndAnnotateOperands("+static_cast<ostringstream*>( &(ostringstream() << fun->returnList->size()) )->str()+",";
            for(vector<AST_VariableDeclaration *>::iterator it = fun->returnList->begin();it!=fun->returnList->end();it++)
            {

                if(check[*((*it)->name->name)])
                {
                    printf("%d-%d Error - Return variable with name %s already exists ",compilerContext.lineNumber,compilerContext.linePosition,(*it)->name->name->c_str());
                    exit(1);
                }
                check[*((*it)->name->name)] = 1;
                
                string typeName;
                switch((*it)->type->tokenType)
                {
                    case TYPE_DEF_LIST:
                        typeName = "HDTypeList";
                        break;
                    case TYPE_DEF_DECIMAL:
                        if((*it)->sizes)  typeName ="HDTypeTensor";
                            else
                        typeName = "HDTypeDecimal";
                        break;
                    case TYPE_DEF_INTEGER:
                        typeName = "HDTypeInteger";
                        break;
                    case TYPE_DEF_STRING:
                        typeName = "HDTypeString";
                        break;
                    default:
                        printf("%d-%d Error - Wrong type %d for return list",compilerContext.lineNumber,compilerContext.linePosition,(*it)->type->tokenType);
                        exit(1);
                }
                
                
                out+="\""+*((*it)->name->name)+"\","+typeName;
                if(it+1!=fun->returnList->end()) out+=",";
            }
            out+=");\n";
            
            out += "  return listToVerify;\n";
        } else {
            out+="  return ";
            out+=context->constructSubstitutionString(returnType,expression,"return statement")+";\n";
        }
        
        
        return out;

    }
    else
    {
        if(returnType != NULL) {
            printf("%d-%d Error - Return statement expected type %s and instead got nothing ",compilerContext.lineNumber,compilerContext.linePosition,returnType->constructOutputString(context).c_str());
            exit(1);
        }
    }
    
    return "return;\n";
}


