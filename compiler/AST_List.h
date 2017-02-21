//
//  AST_List.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_List__
#define __Hodor__AST_List__

#include <iostream>
#include <vector>
#include "AST_Expression.h"

class AST_List : public AST_Expression
{
public:
    AST_List(std::vector<AST_Expression*> *args);
    std::vector<AST_Expression*> *args;
    
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_List();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_List__) */
