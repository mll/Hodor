//
//  AST_Block.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Block.h"
#include "AST_VariableDeclaration.h"




HDExecutionContext *AST_Block::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */
{
    switch(passNo)
    {
        case 1:
            
        case 2:
            //printf("Block substituting for context: %p\n",&executionContext);
            executionContext = *context;

            break;
        case 5:
            
            break;
        default:
            break;
    }
    return &executionContext;
}



AST_Block::AST_Block()
{
    typ = TYPE_BLOCK;
}


void AST_Block::addStatement(AST_Statement *s)
{
    children.push_back(s);
    statements.push_back(s);
}

std::string AST_Block::constructOutputString(HDExecutionContext *contextZewnetrzny)
{
    std::string out = "{\n";
    
    /* Najpierw deklaracje zmiennych */
//    for(list<AST_VariableDeclaration *>::iterator i=executionContext.locallyDeclaredVariables.begin();i!=executionContext.locallyDeclaredVariables.end();i++)
//    {
//            AST_VariableDeclaration *d = (*i);
//            out+= d->constructDeclarationString(&executionContext);
//            std::string oo = (*i)->constructOutputString(&executionContext);
//            if(oo.length()) out+="  "+oo+";\n";
//    }

    for(vector<AST_Statement *>::iterator i=statements.begin();i!=statements.end();i++)
    {
        if((*i)->typ == TYPE_VAR_DECLARATION)
        {
          AST_VariableDeclaration *d = dynamic_cast<AST_VariableDeclaration *>(*i);
          assert(d);
          out+= d->constructDeclarationString(&executionContext);
        }
        std::string oo = (*i)->constructOutputString(&executionContext);
        if(oo.length()) out+="  "+oo+";\n";
    }
    out+="}\n";
    return out;
}

AST_Block::~AST_Block()
{
    for (std::vector<AST_Statement*>::iterator i = statements.begin();i!=statements.end();i++) delete (*i);
}

