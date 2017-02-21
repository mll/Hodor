%{
#include "nodes.h"
#include "parser.hpp"
#include <stack>
using namespace std;  

extern int linesInInclude;

#define YY_USER_ACTION if(currentLine > linesInInclude && linesInInclude != 0) { currentLine = currentLine - linesInInclude+1; linesInInclude = 0;}  yylloc.first_line = yylloc.last_line = currentLine; yylloc.first_column = currentLinePosition; yylloc.last_column = currentLinePosition + yyleng - 1;
#define RETURN_TOKEN(t) yylval.token = new HDParseSymbol(); return returnToken(t);
#define RETURN_NO_CREATION(t) return returnToken(t);


std::string buffer;
int currentLine = 1; 
int currentLinePosition = 1;
std::string lineText;

void resetFlexBuffers()
{
 buffer = "";
 currentLine = 1;
 currentLinePosition = 1;
 lineText = "";
}

inline int returnToken(int type) 
{
 yylval.token->tokenType = type; 
 yylval.token->lineNumber = currentLine; 
 yylval.token->linePosition = currentLinePosition;
 yylval.token->lineText = new string(lineText);
 yylval.token->symbolText = new string(yytext,yyleng);
 yylval.token->value = new string(yytext,yyleng);  
 currentLinePosition+=yyleng;
 return type;
}




extern "C" int yywrap() {return 1;}
%}
            
%option yylineno

%x STRING_MATCHED
%s STANDARD

%%
BEGIN STANDARD;
<STRING_MATCHED>\"  {
                      if(buffer.length() && buffer[buffer.length()-1] == '\\')
                      {
                        currentLinePosition++; buffer+=yytext;
                      }
                      else
                      {
                        BEGIN STANDARD; yylval.token = new HDParseSymbol();
                        yylval.token->value = new string(buffer);
                        yylval.token->tokenType = TYPE_STRING;
                        yylval.token->lineNumber = currentLine;
                        yylval.token->linePosition = currentLinePosition;
                        lineText+=buffer;
                        yylval.token->lineText = new string(lineText);
                        yylval.token->symbolText = new string(buffer);
                        yylval.token->value = new string(buffer);
                        currentLinePosition+=buffer.length()+1;
                        return TYPE_STRING;
                      }
                    }
<STRING_MATCHED>\n currentLinePosition = 1; currentLine++; buffer+=yytext;
<STRING_MATCHED><<EOF>> printf("%d:%d Syntax error. Unexpected end of file",currentLine,currentLinePosition);
<STRING_MATCHED>. currentLinePosition++; buffer+=yytext;
 
\n ++currentLine;currentLinePosition = 0;lineText = "";
"/*"        {
              register int cc = 0;
              register int last = 0;
              currentLinePosition+=2;
              while ( ((cc = yyinput()) != '/' || last != '*')  && cc != EOF )     /* eat up text of comment */
              {
                  if(cc == '\n') 
                  {
                    currentLinePosition = 0;
                    currentLine++;
                  }
                  else
                  {
                    currentLinePosition++;
                  }
                  last = cc;
              } 
              if ( cc != '/' )
              {
                  printf( "%d:%d - Syntax error. End of file in a comment",currentLine,currentLinePosition);
                  yyterminate();
              }
            }
"//" {
              register int cc = 0;
              register int last = 0;
              currentLinePosition+=2;
              while ( (cc = yyinput()) != '\n'  && cc != EOF ) currentLinePosition++;    /* eat up text of comment */
              
              if(cc == '\n') 
              {
                currentLinePosition = 0;
                currentLine++;
              }
               
     }
\"                       currentLinePosition++; buffer = ""; BEGIN STRING_MATCHED;

