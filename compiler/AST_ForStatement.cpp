//
//  AST_ForStatement.cpp
//  Hodor
//
//  Created by Test on 23.11.2013.
//
//

#include "AST_ForStatement.h"
#include "AST_FunctionCall.h"
#include "AST_Expression.h"
#include "AST_VariableDeclaration.h"

static AST_VariableDeclaration *argInt = NULL;



AST_ForStatement::AST_ForStatement(AST_VariableDeclaration *var, AST_Expression *set, AST_Block *execBlock, AST_VariableDeclaration *secondVar) : AST_Statement(&(var->compilerContext))
{
    this->counter = var;
    this->execBlock = execBlock;
    this->set = set;
    this->secondCounter = secondVar;
    if(secondVar) children.push_back(secondVar);
    if(var) children.push_back(var);
    if(set) children.push_back(set);

    if(execBlock) children.push_back(execBlock);

    hasOptimizedRange = false;
    if(set->typ == TYPE_FUNCTION_CALL)
    {
        AST_FunctionCall *fc = dynamic_cast<AST_FunctionCall *>(set);
        if(strcmp(fc->ident->name->c_str(),"range")==0)
        {
            hasOptimizedRange = true;
        }
        if(strcmp(fc->ident->name->c_str(),"reverseRange")==0)
        {
            hasOptimizedRange = true;
        }
    }
    
    typ = TYPE_FOR_KEYWORD;
}

AST_ForStatement::~AST_ForStatement()
{
    if(execBlock) delete execBlock;
    if(set) delete set;
    if(counter) delete counter;
    if(secondCounter) delete secondCounter;
}



HDExecutionContext *AST_ForStatement::applyCompilerPassWithContext(int passNo,HDExecutionContext *context,std::string *output) /* returns context for deeper progress */
{
    switch (passNo) {
        case 2:
            executionContext = *context;
            if(!hasOptimizedRange) counter->locus = AST_VariableDeclarationLocusForLoop;
            if(secondCounter) {
                secondCounter->locus = AST_VariableDeclarationLocusForLoopSecondCounter;
            }
          //  printf("%p Binding for loop with context %p\n",this,&executionContext);
            break;
        case 5:
        {
            AST_VariableDeclaration *type = set->computeFinalType(&executionContext);
            if(type->type->tokenType == TYPE_DEF_LIST && !hasOptimizedRange)
            {
                counter->type->tokenType = TYPE_DEF_VOID;
            }
        }
            
            
            break;
        default:
            break;
    }
    return &executionContext;
}







