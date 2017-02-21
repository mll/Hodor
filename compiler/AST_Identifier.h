//
//  AST_Identifier.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Identifier__
#define __Hodor__AST_Identifier__

#include <iostream>
#include "AST_Expression.h"


class AST_Identifier : public AST_Expression
{
public:
    bool smartDefinesEnabled;
    std::string *name;
    
    bool operator!=(const AST_Identifier &op);
    bool operator==(const AST_Identifier &op);

    AST_Identifier(HDParseSymbol *s);
    AST_Identifier(const char *name);
    virtual std::string constructOutputString(HDExecutionContext *context);
    std::string constructSmartOutputString(HDExecutionContext *context);
    virtual ~AST_Identifier();
    virtual AST_VariableDeclaration *computeFinalType(HDExecutionContext *context); /* returned construct is borrowed for the caller */
    AST_VariableDeclaration *computeSmartType(HDExecutionContext *context); /* returned construct is borrowed for the caller */
    AST_Identifier &operator=(const AST_Identifier& c);
    AST_Identifier(const AST_Identifier &c);
    
};


#endif /* defined(__Hodor__AST_Identifier__) */
