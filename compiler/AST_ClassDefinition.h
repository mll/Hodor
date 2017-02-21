//
//  AST_ClassDefinition.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_ClassDefinition__
#define __Hodor__AST_ClassDefinition__

#include <iostream>
#include "AST_Definition.h"
#include <vector>
#include "AST_Identifier.h"
#include "AST_Block.h"

class AST_ClassDefinition : public AST_Definition
{
public:

    AST_Identifier *ident;
    std::vector<AST_Identifier *> *inherits;
    
    std::vector<AST_VariableDeclaration *> *fields;
    AST_Definitions *methods;
    virtual std::string defName();
    
    
    AST_ClassDefinition(AST_Identifier *id, std::vector<AST_Identifier*> *inherits,std::vector<AST_Node *> *body);
    virtual std::string constructOutputString(HDExecutionContext *context);
    
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output); /* returns context for deeper progress */

    virtual ~AST_ClassDefinition();
};


#endif /* defined(__Hodor__AST_ClassDefinition__) */
