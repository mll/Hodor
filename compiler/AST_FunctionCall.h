//
//  AST_FunctionCall.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_FunctionCall__
#define __Hodor__AST_FunctionCall__

#include <iostream>
#include "AST_Expression.h"
#include <vector>
#include "AST_Identifier.h"
#include "AST_FunctionDefinition.h"

class AST_FunctionCall : public AST_Expression
{
public:
    AST_Identifier *ident;
    std::vector<AST_Expression*> *args;
    AST_Expression *objectOfCall;
    
    AST_FunctionDefinition *getFunctionDefinition(HDExecutionContext *context);
    
    AST_FunctionCall(AST_Identifier *id, std::vector<AST_Expression*> *args,AST_Expression *objectOfCall = NULL);
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output); /* returns context for deeper progress */
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); 
    virtual ~AST_FunctionCall();
};



#endif /* defined(__Hodor__AST_FunctionCall__) */
