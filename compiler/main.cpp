#include "nodes.h"
#include <iostream>
#include "parser.hpp"
#include <stack>


struct yy_buffer_state;

extern struct yy_buffer_state *yy_scan_string (const char *yy_str);
extern void yy_flush_buffer(struct yy_buffer_state *aa);
extern void resetFlexBuffers();

int linesInInclude = 0;

bool hasMain;

HDExecutionContext primaryContext;

extern AST_Definitions *program;
extern int yyparse(); 
extern bool hasErrors;
extern int yydebug;

#define BUF_SIZ 1000000

int main(int argc, char ** argv) 
{
  hasMain = false;
  std::string out = "";
  program = NULL;
  FILE *lib,*file;

//yydebug = 1;
  printf("Reading stdlib.hd file...\n");
  lib = fopen("stdlib.hd","r");
  if(!lib) 
  {
    printf("Error - please supply stdlib.hd file (may be empty).\n");
    exit(1);  
  }
  
  char fileBuffer[BUF_SIZ]; /* 10 megaBytes */
  int readBytesFromLibraryFile = fread(fileBuffer,sizeof(char),BUF_SIZ,lib);
  fclose(lib);
  
  fileBuffer[readBytesFromLibraryFile] = 0;
  struct yy_buffer_state *state = yy_scan_string(fileBuffer);
  
  
  yyparse();

  yy_flush_buffer(state);
  resetFlexBuffers();
  
  AST_Definitions *headerFileTree = program;
  program = NULL;
  if(hasErrors)
  {
    exit(1);
  }
   headerFileTree->compilerPassWithContext(1,&primaryContext);
   headerFileTree->compilerPassWithContext(2,&primaryContext);
   headerFileTree->compilerPassWithContext(3,&primaryContext);
   headerFileTree->compilerPassWithContext(4,&primaryContext);
  
  
  for(map<std::string,AST_FunctionDefinition *>::iterator it = primaryContext.functionsDefinedInContext.begin();it!=primaryContext.functionsDefinedInContext.end();it++)
  {
    AST_FunctionDefinition *fun = (*it).second;
    const std::string &key = (*it).first;
    fun->isExternalCppFunction = true;
    if(key.length()>9 && std::string("_HDClass_").compare(key.substr(0,9))==0)
       fun->isExternalCppFunction = false;
  }
  
  
  if(argc < 2)
  {
    printf("Error - please supply the file name to read.\n");
    exit(1);
  }
  printf("Reading %s...\n",argv[1]);
  file = fopen(argv[1],"r");
  if(!file)
  {
    printf("Error - given file not found.\n");
    exit(1);
  }
  
  int readBytesFromActualFile = fread(fileBuffer,sizeof(char),BUF_SIZ,file);
  fclose(file);

  fileBuffer[readBytesFromActualFile] = 0;
  yy_scan_string(fileBuffer);

  yyparse();
  
  if(hasErrors)
  {
    exit(1);
  }

//printf("Complicted");

primaryContext.floatingPointTypeName = "double";

out = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include \"../runtime/HDUniversalEnvelope.cpp\"\n#include \"../runtime/HDTensor.cpp\"\n#include \"../runtime/HDStringEnvelope.cpp\"\n#include <math.h>\n#include \"../runtime/HDPrimitiveEnvelope.cpp\"\n#include \"../runtime/HDList.cpp\"\n#include \"../runtime/HStdLib.cpp\"\n\n\n";

  
  
  /* Pass one - Locate all function and class definitions and insert them into main context */
    program->compilerPassWithContext(1,&primaryContext);
  /* Pass two - Locate all variable declarations (including arguments) and fit them into corre=ct contexts. We also propagate information and create contexts for for loops and if statements */
    program->compilerPassWithContext(2,&primaryContext);
  /* Pass three - only function calls are analyzed, we fill out functionNameForFunctionPointer field on pointers */
    program->compilerPassWithContext(3,&primaryContext);
  /* Pass four - Variable declarations bind function pointers */
    program->compilerPassWithContext(4,&primaryContext);
  /* Function and class definitions create their prototype strings */
    out+=program->compilerPassWithContext(5,&primaryContext);
    
    out+="\n/* *************************** functions ******************************** */\n\n";
//printf("%lu %lu\n",context.variablesDefinedInContext.size(),context.functionsDefinedInContext.size());



if(program!=NULL) { out+= program->constructOutputString(&primaryContext); }
if(hasMain)
    out+="int main()\n{\nreturn HDFunction__main();\n}\n";
    
  cout<<"Generating output file: "+(argv[1]+std::string(".cpp"))<<endl;
  file = fopen((argv[1]+std::string(".cpp")).c_str(),"w");
  fwrite(out.c_str(),sizeof(char),strlen(out.c_str()),file);
  fclose(file);
  
    const HDMetaContext &smartContext = HDMetaContext::sharedContext();
    int i=0;
    for(map<std::string,AST_VariableDeclaration *>::const_iterator it = smartContext.smartDefines.begin();it!=smartContext.smartDefines.end();it++)
    {
        if(i++==0) printf("Warning - the following variables were not declared and were assigned integer values:\n");
        printf("Variable: %s - %d\n",(*it).first.c_str(),i);
    }
  delete headerFileTree;
  delete program;
return 0;
}