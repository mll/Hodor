//
//  AST_Statement.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Statement__
#define __Hodor__AST_Statement__

#include <iostream>
#include "AST_Node.h"

class AST_Statement :public  AST_Node
{
public:
    AST_Statement() {}
    AST_Statement(HDParseSymbol *s);
    AST_Statement(AST_Node *s);
    virtual ~AST_Statement();
};


#endif /* defined(__Hodor__AST_Statement__) */
