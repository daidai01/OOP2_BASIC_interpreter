/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Yiyang Ling
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
 //  cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cout << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);
   string firstToken;
   if(scanner.hasMoreTokens()) firstToken=scanner.nextToken();
   else return;
   TokenType firstType=scanner.getTokenType(firstToken);
   if(firstType==NUMBER)
   {
       int lineNumber=stringToInteger(firstToken);
       if(scanner.hasMoreTokens())
       {
           try
           {
               program.addSourceLine(lineNumber,line);
           } catch (ErrorException &ex)
           {
                program.removeSourceLine(lineNumber);
                throw ErrorException(ex.getMessage());
           }
       }
       else program.removeSourceLine(lineNumber);
   }
   else if(!scanner.hasMoreTokens())
   {
       if(firstToken=="RUN") program.run(state);
       else if(firstToken=="LIST") program.list();
       else if(firstToken=="CLEAR")
       {
           program.clear();
           state.clear();
       }
       else if(firstToken=="QUIT") exit(0);
       else if(firstToken=="HELP") Program::help();
       else cout<<"SYNTAX ERROR"<<endl;
   }
   else if(firstToken=="PRINT")
   {
       Expression *exp;
       exp=parseExp(scanner);
       int value=exp->eval(state);
       cout<<value<<endl;
       delete exp;
   }
   else if(firstToken=="LET")
   {
       string var=scanner.nextToken();
       if(EvalState::isReserved(var)) throw ErrorException("SYNTAX ERROR");
       string equalSign=scanner.nextToken();
       Expression *exp;
       exp=parseExp(scanner);
       int value=exp->eval(state);
       state.setValue(var,value);
       delete exp;
   }
   else if(firstToken=="INPUT")
   {
       string var=scanner.nextToken();
       if(scanner.hasMoreTokens()||EvalState::isReserved(var))
           throw ErrorException("SYNTAX ERROR");
       int value;
       string svalue;
       while(true)
       {
           try
           {
               svalue=getLine("?");
               value=stringToInteger(svalue);
           }
           catch(...)
           {
               cout<<"INVALID NUMBER"<<endl;
               continue;
           }
           break;
       }
       state.setValue(var,value);
   }
   else throw ErrorException("SYNTAX ERROR");
}
