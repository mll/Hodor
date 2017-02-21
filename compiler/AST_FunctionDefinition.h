//
//  AST_FunctionDefinition.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_FunctionDefinition__
#define __Hodor__AST_FunctionDefinition__

#include <iostream>
#include "AST_Definition.h"
#include "AST_Block.h"
#include "AST_VariableDeclaration.h"
#include "AST_Identifier.h"
#include "AST_ClassDefinition.h"

class AST_FunctionCall;

class AST_FunctionDefinition : public AST_Definition
{
public:
    AST_Identifier *ident;
    AST_VariableDeclaration *returnType;
    bool isExternalCppFunction;
    std::vector<AST_VariableDeclaration*> *args;
    std::vector<AST_VariableDeclaration*> *returnList;
    AST_Block *cialo;
    
    std::string encompassingClass;
    
    virtual std::string defName();
    
    bool functionCallMatches(const AST_FunctionCall &op,HDExecutionContext *context);
    
    bool operator==(const AST_FunctionDefinition &op);
    bool operator!=(const AST_FunctionDefinition &op);
    
    AST_FunctionDefinition(AST_Identifier *id, std::vector<AST_VariableDeclaration*> *args,AST_Block *cialo,AST_VariableDeclaration *returnType, std::vector<AST_VariableDeclaration*> *returnList = NULL);
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output);
    std::string constructDefinitionString(HDExecutionContext *context);
    std::string constructDefinitionWithName(std::string name,HDExecutionContext *context);
    
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual ~AST_FunctionDefinition();
};


#endif /* defined(__Hodor__AST_FunctionDefinition__) */
