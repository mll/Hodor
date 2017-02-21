//
//  AST_Definitions.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Definitions.h"
#include "AST_Definition.h"

AST_Definitions::AST_Definitions()
{
    typ = TYPE_DECLARATIONS;
}

void AST_Definitions::addDefinition(AST_Definition *d)
{
    assert(d);
    children.push_back(d);
    definitions.push_back(d);
}

std::string AST_Definitions::constructOutputString(HDExecutionContext *context)
{
   // printf("Mamy do czynienia ze zbiorem deklaracji, zawiera on %d deklaracji:\n",(int)definitions.size());
    std::string out = "";
    AST_Node *last = NULL;

    for(vector<AST_Node *>::iterator i=definitions.begin();i!=definitions.end();i++)
    {
        AST_Definition *d = dynamic_cast<AST_Definition *>(*i);
//        printf("Running def: '%s' name '%s'\n",d?d->classNameFromAbi().c_str():"none",d?d->defName().c_str():"none");
        out+=(*i)->constructOutputString(context)+"\n";
    }
  
    return out;
}








AST_Definitions::~AST_Definitions()
{
    for (std::vector<AST_Node*>::iterator i = definitions.begin();i!=definitions.end();i++) delete (*i);
}
