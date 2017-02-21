//
//  AST_BinaryOperator.cpp
//  Hodor
//
//  Created by Marek Lipert on 02.11.2013.
//
//

#include "AST_BinaryOperator.h"
#include "AST_VariableDeclaration.h"
#include "AST_Identifier.h"
#include "AST_FunctionCall.h"


static AST_VariableDeclaration *var = NULL;


AST_BinaryOperator::AST_BinaryOperator(AST_Expression *l, AST_Expression *r, HDParseSymbol*s) : AST_Expression(s)
{
    left=l;
    right=r;
    this->op=s->tokenType;
    if(l) children.push_back(l);
    if(r) children.push_back(r);
    
    typ = TYPE_BINARY_OPERATOR;
}



bool AST_BinaryOperator::findPlusSubstitution(std::string name,HDExecutionContext *context)
{
  if(!allSubnodesPlus()) return false;
  
  return  seekForNameAndExpel(name);
}

bool AST_BinaryOperator::seekForNameAndExpel(std::string name)
{
  
  if(left && left->typ == TYPE_IDENTIFIER && name.compare(*(dynamic_cast<AST_Identifier *>(left)->name))==0)
  {
    children.erase(std::remove(children.begin(), children.end(), left), children.end()); /* how i hate c++; this just removes left from vector! */
    delete left;
    left = NULL;
    return true;
  }
  if(right && right->typ == TYPE_IDENTIFIER && name.compare(*(dynamic_cast<AST_Identifier *>(right)->name))==0)
  {
    children.erase(std::remove(children.begin(), children.end(), right), children.end());
    delete right;
    right = NULL;
    return true;
  }


  if(left && left->typ == TYPE_BINARY_OPERATOR && dynamic_cast<AST_BinaryOperator *>(left)->seekForNameAndExpel(name)) return true;
  if(right && right->typ == TYPE_BINARY_OPERATOR && dynamic_cast<AST_BinaryOperator *>(right)->seekForNameAndExpel(name)) return true;
  return false;
}


bool AST_BinaryOperator::allSubnodesPlus()
{
  if(op!=TYPE_PLUS) return false;
  if(left->typ != TYPE_BINARY_OPERATOR && right->typ != TYPE_BINARY_OPERATOR) return true;
  bool leftPart = true;
  bool rightPart = true;
  if(left && left->typ == TYPE_BINARY_OPERATOR) leftPart = dynamic_cast<AST_BinaryOperator *>(left)->allSubnodesPlus();
  if(right && right->typ == TYPE_BINARY_OPERATOR) leftPart = dynamic_cast<AST_BinaryOperator *>(right)->allSubnodesPlus();
  return leftPart && rightPart;
}



