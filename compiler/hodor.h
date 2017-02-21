#ifndef _WEZLY
#define _WEZLY
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <list>


using namespace std;


#define TYPE_BLOCK              1000001
#define TYPE_INTERNAL_SUBS      1000002
#define TYPE_EXPR_INSTR         1000003
#define TYPE_BINARY_OPERATOR    1000004
#define TYPE_EXPR_IDENT         1000005
#define TYPE_UNARY_OPERATOR     1000006
#define TYPE_FUN_DEFINITION     1000007
#define TYPE_FUNCTION_CALL      1000008
#define TYPE_LIST               1000009
#define TYPE_DICTIONARY         1000010
#define TYPE_DICT_EXPR          1000011
#define TYPE_ACCESS             1000012
#define TYPE_DECLARATIONS       1000013
#define TYPE_VAR_DECLARATION    1000014
#define TYPE_RUNTIME            1000015
#define TYPE_STATEMENT_RETURN   1000016
#define TYPE_CONDITIONAL_EXPRESSION   1000017
#define TYPE_CLASS_NAME         1000018

class AST_FunctionCall;
class AST_VariableDeclaration;
class AST_IdentifierExpression;
class AST_BinaryOperator;
class AST_UnaryOperator;
class AST_ExpressionStatement;
class AST_Identifier;
class AST_Substitution;
class AST_ReturnStatement;
class AST_Statement;
class AST_Expression;
class AST_Block;
class AST_Node;
class AST_DictionaryExpression;
class AST_ArrayAccess;
class AST_FunctionDefinition;
class AST_Definitions;
class AST_Definition;
class AST_String;
class AST_Decimal;
class AST_Numeric;
class AST_Integer;
class AST_List;
class AST_Dictionary;
class AST_IfStatement;
class AST_ForStatement;
class AST_InTest;
class AST_ArrayAccessExpression;
class AST_ConditionalExpression;
class AST_WhileStatement;

class HDParseSymbol
{
  public:
  int tokenType; 
  int lineNumber;
  int linePosition;
  std::string *lineText;
  std::string *symbolText;
  std::string *value;
  HDParseSymbol()
  {
   tokenType = -1;
   lineNumber = -1;
   linePosition = -1;
   lineText = NULL;
   symbolText = NULL;
   value = NULL;
  }
    
    HDParseSymbol(int typs)
    {
        tokenType = typs;
        lineNumber = -1;
        linePosition = -1;
        lineText = NULL;
        symbolText = NULL;
        value = NULL;
    }

    
  ~HDParseSymbol()
  {
   if(lineText) delete lineText;
   if(symbolText) delete symbolText;
   if(value) delete value;
  }
  
  HDParseSymbol &operator=(const HDParseSymbol &o)
  {
    this->tokenType = o.tokenType;
    this->lineNumber = o.lineNumber;
    this->linePosition = o.linePosition;
    if(lineText) delete this->lineText;
    if(symbolText) delete this->symbolText;
    if(value) delete this->value;
    this->lineText = o.lineText?new std::string(*(o.lineText)):NULL;
    this->symbolText = o.symbolText?new std::string(*(o.symbolText)):NULL;
    this->value =o.value? new std::string(*(o.value)):NULL;
    return *this;
  }
  
};


struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
    {
        printf("Comparison %s and %s equals %d",s1,s2,strcmp(s1, s2) < 0);
        return strcmp(s1, s2) < 0;
    }
};

void print_trace (void);


class AST_VariableDeclaration;
class AST_FunctionDefinition;
class AST_ClassDefinition;


class HDMetaContext
{
public:
    map<std::string,AST_VariableDeclaration *> smartDefines;
    int highestSmartDefineTaken;
    static HDMetaContext &sharedContext()
    {
        static HDMetaContext *mt = NULL;
        if(!mt) {
            mt = new HDMetaContext();
            mt->highestSmartDefineTaken = 1;
        }
        return *mt;
    }
};


class HDExecutionContext 
{
  public:
    
    
    map<std::string,AST_VariableDeclaration *> variablesDefinedInContext;
    list<AST_VariableDeclaration *> locallyDeclaredVariables;
    multimap<std::string,AST_FunctionDefinition *> functionsDefinedInContext;
    map<std::string,AST_ClassDefinition *> classesDefinedInContext;
  
    AST_FunctionDefinition *encompassingFunction;
    string floatingPointTypeName;
  
    void addFunctionDefinedInContext(AST_FunctionDefinition *fun, std::string name);
  
    vector<HDExecutionContext *> children;
    HDExecutionContext &operator=(HDExecutionContext &k);
    
    std::string constructSubstitutionString(AST_VariableDeclaration *expected, AST_Expression *got,const char *callType);
    bool checkVariableApplicability(AST_VariableDeclaration *expected, AST_Expression *got);
    
    AST_FunctionDefinition *findMatchingFunction(std::string name,AST_FunctionDefinition *fun);
    AST_FunctionDefinition *findMatchingFunction(std::string name,AST_FunctionCall *fun);

    
};



#endif