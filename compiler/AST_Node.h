//
//  AST_Node.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Node__
#define __Hodor__AST_Node__

#include <iostream>
#include "hodor.h" 
#include "parser.hpp"

class AST_Node
{
public:
    HDParseSymbol compilerContext;
    HDExecutionContext executionContext;
    /* This vector does hold only references, not ownerships. We do not delete those */
    vector<AST_Node *> children;
    int typ;
    AST_Node();
    
    AST_Node(AST_Node *s);
    AST_Node(HDParseSymbol *s);
    
    /* utils */
    void errorMessage(const char *format,...);
    std::string intToString(int a);
    
    std::string compilerPassWithContext(int passNo,HDExecutionContext *context);
    
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output); /* returns context for deeper progress */
    std::string classNameFromAbi();
    
    
    virtual ~AST_Node();
    virtual std::string constructOutputString(HDExecutionContext *context);
    virtual std::string constructOutputString(HDExecutionContext *context,int forcedReturnType);
};




#endif /* defined(__Hodor__AST_Node__) */
