//
//  AST_InTest.h
//  Hodor
//
//  Created by Test on 23.11.2013.
//
//

#ifndef __Hodor__AST_InTest__
#define __Hodor__AST_InTest__

#include <iostream>
#include "AST_Expression.h"

class AST_InTest :public  AST_Expression
{
public:
    AST_Expression *left;
    AST_Expression *right;
    
    AST_InTest(AST_Expression *left,AST_Expression *right);
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */
    
    virtual ~AST_InTest();
};


#endif /* defined(__Hodor__AST_InTest__) */
