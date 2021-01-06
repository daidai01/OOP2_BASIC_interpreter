/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "parser.h"
#include "../StanfordCPPLib/error.h"
using namespace std;

Program::Program() =default;

Program::~Program() {
   clear();
}

void Program::clear() {
    for(auto it=allLines.begin();it!=allLines.end();++it)
        delete it->second.statement;
    allLines.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    if(allLines.count(lineNumber)) removeSourceLine(lineNumber);
    Line newLine;
    newLine.sourceLine=line;
    newLine.statement= nullptr;
    allLines[lineNumber]=newLine;
    setParsedStatement(lineNumber,convert(line));
}

void Program::removeSourceLine(int lineNumber) {
    auto it=allLines.find(lineNumber);
    if(it!=allLines.end())
    {
        delete it->second.statement;
        allLines.erase(lineNumber);
    }
}

string Program::getSourceLine(int lineNumber) {
    auto it=allLines.find(lineNumber);
    return it->second.sourceLine;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    allLines[lineNumber].statement=stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    auto it=allLines.find(lineNumber);
    return it->second.statement;
}

int Program::getFirstLineNumber() {
    return allLines.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it=allLines.find(lineNumber);
    ++it;
    if(it==allLines.end()) return -1;
    return it->first;
}

void Program::help()
{
    cout<<"BASIC interpreter written by daidai"<<endl;
    cout<<"commands:"<<endl;
    cout<<"RUN:This command starts program execution beginning at the lowest-numbered line."<<endl;
    cout<<"LIST:This command lists the steps in the program in numerical sequence."<<endl;
    cout<<"CLEAR:This command deletes all program and variables."<<endl;
    cout<<"QUIT:This command exits from the BASIC interpreter by calling exit(0)."<<endl;
    cout<<"HELP:This command provides a simple help message describing your interpreter."<<endl;
    cout<<"Error Reporting:"<<endl;
    cout<<"DIVIDE BY ZERO:Calculating some number divide by zero."<<endl;
    cout<<"INVALID NUMBER:User types wrong value to answer INPUT statement."<<endl;
    cout<<"VARIABLE NOT DEFINED:A variable used before assigned it."<<endl;
    cout<<"LINE NUMBER ERROR:GOTO statement's line number not exist."<<endl;
    cout<<"SYNTAX ERROR:Any other errors."<<endl;
}

void Program::run(EvalState &state)
{
    int curNumber=getFirstLineNumber();
    while(true)
    {
        if(curNumber==-1) break;
        try
        {
            getParsedStatement(curNumber)->execute(state);
        }
        catch (controlClass ctrl)
        {
            if(ctrl.getType()==-1) break;
            else if(ctrl.getType()==1)
            {
                if(allLines.count(ctrl.getLineNumber()))
                {
                    curNumber=ctrl.getLineNumber();
                    continue;
                }
                else cout<<"LINE NUMBER ERROR"<<endl;
            }
        }
        catch (ErrorException err)
        {
            cout<<err.getMessage()<<endl;
        }
        catch (...)
        {

        }
        curNumber=getNextLineNumber(curNumber);
    }
}

void Program::list()
{
    for(auto it=allLines.begin();it!=allLines.end();++it)
        cout<<(it->second).sourceLine<<endl;
}

Statement *convert(string line)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    string slineNumner=scanner.nextToken();
    string command=scanner.nextToken();
    if(command=="REM") return new REM;
    else if(command=="LET")
    {
        Expression *exp;
        exp=parseExp(scanner);
        return new LET(exp);
    }
    else if(command=="PRINT")
    {
        Expression *exp;
        exp=parseExp(scanner);
        return new PRINT(exp);
    }
    else if(command=="INPUT")
    {
        string var=scanner.nextToken();
        return new INPUT(var);
    }
    else if(command=="END") return new END;
    else if(command=="IF")
    {
        int cmpPosition=-1;
        for(int i=0;i<line.size();++i)
        {
            if(line[i]=='='||line[i]=='<'||line[i]=='>')
            {
                cmpPosition=i;
                break;
            }
        }
        if(cmpPosition==-1) throw ErrorException("SYNTAX ERROR");
        string line1,line2,str1,str2;
        Expression  *_exp1,*_exp2;
        for(int i=0;i<cmpPosition;++i) line1+=line[i];
        TokenScanner _scanner;
        _scanner.ignoreWhitespace();
        _scanner.scanNumbers();
        _scanner.setInput(line1);
        str1=_scanner.nextToken();
        str1=_scanner.nextToken();
        _exp1=readE(_scanner);
        for(int i=cmpPosition;i<line.size();++i) line2+=line[i];
        _scanner.setInput(line2);
        string _cmp=_scanner.nextToken();
        _exp2=readE(_scanner);
        str2=_scanner.nextToken();
        str2=_scanner.nextToken();
      //  cout<<str2<<endl;
        int _lineNumber=stringToInteger(str2);
        return new IF(_exp1,_cmp,_exp2,_lineNumber);
    }
    else if(command=="GOTO")
    {
        string slineNumber=scanner.nextToken();
        return new GOTO(stringToInteger(slineNumber));
    }
    else throw ErrorException("SYNTAX ERROR");
}

