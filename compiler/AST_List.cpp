//
//  AST_List.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_List.h"
#include "AST_VariableDeclaration.h"
#include <string>
static AST_VariableDeclaration *var = NULL;

AST_List::AST_List(std::vector<AST_Expression*> *args)
{
    this->args = args;
    typ = TYPE_LIST;
    for(std::vector<AST_Expression *>::iterator i = args->begin();i!=args->end();i++) children.push_back(*i);
}




std::string AST_List::constructOutputString(HDExecutionContext *context)
{
    std::string out = "HDList<"+context->floatingPointTypeName+",char>(";
    out+=static_cast<ostringstream*>( &(ostringstream() << args->size()) )->str();
    if(args->size()) out+=", ";
    for(std::vector<AST_Expression*>::iterator i = args->begin();i!=args->end();i++)
    {
        AST_Expression *expr = *i;
        AST_VariableDeclaration *type = expr->computeFinalType(context);
        switch(type->type->tokenType)
        {
            case TYPE_DEF_INTEGER:
                if(type->sizes == NULL)
                out+="HDPrimitiveEnvelope<int>("+expr->constructOutputString(context)+",HDTypeInteger).createWeakCopy()";
                else out+=expr->constructOutputString(context)+".createWeakCopy()";
                break;
            case TYPE_DEF_DECIMAL:
                if(type->sizes == NULL)
                out+="HDPrimitiveEnvelope<"+context->floatingPointTypeName+" >("+expr->constructOutputString(context)+",HDTypeDecimal).createWeakCopy()";
                else out+=expr->constructOutputString(context)+".createWeakCopy()";
                break;
            default:
                out+=expr->constructOutputString(context)+".createWeakCopy()";
                break;
        }
        if(i+1!=args->end()) out+=", ";
    }
    out+=")";
    
    return out;
}


AST_List::~AST_List()
{
    delete args;
}

AST_VariableDeclaration *AST_List::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HD internal list",TYPE_DEF_LIST);
    return var;
}
