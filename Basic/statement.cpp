/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/simpio.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

controlClass::controlClass(int _type,int _lineNumber):type(_type),lineNumber(_lineNumber){}

int controlClass::getType() const
{
    return type;
}

int controlClass::getLineNumber() const
{
    return lineNumber;
}

REM::REM()=default;

REM::~REM()=default;

void REM::execute(EvalState &state){}

LET::LET(Expression *_exp):exp(_exp){}

LET::~LET()=default;

void LET::execute(EvalState &state)
{
    exp->eval(state);
}

PRINT::PRINT(Expression *_exp):exp(_exp){}

PRINT::~PRINT()=default;

void PRINT::execute(EvalState &state)
{
    cout<<exp->eval(state)<<endl;
}

INPUT::INPUT(string _var):var(_var){}

INPUT::~INPUT()=default;

void INPUT::execute(EvalState &state)
{
    int value;
    while(true)
    {
        try
        {
            value=stringToInteger(getLine("?"));
        } catch (...)
        {
            cout<<"INVALID NUMBER"<<endl;
            continue;
        }
        break;
    }
    state.setValue(var,value);
}

END::END()=default;

END::~END()=default;

void END::execute(EvalState &state)
{
    throw controlClass(-1);
}

IF::IF(Expression *_exp1, string _cmp, Expression *_exp2, int _lineNumber)
:  exp1(_exp1),cmp(_cmp),exp2(_exp2),lineNumber(_lineNumber){}

IF::~IF()=default;

void IF::execute(EvalState &state)
{
    int value1=exp1->eval(state);
    int value2=exp2->eval(state);
    bool flag=false;
    if(cmp=="=") flag=(value1==value2);
    else if(cmp==">") flag=(value1>value2);
    else if(cmp=="<") flag=(value1<value2);
    if(flag) throw controlClass(1,lineNumber);
}

GOTO::GOTO(int _lineNumber):lineNumber(_lineNumber){}

GOTO::~GOTO()=default;

void GOTO::execute(EvalState &state)
{
    throw controlClass(1,lineNumber);
}



