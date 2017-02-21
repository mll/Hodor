//
//  AST_Definitions.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Definitions__
#define __Hodor__AST_Definitions__

#include <iostream>
#include "AST_Node.h"
#include "AST_Definition.h"

class AST_Definitions : public AST_Node
{
public:
    vector<AST_Node *> definitions;
    AST_Definitions();
    virtual ~AST_Definitions();
    void addDefinition(AST_Definition *d);
    
    
    virtual std::string constructOutputString(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_Definitions__) */