std::string AST_ForStatement::constructOutputString(HDExecutionContext *context)
{
    AST_VariableDeclaration *type = set->computeFinalType(context);
    std::string out = "";
    std::string setOut = set->constructOutputString(context);
    std::string loopSetVarPrefix = "";
    bool shouldCreateLoopSetVar = false;
    
//    printf("for loop counter %s type %d addr %p \n",counter->name->name->c_str(),counter->type->tokenType,counter);
    
    
    if(set->typ != TYPE_EXPR_IDENT)
    {
        shouldCreateLoopSetVar = true;
        setOut = "HDSet__loop";
        loopSetVarPrefix = type->typeString(context,setOut,set)+" HDSet__loop = "+set->constructOutputString(context)+";\n";
    }
    
    if(type->type->tokenType == TYPE_DEF_VOID)
    {
        printf("%d-%d Error - you must use explicit cast to iterate thorugh object enclosed in list/dictionary.",compilerContext.lineNumber,compilerContext.linePosition);
        exit(1);
    }
    
    if(counter->sizes)
    {
        printf("%d-%d Error - iterating requires an iterator, not array of iterators.",compilerContext.lineNumber,compilerContext.linePosition);
        exit(1);
    }
    
    /* we can iterate through all that has size plus list, dictionary and string */
    std::string secondCounterPrefix = "";
    std::string secondCounterSufix = "";
    if(secondCounter) {
        if(secondCounter->sizes)
        {
            secondCounter->errorMessage("iterating through second argument requires an integer, not array of itegers.");
        }
        if(secondCounter->type->tokenType != TYPE_DEF_INTEGER)
        {
            secondCounter->errorMessage("iterating requires an integer, not %d.",secondCounter->type->tokenType);
        }
        secondCounterPrefix = "int "+*(secondCounter->name->name)+" = 1;\n";
        secondCounterSufix = ","+*(secondCounter->name->name)+"++";
    }
    secondCounterPrefix+=loopSetVarPrefix;
    
    
    
    
    if(type->sizes)
    {
        if(type->type->tokenType != counter->type->tokenType)
        { /* iterator for arrays */
            if(counter->type->tokenType != TYPE_DEF_ITERATOR)
            {
                printf("%d-%d Error - iterating requires iterator or variable of the type stored in an array.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(type->type->tokenType == TYPE_DEF_DECIMAL)
            {
                out+=secondCounterPrefix+"for(HDTensor<"+context->floatingPointTypeName+">::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
                out+=execBlock->constructOutputString(&executionContext);
                return secondCounter || shouldCreateLoopSetVar? "{\n"+out+"}" : out;
            }
            
            if(type->type->tokenType == TYPE_DEF_INTEGER)
            {
                out+=secondCounterPrefix+"for(HDTensor<int>::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
                out+=execBlock->constructOutputString(&executionContext);
                return secondCounter || shouldCreateLoopSetVar? "{\n"+out+"}" : out;
            }
            
            return secondCounter || shouldCreateLoopSetVar ? "{\n"+out+"}" : out;
        }
        
        if(type->type->tokenType == TYPE_DEF_STRING)
        {
            out+=secondCounterPrefix+"for(HDList<"+context->floatingPointTypeName+",char>::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
            out+=execBlock->constructOutputString(&executionContext);
            return secondCounter || shouldCreateLoopSetVar? "{\n"+out+"}" : out;
        }
        
        if(type->type->tokenType == TYPE_DEF_DECIMAL)
        {
            out+=secondCounterPrefix+"for(HDTensor<"+context->floatingPointTypeName+">::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
            out+=execBlock->constructOutputString(&executionContext);
            return secondCounter || shouldCreateLoopSetVar? "{\n"+out+"}" : out;
        }
        
        if(type->type->tokenType == TYPE_DEF_INTEGER)
        {
            out+=secondCounterPrefix+"for(HDTensor<int>::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
            out+=execBlock->constructOutputString(&executionContext);
            return secondCounter || shouldCreateLoopSetVar? "{\n"+out+"}" : out;
        }

        
        
    }
    
    if(type->type->tokenType == TYPE_DEF_LIST)
    { /* iterating through list, iterator can be anything */
        if(set->typ == TYPE_FUNCTION_CALL)
        { /* special case - set is range */
            AST_FunctionCall *fc = dynamic_cast<AST_FunctionCall *>(set);
            if(!argInt) argInt = new AST_VariableDeclaration("INT",TYPE_DEF_INTEGER);
            
            if(strcmp(fc->ident->name->c_str(),"range")==0)
            {
                if(fc->args->size()!=2) errorMessage("range() needs two arguments");
                std::string low = context->constructSubstitutionString(argInt,(*(fc->args->begin())),"for loop");
                std::string high = context->constructSubstitutionString(argInt,(*(++(fc->args->begin()))),"for loop");
                secondCounterPrefix = "int HDHelper__c1 = "+low+";\nint HDHelper__c2 = "+high+";\n";
                
                out+=secondCounterPrefix+"for(int "+*(counter->name->name)+" = HDHelper__c1;"+*(counter->name->name)+"<=HDHelper__c2;"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
            }
            if(strcmp(fc->ident->name->c_str(),"reverseRange")==0)
            {
                if(fc->args->size()!=2) errorMessage("reverseRange() needs two arguments");
                std::string low = context->constructSubstitutionString(argInt,(*(fc->args->begin())),"for loop");
                std::string high = context->constructSubstitutionString(argInt,(*(++(fc->args->begin()))),"for loop");
                
                secondCounterPrefix = "int HDHelper__c1 = "+low+";\nint HDHelper__c2 = "+high+";\n";
                
                out+=secondCounterPrefix+"for(int "+*(counter->name->name)+" = HDHelper__c1;"+*(counter->name->name)+">=HDHelper__c2;"+*(counter->name->name)+"--"+secondCounterSufix+")\n";
            }

        }

        if(secondCounter) errorMessage("Cannot use second counter for range");
        if(!hasOptimizedRange) out+=secondCounterPrefix+"for(HDList<"+context->floatingPointTypeName+",char>::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++)\n";
        out+=execBlock->constructOutputString(&executionContext);
        return (secondCounter || shouldCreateLoopSetVar || hasOptimizedRange) ? "{\n"+out+"}" : out;
    }
    
    if(type->type->tokenType == TYPE_DEF_DICTIONARY)
    { /* iterating through dictionary, iterator will be string */
        if(counter->type->tokenType != TYPE_DEF_STRING)
        {
            printf("%d-%d Error - you must use string variable to iterate through dictionary.",compilerContext.lineNumber,compilerContext.linePosition);
            exit(1);
        }
        out+=secondCounterPrefix+"for(HDDictionary::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
        out+=execBlock->constructOutputString(&executionContext);
        return secondCounter|| shouldCreateLoopSetVar ? "{\n"+out+"}" : out;
    }

    if(type->type->tokenType == TYPE_DEF_STRING)
    { /* iterating through dictionary, iterator will be string */
        if(counter->type->tokenType != TYPE_DEF_STRING && counter->type->tokenType != TYPE_DEF_ITERATOR)
        {
            printf("%d-%d Error - you must use string or iterator variable to iterate through string.",compilerContext.lineNumber,compilerContext.linePosition);
            exit(1);
        }
        out+=secondCounterPrefix+"for(HDStringEnvelope<"+context->floatingPointTypeName+",char>::iterator "+*(counter->name->name)+" = "+setOut+".begin();"+*(counter->name->name)+"!="+setOut+".end();"+*(counter->name->name)+"++"+secondCounterSufix+")\n";
        out+=execBlock->constructOutputString(&executionContext);
        return secondCounter|| shouldCreateLoopSetVar ? "{\n"+out+"}" : out;
    }

    printf("%d-%d Error - unsupported data type to iterate through: %d.",compilerContext.lineNumber,compilerContext.linePosition,type->type->tokenType);
    exit(1);
    
    return std::string();
}
