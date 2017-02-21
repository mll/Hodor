//
//  AST_VariableDeclaration.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_VariableDeclaration__
#define __Hodor__AST_VariableDeclaration__

#include <iostream>
#include <vector>
#include "AST_Expression.h"
#include "AST_Statement.h"
#include "AST_Identifier.h"

enum AST_VariableDeclarationLocus
{
    AST_VariableDeclarationLocusUnknown = 0,
    AST_VariableDeclarationLocusBlock,
    AST_VariableDeclarationLocusArgument,
    AST_VariableDeclarationLocusForLoop,
    AST_VariableDeclarationLocusForLoopSecondCounter,
    AST_VariableDeclarationLocusReturnList, /* those two not yet supported */
    AST_VariableDeclarationLocusClassVariable
};


class AST_VariableDeclaration : public AST_Statement
{
public:
    HDParseSymbol *type;
    AST_Identifier *name;
    std::vector<AST_Expression*> *sizes;
    AST_Expression *defaultValue;
    AST_VariableDeclarationLocus locus;
    
    std::string functionNameForFunctionPointer;
    std::string typeString(HDExecutionContext *context,std::string n,AST_Node *motherObject);
    std::string className;
    
    int smartDefineNumber;
    
    bool operator==(const AST_VariableDeclaration &op);
    bool operator!=(const AST_VariableDeclaration &op);
    
    AST_VariableDeclaration();
    AST_VariableDeclaration(const char *name,int type, int smartDefineNumber = 0,std::string className = "");
    
    AST_VariableDeclaration(AST_Identifier *name,HDParseSymbol *t,std::vector<AST_Expression*> *sizes,AST_Expression *defaultValue,AST_VariableDeclarationLocus locus,std::string className = "");
    
    virtual ~AST_VariableDeclaration();
    
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output);
    
    virtual std::string constructOutputString(HDExecutionContext *context);
    std::string constructDeclarationString(HDExecutionContext *context);
  
    std::string constructOutputStringForBlockLocus(HDExecutionContext *context);
    std::string constructOutputStringForArgumentLocus(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_VariableDeclaration__) */
