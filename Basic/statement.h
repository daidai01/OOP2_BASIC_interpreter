/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

    /*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */


class controlClass
{
private:
    int type;
    /*
     * -1 END
     * 1 GOTO
     */
    int lineNumber;
public:
    explicit controlClass(int _type,int _lineNumber=0);
    int getType() const;
    int getLineNumber() const;
};

class REM:public Statement
{
public:
    REM();
    ~REM() override;
    void execute(EvalState &state) override;
};

class LET: public Statement
{
private:
    Expression *exp;

public:
    explicit LET(Expression *_exp);
    ~LET() override;
    void execute(EvalState &state) override;
};

class PRINT: public Statement
{
private:
    Expression *exp;

public:
    explicit PRINT(Expression *_exp);
    ~PRINT() override;
    void execute(EvalState &state) override;
};

class INPUT: public Statement
{
private:
    string var;

public:
    explicit INPUT(string _var);
    ~INPUT() override;
    void execute(EvalState &state) override;
};

class END:public Statement
{
public:
    END();
    ~END() override;
    void execute(EvalState &state) override;
};

class GOTO;

class IF: public Statement
{
private:
    Expression *exp1,*exp2;
    string cmp;
    int lineNumber;

public:
    IF(Expression *_exp1, string _cmp, Expression *_exp2, int _lineNumber);
    ~IF() override;
    void execute(EvalState &state) override;
};

class GOTO: public Statement
{
private:
    int lineNumber;

public:
    explicit GOTO(int _lineNumber);
    ~GOTO() override;
    void execute(EvalState &state) override;
};

#endif
