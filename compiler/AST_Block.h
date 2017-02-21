//
//  AST_Block.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Block__
#define __Hodor__AST_Block__

#include <iostream>
#include "AST_Node.h"
#include "AST_Statement.h"
#include <vector>
#include "AST_FunctionDefinition.h"
class AST_Block : public AST_Node
{
public:
    vector<AST_Statement *> statements;
    AST_Block();
    
    virtual HDExecutionContext *applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */;
    void addStatement(AST_Statement *s);
    
    virtual ~AST_Block();
    virtual std::string constructOutputString(HDExecutionContext *context);
};


#endif /* defined(__Hodor__AST_Block__) */
