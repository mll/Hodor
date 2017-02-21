//
//  AST_Decimal.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Decimal.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *var = NULL;

std::string string_format(const std::string fmt_str, ...) {
    int final_n, n = fmt_str.size() * 2; /* reserve 2 times as much as the length of the fmt_str */
    std::string str;
    char * formatted;
    va_list ap;
    while(1) {
        formatted = new char[n]; /* wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    std::string retVal = std::string(formatted);
    delete formatted;
    return retVal;
}


AST_Decimal::AST_Decimal(HDParseSymbol *s) : AST_Numeric(s)
{
    char *verification = 0;
    std::replace( s->value->begin(), s->value->end(), 'd', 'e');
    std::replace( s->value->begin(), s->value->end(), 'D', 'e');
  
  
  
    this->value = strtod(s->value->c_str(),&verification);
    if(*verification){
        errorMessage("Syntax error: %s!",s->value->c_str());
    }
    typ = TYPE_DECIMAL;
}

std::string AST_Decimal::constructOutputString(HDExecutionContext *context)
{
//    printf("Mamy do czynienia z liczba:'%f'\n",value);
  if(context->floatingPointTypeName.compare(std::string("float"))==0)
    return string_format("%.8e",value);
  else
    return string_format("%.16e",value);
}

AST_VariableDeclaration *AST_Decimal::computeFinalType(HDExecutionContext *context)
{
    if(!var) var = new AST_VariableDeclaration("HD internal decimal",TYPE_DEF_DECIMAL);
    return var;
}



AST_Decimal::~AST_Decimal()
{
    
}