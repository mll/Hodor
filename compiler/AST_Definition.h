//
//  AST_Definition.h
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#ifndef __Hodor__AST_Definition__
#define __Hodor__AST_Definition__

#include <iostream>
#include "AST_Node.h"
class AST_Definition : public AST_Node
{
public:
    AST_Definition(HDParseSymbol *s);
    AST_Definition(AST_Node *s);
    virtual std::string defName() {return std::string();}
};


#endif /* defined(__Hodor__AST_Definition__) */
