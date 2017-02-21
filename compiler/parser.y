%{
    #include "nodes.h"
    #include <iostream>
    //#define YYDEBUG 1
    AST_Definitions *program; /* the top level root node of our final AST_ */
    bool hasErrors = false;
    extern int yylex();
    void yyerror(const char *s,...);

%}

%locations
%error-verbose 

/* Represents the many different ways we can access our data */
%union {
    AST_Node *node;
    AST_Block *block;
    AST_Expression *expression;
    AST_Numeric *numeric;
    AST_Statement *statement;
    AST_Identifier *identifier;
    AST_Definition *definition;
    AST_Definitions *definitions;
    AST_VariableDeclaration *variable_declaration;
    std::vector<AST_Expression*> *vector_expression;
    std::vector<AST_Node*> *vector_node;
    std::vector<AST_Identifier*> *vector_identifier;
    std::vector<AST_VariableDeclaration*> *vector_variable_declaration;
    std::vector<AST_ArrayAccessExpression*> *vector_access_range;
    HDParseSymbol *token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */

 
%token <token> TYPE_DEF_VOID TYPE_DEF_STRING TYPE_DEF_DECIMAL TYPE_DEF_INTEGER TYPE_DEF_LIST TYPE_DEF_DICTIONARY TYPE_KEYWORD_RETURN TYPE_DEF_FUNCTION TYPE_DEF_ITERATOR
%token <token> TYPE_IDENTIFIER TYPE_INTEGER TYPE_DECIMAL TYPE_STRING
%token <token> TYPE_SUBSTITUTION TYPE_LEFT_ROUND TYPE_RIGHT_ROUND 
%token <token> TYPE_TIMES TYPE_PLUS TYPE_MINUS TYPE_DIVIDED TYPE_POW TYPE_TENSOR_PRODUCT
%token <token> TYPE_EQUAL TYPE_NEQUAL TYPE_GREATER_T TYPE_SMALLER_T TYPE_SMALLER_EQUAL TYPE_GREATER_EQUAL
%token <token> TYPE_AND TYPE_OR TYPE_NOT
%token <token> TYPE_COMMA TYPE_COLON TYPE_SEMICOLON TYPE_AT TYPE_DOT TYPE_QUESTION_MARK
%token <token> TYPE_LEFT_BRACKET TYPE_RIGHT_BRACKET TYPE_LEFT_MUSTACHE TYPE_RIGHT_MUSTACHE
%token <token> TYPE_FUNCTION_DEF TYPE_DEF_CLASS
%token <token> TYPE_PLUS_EQUAL TYPE_MINUS_EQUAL TYPE_TIMES_EQUAL TYPE_DIVIDE_EQUAL
%token <token> TYPE_IF_KEYWORD TYPE_FOR_KEYWORD TYPE_IN_KEYWORD TYPE_ELSE_KEYWORD TYPE_WHILE_KEYWORD

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an identifier (defined by union type identifier) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <expression>  string function_call expression list /* slownik expression_slownikowe klasy */
%type <expression> numeric
%type <block> statements block
%type <definitions> definitions program
%type <definition> definition function_definition class_declaration
%type <statement> statement 
%type <variable_declaration> variable_declaration return_type_definition
%type <token> type_definition
%type <identifier> identifier;
%type <vector_identifier> vector_identifiers
%type <vector_variable_declaration> vector_variable_declaration return_list
%type <vector_expression> vector_expression /*vector_expression_slownikowe */
%type <statement> return_statement if_statement for_statement while_statement
%type <expression> in_test_expression conditional_expression
%type <expression> three_dots access_range array_access
%type <vector_access_range> vector_access_range
%type <vector_node> class_body

%right TYPE_SUBSTITUTION 
%right TYPE_PLUS_EQUAL
%right TYPE_MINUS_EQUAL
%right TYPE_TIMES_EQUAL
%right TYPE_DIVIDE_EQUAL

%left TYPE_QUESTION_MARK

%left TYPE_OR
%left TYPE_AND
%nonassoc TYPE_NOT
%left TYPE_NEQUALS TYPE_GREATER_EQUALS TYPE_SMALLER_EQUALS TYPE_GREATER_T TYPE_SMALLER_T TYPE_EQUAL
%left TYPE_PLUS TYPE_MINUS
%left TYPE_TIMES TYPE_DIVIDED
%left UNARY
%left TYPE_POW
%left BRACKETS


%start program
%%

program : definitions
{
    program = $1;
}
        ;

statements : statement { $$ = new AST_Block(); $$->addStatement($<statement>1); }
      | statements statement { $1->addStatement($<statement>2); }
      | { $$ = new AST_Block();}
      ;

variable_declaration : variable_declaration TYPE_SUBSTITUTION expression
{
    $1->defaultValue = $3;
    $$ = $1;
}


statement : variable_declaration TYPE_SEMICOLON { $$ = $1 } 

statement : return_statement TYPE_SEMICOLON {$$ = $1}

statement :  expression TYPE_SEMICOLON { $$ = new AST_ExpressionStatement($1); }

statement : for_statement | if_statement | while_statement

if_statement : TYPE_IF_KEYWORD TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND block TYPE_ELSE_KEYWORD block {$$ = new AST_IfStatement($3,$5,$7);}

if_statement : TYPE_IF_KEYWORD TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND statement TYPE_ELSE_KEYWORD block
{
    AST_Block *block = new AST_Block();
    block->addStatement($<statement>5);
    $$ = new AST_IfStatement($3,block,$7);
}

if_statement : TYPE_IF_KEYWORD TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND block TYPE_ELSE_KEYWORD statement
{
    AST_Block *block = new AST_Block();
    block->addStatement($<statement>7);
    $$ = new AST_IfStatement($3,$5,block);
}



if_statement : TYPE_IF_KEYWORD TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND statement TYPE_ELSE_KEYWORD statement
{
    AST_Block *block = new AST_Block();
    block->addStatement($<statement>5);
    
    AST_Block *block2 = new AST_Block();
    block2->addStatement($<statement>7);
    
    $$ = new AST_IfStatement($3,block,block2);
}

if_statement : TYPE_IF_KEYWORD TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND block {$$ = new AST_IfStatement($3,$5,NULL);}
if_statement : TYPE_IF_KEYWORD TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND statement
{
    AST_Block *block = new AST_Block();
    block->addStatement($<statement>5);
    $$ = new AST_IfStatement($3,block,NULL);
}


while_statement : TYPE_WHILE_KEYWORD TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND block {$$ = new AST_WhileStatement($3,$5);}

for_statement : TYPE_FOR_KEYWORD TYPE_LEFT_ROUND variable_declaration TYPE_COMMA variable_declaration TYPE_IN_KEYWORD expression TYPE_RIGHT_ROUND block { $$ = new AST_ForStatement($3,$7,$9,$5); }

for_statement : TYPE_FOR_KEYWORD TYPE_LEFT_ROUND variable_declaration TYPE_COMMA variable_declaration TYPE_IN_KEYWORD expression TYPE_RIGHT_ROUND statement
{
    AST_Block *block = new AST_Block();
    block->addStatement($9);
    
    $$ = new AST_ForStatement($3,$7,block,$5);
};



for_statement : TYPE_FOR_KEYWORD TYPE_LEFT_ROUND variable_declaration TYPE_IN_KEYWORD expression TYPE_RIGHT_ROUND block { $$ = new AST_ForStatement($3,$5,$7); }

for_statement : TYPE_FOR_KEYWORD TYPE_LEFT_ROUND variable_declaration TYPE_IN_KEYWORD expression TYPE_RIGHT_ROUND statement
{
    AST_Block *block = new AST_Block();
    block->addStatement($7);

    $$ = new AST_ForStatement($3,$5,block);
};

conditional_expression : expression TYPE_QUESTION_MARK expression TYPE_COLON expression { $$ = new AST_ConditionalExpression($1,$3,$5); }

definition : function_definition | class_declaration
     ;
definitions : definition { $$ = new AST_Definitions(); $$->addDefinition($<definition>1); }
      | definitions definition { $1->addDefinition($<definition>2); }
      | { $$ = new AST_Definitions();}

block : TYPE_LEFT_MUSTACHE statements TYPE_RIGHT_MUSTACHE { $$ = $2; }

type_definition : TYPE_DEF_INTEGER | TYPE_DEF_STRING | TYPE_DEF_DECIMAL | TYPE_DEF_LIST | TYPE_DEF_DICTIONARY | TYPE_DEF_VOID | TYPE_DEF_ITERATOR | TYPE_DEF_FUNCTION

//type_definition : identifier { $$ = new HDParseSymbol(TYPE_CLASS_NAME); $$->value = new std::string(*($1->name)); delete $1; }


return_type_definition : type_definition { $$ = new AST_VariableDeclaration(new AST_Identifier("HD_return_value"),$1,NULL,NULL,AST_VariableDeclarationLocusUnknown); }
return_type_definition : type_definition TYPE_LEFT_BRACKET TYPE_RIGHT_BRACKET { $$ = new AST_VariableDeclaration(new AST_Identifier("HD_return_value"),$1,new std::vector<AST_Expression*>,NULL,AST_VariableDeclarationLocusUnknown); }

function_definition : return_type_definition identifier TYPE_LEFT_ROUND vector_variable_declaration TYPE_RIGHT_ROUND block { $$ = new AST_FunctionDefinition($2,$4,$6,$1);}

function_definition : return_type_definition identifier TYPE_LEFT_ROUND vector_variable_declaration TYPE_RIGHT_ROUND TYPE_SEMICOLON { $$ = new AST_FunctionDefinition($2,$4,NULL,$1);}


function_definition : return_list identifier TYPE_LEFT_ROUND vector_variable_declaration TYPE_RIGHT_ROUND block { $$ = new AST_FunctionDefinition($2,$4,$6,NULL,$1);}

function_definition : return_list identifier TYPE_LEFT_ROUND vector_variable_declaration TYPE_RIGHT_ROUND TYPE_SEMICOLON { $$ = new AST_FunctionDefinition($2,$4,NULL,NULL,$1);}


return_list : TYPE_LEFT_BRACKET vector_variable_declaration TYPE_RIGHT_BRACKET { $$ = $2;}



class_body : variable_declaration TYPE_SEMICOLON { $$ = new vector<AST_Node *>(); $$->push_back($1);}
             | class_body variable_declaration TYPE_SEMICOLON { $1->push_back($2); $$ = $1;}
             | function_definition { $$ = new vector<AST_Node *>(); $$->push_back($1);}
             | class_body function_definition  { $1->push_back($2); $$ = $1;}
             | { $$ = new vector<AST_Node *>(); }


class_declaration : TYPE_DEF_CLASS identifier TYPE_LEFT_MUSTACHE class_body TYPE_RIGHT_MUSTACHE {
    $$ = new AST_ClassDefinition($2,new std::vector<AST_Identifier*>,$4);}

class_declaration : TYPE_DEF_CLASS identifier TYPE_LEFT_ROUND vector_identifiers TYPE_RIGHT_ROUND TYPE_LEFT_MUSTACHE class_body TYPE_RIGHT_MUSTACHE { $$ = new AST_ClassDefinition($2,$4,$7); }

return_statement : TYPE_KEYWORD_RETURN expression { $$ = new AST_ReturnStatement($1,$2); } 

function_call : identifier TYPE_LEFT_ROUND vector_expression TYPE_RIGHT_ROUND { $$ = new AST_FunctionCall($1,$3); }


function_call : expression TYPE_DOT function_call {$$ = $3; dynamic_cast<AST_FunctionCall *>($$)->objectOfCall = $1;  }

function_call : expression TYPE_DOT identifier { $$ = new AST_FunctionCall($3,new std::vector<AST_Expression*>()); dynamic_cast<AST_FunctionCall *>($$)->objectOfCall = $1; }


vector_identifiers : vector_identifiers TYPE_COMMA identifier { $1->push_back($3); $$ = $1; }
                 | identifier { $$ = new std::vector<AST_Identifier*>; $$->push_back($1);}
                 | { $$ = new std::vector<AST_Identifier*>;}

vector_expression : vector_expression TYPE_COMMA expression { $1->push_back($3); $$ = $1; }
                 | expression { $$ = new std::vector<AST_Expression*>; $$->push_back($1);}
                 | { $$ = new std::vector<AST_Expression*>;}

vector_variable_declaration : variable_declaration { $$ = new std::vector<AST_VariableDeclaration*>; $$->push_back($<variable_declaration>1); }
      | vector_variable_declaration TYPE_COMMA variable_declaration { $1->push_back($<variable_declaration>3); }
      | {$$ = new std::vector<AST_VariableDeclaration*>;}
      ;

variable_declaration : identifier identifier
{
    HDParseSymbol *p = new HDParseSymbol();
    *p = $1->compilerContext;
    p->tokenType = TYPE_DEF_CLASS;
    $$ = new AST_VariableDeclaration($2,p,NULL,NULL,AST_VariableDeclarationLocusBlock,*($1->name));
    delete $1;
}

variable_declaration : return_type_definition identifier
{

    HDParseSymbol *p = new HDParseSymbol();
    *p = *($1->type);
    $$ = new AST_VariableDeclaration($2,p,$1->sizes?new std::vector<AST_Expression*>:NULL,NULL,AST_VariableDeclarationLocusBlock);
    delete $1;
}

variable_declaration : return_type_definition identifier TYPE_LEFT_BRACKET vector_expression TYPE_RIGHT_BRACKET
{

    HDParseSymbol *p = new HDParseSymbol();
    *p = *($1->type);
    $$ = new AST_VariableDeclaration($2,p,$4,NULL,AST_VariableDeclarationLocusBlock);
    delete $1;
}


three_dots : TYPE_DOT TYPE_DOT TYPE_DOT { $$ = new AST_ArrayAccessExpression($1,NULL,NULL); }

access_range : expression TYPE_COLON expression { $$ = new AST_ArrayAccessExpression($2,$1,$3); }
access_range : TYPE_COLON expression { $$ = new AST_ArrayAccessExpression($1,NULL,$2); }
access_range : expression TYPE_COLON { $$ = new AST_ArrayAccessExpression($2,$1,NULL); }
access_range : TYPE_COLON { $$ = new AST_ArrayAccessExpression($1,NULL,NULL); }
access_range : expression { $$ = new AST_ArrayAccessExpression(new HDParseSymbol(TYPE_SEMICOLON),$1,NULL);}
access_range : three_dots

vector_access_range : access_range { $$ = new vector<AST_ArrayAccessExpression *>; $$->push_back(dynamic_cast<AST_ArrayAccessExpression *>($1)); }
| vector_access_range TYPE_COMMA access_range { $$->push_back(dynamic_cast<AST_ArrayAccessExpression *>($3));}

/*expression_slownikowe : expression TYPE_COLON expression { $$ = new AST_DictionaryExpression($1,$3);} */

list : TYPE_LEFT_BRACKET vector_expression TYPE_RIGHT_BRACKET { $$ = new AST_List($2); }

/*
slownik : TYPE_LEFT_MUSTACHE vector_expression_slownikowe TYPE_RIGHT_MUSTACHE { $$ = new AST_Dictionary($2); }

vector_expression_slownikowe: vector_expression_slownikowe TYPE_COMMA expression_slownikowe { $1->push_back($3); $$ = $1; }
                     | expression_slownikowe { $$ = new std::vector<AST_Expression*>; $$->push_back($1);}
                     | { $$ = new std::vector<AST_Expression*>;} */

identifier : TYPE_IDENTIFIER {  $$ = new AST_Identifier($1);  }
      ;

numeric : TYPE_INTEGER { $$ = new AST_Integer($1); }
        | TYPE_DECIMAL { $$ = new AST_Decimal($1);  }
        ;

string : TYPE_STRING {  $$ = new AST_String($1);   } 



//expression : expression TYPE_DOT expression { $$ = new AST_ClassVariable($1,$3);}




/*klasy: expression TYPE_DOT identifier { }

expression : klasy TYPE_DOT function_call { }

klasy : klasy TYPE_DOT identifier { }*/

in_test_expression : expression TYPE_IN_KEYWORD expression { $$ = new AST_InTest($1,$3);}

array_access : expression TYPE_LEFT_BRACKET vector_access_range TYPE_RIGHT_BRACKET {$$ = new AST_ArrayAccess($1,$3);}


expression : identifier TYPE_SUBSTITUTION expression { $$ = new AST_Substitution( $1, $3,$2);}
     |  identifier TYPE_PLUS_EQUAL expression {$2->tokenType = TYPE_PLUS;$$ = new AST_Substitution( new AST_Identifier(*$1), new AST_BinaryOperator($1,$3,$2),$2);}
     |  identifier TYPE_MINUS_EQUAL expression {$2->tokenType = TYPE_MINUS;$$ = new AST_Substitution( new AST_Identifier(*$1), new AST_BinaryOperator($1,$3,$2),$2);}
     |  identifier TYPE_TIMES_EQUAL expression {$2->tokenType = TYPE_TIMES;$$ = new AST_Substitution( new AST_Identifier(*$1), new AST_BinaryOperator($1,$3,$2),$2);}
     |  identifier TYPE_DIVIDE_EQUAL expression {$2->tokenType = TYPE_DIVIDED; $$ = new AST_Substitution( new AST_Identifier(*$1), new AST_BinaryOperator($1,$3,$2),$2);}
|  array_access TYPE_SUBSTITUTION expression { $$ = new AST_Substitution( dynamic_cast<AST_ArrayAccess *>($1), $3,$2);}
|  array_access TYPE_PLUS_EQUAL expression {$2->tokenType = TYPE_PLUS;$$ = new AST_Substitution( new AST_ArrayAccess(*dynamic_cast<AST_ArrayAccess *>($1)), new AST_BinaryOperator($1,$3,$2),$2);}
|  array_access TYPE_MINUS_EQUAL expression {$2->tokenType = TYPE_MINUS;$$ = new AST_Substitution( new AST_ArrayAccess(*dynamic_cast<AST_ArrayAccess *>($1)), new AST_BinaryOperator($1,$3,$2),$2);}
|  array_access TYPE_TIMES_EQUAL expression {$2->tokenType = TYPE_TIMES;$$ = new AST_Substitution( new AST_ArrayAccess(*dynamic_cast<AST_ArrayAccess *>($1)), new AST_BinaryOperator($1,$3,$2),$2);}
|  array_access TYPE_DIVIDE_EQUAL expression {$2->tokenType = TYPE_DIVIDED; $$ = new AST_Substitution( new AST_ArrayAccess(*dynamic_cast<AST_ArrayAccess *>($1)), new AST_BinaryOperator($1,$3,$2),$2);}
|  conditional_expression
|     numeric
     |  string
     |  TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND { $$ = $2; }
     |  expression TYPE_PLUS expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_TENSOR_PRODUCT expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_MINUS expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_TIMES expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_DIVIDED expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  TYPE_MINUS expression %prec UNARY {$$ = new AST_UnaryOperator($2,$1);}
     |  TYPE_NOT expression %prec UNARY {$$ = new AST_UnaryOperator($2,$1);}
     |  expression TYPE_POW expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  function_call
     |  array_access
     |  list
     |  identifier { $$ = new AST_IdentifierExpression($1); }
     |  TYPE_LEFT_ROUND expression TYPE_RIGHT_ROUND %prec BRACKETS { $$ = $2;}
     |  expression TYPE_OR expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_AND expression {$$ = new AST_BinaryOperator($1,$3,$2);  }
     |  expression TYPE_EQUAL expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_NEQUAL expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_SMALLER_T expression {$$ = new AST_BinaryOperator($1,$3,$2);  }
     |  expression TYPE_GREATER_T expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_SMALLER_EQUAL expression {$$ = new AST_BinaryOperator($1,$3,$2); }
     |  expression TYPE_GREATER_EQUAL expression {$$ = new AST_BinaryOperator($1,$3,$2);}
     |  expression TYPE_AT expression {$$ = new AST_BinaryOperator($1,$3,$2);}
     |  in_test_expression

%%

    void yyerror(const char *s, ...) 
    {  
      va_list ap;
      va_start(ap, s);        
      if(yylloc.first_line)
        fprintf(stderr, "%d.%d-%d.%d: ", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column);
      vfprintf(stderr, s, ap);
      fprintf(stderr, "\n");    
      hasErrors = true;
    }    

    void lyyerror(YYLTYPE t, char *s, ...)
    {
      va_list ap;
      va_start(ap, s);
    
      if(t.first_line) fprintf(stderr, "%d.%d-%d.%d: ", t.first_line, t.first_column, t.last_line, t.last_column);
      vfprintf(stderr, s, ap);
      fprintf(stderr, "\n");
      hasErrors = true;
    }