AST_VariableDeclaration *AST_BinaryOperator::computeFinalType(HDExecutionContext *context) /* returned construct is borrowed for the caller */
{
    if(!var) var = new AST_VariableDeclaration("HD internal bool",TYPE_DEF_INTEGER);

    if(!left) return right->computeFinalType(context);
    if(!right) return left->computeFinalType(context);
  
    AST_VariableDeclaration *leftType = left->computeFinalType(context);
    AST_VariableDeclaration *rightType = right->computeFinalType(context);
    

    /* logical types always return int */
    if(op == TYPE_EQUAL || op == TYPE_AND || op == TYPE_OR || op == TYPE_NEQUAL || op == TYPE_GREATER_T || op == TYPE_GREATER_EQUAL || op == TYPE_SMALLER_T || op == TYPE_SMALLER_EQUAL ) return var;
    
    /* dynamic types are re-propagated up */
    if(rightType->type->tokenType == TYPE_DEF_VOID) return rightType;
    if(leftType->type->tokenType == TYPE_DEF_VOID) return leftType;


    
    if(op == TYPE_SMALLER_T || op == TYPE_GREATER_T || op == TYPE_EQUAL || op == TYPE_NEQUAL || op == TYPE_AND || op == TYPE_OR)
    {
        if((rightType->type->tokenType == leftType->type->tokenType || rightType->type->tokenType == TYPE_DEF_VOID || leftType->type->tokenType == TYPE_DEF_VOID) && ((leftType->sizes && rightType->sizes) || (!leftType->sizes && !rightType->sizes)))
           {
               return var;
           }
           else
           {
               this->errorMessage("unsupported types for binary operator.");
           }
        
    }
    
    if(leftType->type->tokenType == rightType->type->tokenType)
    { /* types agree instantly */
        /* both are of array type */
        if(leftType->sizes && rightType->sizes) return leftType;
        /* both are of standard type */
        if(leftType->sizes == NULL && rightType->sizes == NULL) return leftType;
        if(rightType->sizes) return rightType;
        if(leftType->sizes) return leftType;
        if(leftType->type->tokenType == TYPE_DEF_DECIMAL || leftType->type->tokenType == TYPE_DEF_INTEGER) return leftType;
        printf("%d-%d Error - unsupported types for binary operator.",compilerContext.lineNumber,compilerContext.linePosition);
        exit(1);
    }

    

           /* string op sth --> string */
    if(leftType->type->tokenType == TYPE_DEF_STRING && rightType->sizes == NULL) return leftType;
    if(rightType->type->tokenType == TYPE_DEF_STRING && leftType->sizes == NULL) return rightType;

    /* decimal is stronger */
    if(leftType->type->tokenType == TYPE_DEF_DECIMAL) return leftType;
    if(rightType->type->tokenType == TYPE_DEF_DECIMAL) return rightType;
    
    if(leftType->type->tokenType == TYPE_DEF_INTEGER) return leftType;
    if(rightType->type->tokenType == TYPE_DEF_INTEGER) return rightType;

    printf("%d-%d Error - unsupported types for binary operator.",compilerContext.lineNumber,compilerContext.linePosition);
    exit(1);
    
    return NULL;
}







