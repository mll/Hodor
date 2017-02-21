//
//  AST_Node.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_Node.h"
#include "AST_FunctionCall.h"
#include "AST_ArrayAccess.h"
#include "AST_FunctionDefinition.h"
#include <stdarg.h>
#include <sstream>
#include <cxxabi.h>
#include <execinfo.h>
#include <unistd.h>
#include <string.h>



std::string AST_Node::classNameFromAbi()
{
    int status;
    char * demangled = abi::__cxa_demangle(typeid(*this).name(),0,0,&status);
    std::string str(demangled);
    free(demangled);
    return str;
}


std::string AST_Node::intToString(int a)
{
    return static_cast<ostringstream*>( &(ostringstream() <<  a  ) )->str();
}

std::string AST_Node::compilerPassWithContext(int passNo,HDExecutionContext *context)
{
    std::string out = "";
    HDExecutionContext *retContext = applyCompilerPassWithContext(passNo,context,&out) ? : context;
    for(vector<AST_Node *>::iterator it = children.begin();it!=children.end();it++)
    {
        if(*it) out+= (*it)->compilerPassWithContext(passNo,retContext);
        else {
            errorMessage("Warning - child is NULL for %s class\n",classNameFromAbi().c_str());
        }
    }
    return out;
}

HDExecutionContext *AST_Node::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output)
{
      /* this is to be overloaded */
    return NULL;
}




void AST_Node::errorMessage(const char *format,...)
{
    printf("%d-%d Error: ",compilerContext.lineNumber,compilerContext.linePosition);
    va_list ap;
    va_start(ap,format);
    vprintf(format,ap);
    printf("\n");
    va_end(ap);
    
//    void *array[10];
//    size_t size;
//    char **strings;
//    size_t i;
//    size = backtrace (array, 10);
//    strings = (array, size);
//    printf ("***\nCompiler trace: %zd stack frames.\n", size);
//    for (i = 0; i < size; i++)
//        printf ("%s\n", strings[i]);
//    free (strings);

    exit(1);
}






AST_Node::AST_Node()
{
    
}

AST_Node::AST_Node(AST_Node *s)
{
    compilerContext = s->compilerContext;
  //  printf("%d-%d - created %d\n",compilerContext.lineNumber,compilerContext.linePosition,compilerContext.tokenType);
}


AST_Node::AST_Node(HDParseSymbol *s)
{
    compilerContext = *s;
   // printf("%d-%d - created %d\n",compilerContext.lineNumber,compilerContext.linePosition,compilerContext.tokenType);
}



AST_Node::~AST_Node()
{
}

std::string AST_Node::constructOutputString(HDExecutionContext *context)
{
   // printf("Mamy do czynienia z wezlem\n");
    return std::string();
}

std::string AST_Node::constructOutputString(HDExecutionContext *context,int forcedReturnType)
{
    printf("In construct output\n");
    fflush(stdout);
    return this->constructOutputString(context);
}