"if"                     RETURN_TOKEN(TYPE_IF_KEYWORD);
"for"                    RETURN_TOKEN(TYPE_FOR_KEYWORD);
"while"                  RETURN_TOKEN(TYPE_WHILE_KEYWORD);
"in"                     RETURN_TOKEN(TYPE_IN_KEYWORD);
"else"                   RETURN_TOKEN(TYPE_ELSE_KEYWORD);
"void"                   RETURN_TOKEN(TYPE_DEF_VOID);
"float"                  RETURN_TOKEN(TYPE_DEF_DECIMAL);
"string"                 RETURN_TOKEN(TYPE_DEF_STRING);
"int"                    RETURN_TOKEN(TYPE_DEF_INTEGER);
"dictionary"             RETURN_TOKEN(TYPE_DEF_DICTIONARY);
"function"               RETURN_TOKEN(TYPE_DEF_FUNCTION);
"list"                   RETURN_TOKEN(TYPE_DEF_LIST);
"iterator"               RETURN_TOKEN(TYPE_DEF_ITERATOR);
"class"                  RETURN_TOKEN(TYPE_DEF_CLASS);
"return"                 RETURN_TOKEN(TYPE_KEYWORD_RETURN);
"and"                    RETURN_TOKEN(TYPE_AND);
"or"                     RETURN_TOKEN(TYPE_OR);
"not"                    RETURN_TOKEN(TYPE_NOT);

":"                      RETURN_TOKEN(TYPE_COLON);
[a-zA-Z_][a-zA-Z0-9_]*   RETURN_TOKEN(TYPE_IDENTIFIER);
[0-9]+\.[0-9]*([dDeE][0-9]+)? RETURN_TOKEN(TYPE_DECIMAL);
[0-9]+                   RETURN_TOKEN(TYPE_INTEGER);
"+="                     RETURN_TOKEN(TYPE_PLUS_EQUAL);
"-="                     RETURN_TOKEN(TYPE_MINUS_EQUAL);
"*="                     RETURN_TOKEN(TYPE_TIMES_EQUAL);
"/="                     RETURN_TOKEN(TYPE_DIVIDE_EQUAL);
"="                      RETURN_TOKEN(TYPE_SUBSTITUTION);
"**"                     RETURN_TOKEN(TYPE_POW);
"^"                      RETURN_TOKEN(TYPE_TENSOR_PRODUCT);
"+"                      RETURN_TOKEN(TYPE_PLUS);
"-"                      RETURN_TOKEN(TYPE_MINUS);
"*"                      RETURN_TOKEN(TYPE_TIMES);
"/"                      RETURN_TOKEN(TYPE_DIVIDED);
"("                      RETURN_TOKEN(TYPE_LEFT_ROUND);
")"                      RETURN_TOKEN(TYPE_RIGHT_ROUND);
","                      RETURN_TOKEN(TYPE_COMMA);
"=="                     RETURN_TOKEN(TYPE_EQUAL);
"<="                     RETURN_TOKEN(TYPE_SMALLER_EQUAL);
"!="                     RETURN_TOKEN(TYPE_NEQUAL);
">="                     RETURN_TOKEN(TYPE_GREATER_EQUAL);
">"                      RETURN_TOKEN(TYPE_GREATER_T);
"<"                      RETURN_TOKEN(TYPE_SMALLER_T);
"["                      RETURN_TOKEN(TYPE_LEFT_BRACKET);
"]"                      RETURN_TOKEN(TYPE_RIGHT_BRACKET);
"{"                      RETURN_TOKEN(TYPE_LEFT_MUSTACHE);
"}"                      RETURN_TOKEN(TYPE_RIGHT_MUSTACHE);
"."                      RETURN_TOKEN(TYPE_DOT);
";"                      RETURN_TOKEN(TYPE_SEMICOLON);
"@"                      RETURN_TOKEN(TYPE_AT);
"?"                      RETURN_TOKEN(TYPE_QUESTION_MARK);
" "|\t                   currentLinePosition++;
.                        printf("%d:%d - Syntax error. Unknown symbol '%s' \n",currentLine,currentLinePosition,yytext); yyterminate();
%%