std::string AST_BinaryOperator::constructOutputString(HDExecutionContext *context)
{
   if(!left) return right->constructOutputString(context);
   if(!right) return left->constructOutputString(context);
  
//    printf("Operator binarny dostal operacje %d dla dwoch ponizszych wyrazen\n",op);
    AST_VariableDeclaration *leftTypeO = left->computeFinalType(context);
    AST_VariableDeclaration *rightTypeO = right->computeFinalType(context);
    bool leftCompound = leftTypeO->sizes != NULL;
    bool rightCompound = rightTypeO->sizes != NULL;
    int leftType = leftTypeO->type->tokenType;
    int rightType = rightTypeO->type->tokenType;
    
    switch (op) {
            
        /* I. Mathematical */
            
        case TYPE_POW:
      {
            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Base of power operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Exponent of power operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(rightCompound)
            {
                printf("%d-%d Error - Exponent of power operator cannot be an array.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            AST_Identifier *id = new AST_Identifier("pow");
            id->compilerContext = this->compilerContext;
            vector<AST_Expression *> *vecs = new vector<AST_Expression *>();
            vecs->push_back(left);
            vecs->push_back(right);
        
            AST_FunctionCall *fcall = new AST_FunctionCall(id,vecs);
            std::string out = fcall->constructOutputString(context);
        
        /* Those two l;ines are here to prevent deletion of fake function call from deleteing our left and right */
            delete fcall->args;
            fcall->args = new vector<AST_Expression *>();
        
            delete fcall;
            return out;
            break;
      }
        case TYPE_TENSOR_PRODUCT:
            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID)  { leftType = TYPE_DEF_DECIMAL; leftCompound = true;}
            if(rightType == TYPE_DEF_VOID) { rightType = TYPE_DEF_DECIMAL; rightCompound = true;}
            
            
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Left operand of tensor product operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Right operand of tensor product operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!rightCompound || !leftCompound)
            {
                printf("%d-%d Error - Operands of tensor product operator have to be tensors",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "tensorProduct("+left->constructOutputString(context)+", "+right->constructOutputString(context);
      case TYPE_AT:
        /* auto-casts in case of dynamic binding */
        if(leftType == TYPE_DEF_VOID)  { leftType = TYPE_DEF_DECIMAL; leftCompound = true;}
        if(rightType == TYPE_DEF_VOID) { rightType = TYPE_DEF_DECIMAL; rightCompound = true;}
        
        
        if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL))
        {
          printf("%d-%d Error - Left operand of matrix multiplication has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
          exit(1);
        }
        if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL))
        {
          printf("%d-%d Error - Right operand of matrix multiplication has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
          exit(1);
        }
        if(!rightCompound || !leftCompound)
        {
          printf("%d-%d Error - Operands of matrix multiplication operator have to be tensors",compilerContext.lineNumber,compilerContext.linePosition);
          exit(1);
        }
        return "matrixMultiply("+left->constructOutputString(context)+", "+right->constructOutputString(context);

        
        
        case TYPE_PLUS:
            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of + operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of + operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String addition does not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" + "+right->constructOutputString(context)+")";
            break;
        case TYPE_MINUS:
            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Left operand of - operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Right operand of - operator has to be numerical",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" - "+right->constructOutputString(context)+")";
            break;
        case TYPE_TIMES:
            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of * operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of * operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String multiplication does not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" * "+right->constructOutputString(context)+")";
            break;
        case TYPE_DIVIDED:
            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Left operand of / operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Right operand of / operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" / "+right->constructOutputString(context)+")";
            break;
       /* II. Comparison */
        case TYPE_EQUAL:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }

            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of == operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of == operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" == "+right->constructOutputString(context)+")";
            break;
        case TYPE_NEQUAL:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }

            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of != operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of != operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" != "+right->constructOutputString(context)+")";
            break;
        case TYPE_GREATER_T:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }

            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of > operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of > operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" > "+right->constructOutputString(context)+")";
            break;
        case TYPE_SMALLER_T:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }

            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of < operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of < operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" < "+right->constructOutputString(context)+")";
            break;
        case TYPE_SMALLER_EQUAL:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }

            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of <= operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of <= operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" <= "+right->constructOutputString(context)+")";
            break;
        case TYPE_GREATER_EQUAL:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            
            
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL || leftType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Left operand of >= operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL || rightType == TYPE_DEF_STRING))
            {
                printf("%d-%d Error - Right operand of >= operator has to be numerical or string.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if((leftType == TYPE_DEF_STRING || rightType == TYPE_DEF_STRING) && (leftCompound || rightCompound))
            {
                printf("%d-%d Error - String comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" >= "+right->constructOutputString(context)+")";
            break;
         /* III. Logic */
        case TYPE_AND:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }

            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Left operand of && operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Right operand of && operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" && "+right->constructOutputString(context)+")";
            break;
        case TYPE_OR:
            if(leftCompound || rightCompound)
            {
                printf("%d-%d Error - Comparisons do not work for arrays.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }

            /* auto-casts in case of dynamic binding */
            if(leftType == TYPE_DEF_VOID) leftType = TYPE_DEF_DECIMAL;
            if(rightType == TYPE_DEF_VOID) rightType = TYPE_DEF_DECIMAL;
            if(!(leftType == TYPE_DEF_INTEGER || leftType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Left operand of || operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            if(!(rightType == TYPE_DEF_INTEGER || rightType == TYPE_DEF_DECIMAL))
            {
                printf("%d-%d Error - Right operand of || operator has to be numerical.",compilerContext.lineNumber,compilerContext.linePosition);
                exit(1);
            }
            return "("+left->constructOutputString(context)+" || "+right->constructOutputString(context)+")";
            break;
        default:
                printf("%d-%d Error - Unknown binary operator %d.",compilerContext.lineNumber,compilerContext.linePosition,op);
                exit(1);
            break;
    }
    return std::string();
}



AST_BinaryOperator::~AST_BinaryOperator()
{
    if(left) delete left;
    if(right) delete right;
}

