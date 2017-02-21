//
//  AST_Integer.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Integer.h"
#include "string.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *var = NULL;


AST_Integer::AST_Integer(HDParseSymbol *s) :AST_Numeric(s)
{
    char *verification = 0;
    this->value = strtol(s->value->c_str(),&verification,10);
    if(*verification){
        printf("%d-%d: Syntax error. Wrong number literal %s",s->lineNumber,s->linePosition,s->value->c_str());
    }
    typ = TYPE_INTEGER;
}

std::string AST_Integer::constructOutputString(HDExecutionContext *context)
{
  //  printf("Mamy do czynienia z liczba:'%d'\n",value);
    ostringstream con;
    con << value;
    return con.str();
}

AST_Integer::~AST_Integer()
{
    
}

AST_VariableDeclaration *AST_Integer::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HD internal int",TYPE_DEF_INTEGER);
    return var;
}
