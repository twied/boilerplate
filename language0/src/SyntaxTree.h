/*
 * language0: A very simple programming language.
 * Copyright (C) 2014 Tim Wiederhake
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */


#ifndef SYNTAXTREE_H_
#define SYNTAXTREE_H_

#include <string>
#include <vector>

namespace lang0 {

namespace ast {

class Walker;

struct Node {
    Node(const unsigned& source_line, const unsigned& source_column);
    virtual ~Node() = 0;

    unsigned source_line;
    unsigned source_column;
};

struct Expression : public Node {
    Expression(const unsigned& source_line, const unsigned& source_column);
    virtual ~Expression() = 0;
    virtual void* process(Walker* walker) = 0;
};


struct Statement : public Node {
    Statement(const unsigned& source_line, const unsigned& source_column);
    virtual ~Statement() = 0;
    virtual void* process(Walker* walker) = 0;
};

struct Program : public Node {
    std::vector<std::string> variables;
    std::vector<Statement*> statements;


    Program(const unsigned& source_line, const unsigned& source_column, const std::vector<std::string>& variables, const std::vector<Statement*> statements);
    ~Program();
    void* process(Walker* walker);
};

namespace expression {

struct Identifier : public Expression {
    std::string identifier;

    Identifier(const unsigned& source_line, const unsigned& source_column, const std::string& identifier);
    ~Identifier();
    void* process(Walker* walker);
};

struct Numeral : public Expression {
    int numeral;

    Numeral(const unsigned& source_line, const unsigned& source_column, const std::string& identifier);
    Numeral(const unsigned& source_line, const unsigned& source_column, const int& numeral);
    ~Numeral();
    void* process(Walker* walker);
};

struct UnOp : public Expression {
    enum Operator {
        NOT,
        NEG
    };

    Operator oper;
    Expression* expression;

    UnOp(const unsigned& source_line, const unsigned& source_column, const Operator& oper, Expression* expression);
    ~UnOp();
    void* process(Walker* walker);
};

struct BinOp : public Expression {
    enum Operator {
        ADD,    /* +  */
        SUB,    /* -  */
        MUL,    /* *  */
        DIV,    /* /  */
        AND,    /* && */
        OR,     /* || */
        EQU,    /* == */
        NEQ,    /* != */
        LES,    /* <  */
        LEQ,    /* <= */
        GRT,    /* >  */
        GEQ     /* >= */
    };

    Operator oper;
    Expression* lhs_expression;
    Expression* rhs_expression;

    BinOp(const unsigned& source_line, const unsigned& source_column, const Operator& oper, Expression* lhs_expression, Expression* rhs_expression);
    ~BinOp();
    void* process(Walker* walker);
};

} /* namespace expression */

namespace statement {

struct Assignment : public Statement {
    std::string identifier;
    Expression* expression;

    Assignment(const unsigned& source_line, const unsigned& source_column, const std::string& identifier, Expression* expression);
    ~Assignment();
    void* process(Walker* walker);
};

struct If : public Statement {
    Expression* expression;
    std::vector<Statement*> then_statements;
    std::vector<Statement*> else_statements;

    If(const unsigned& source_line, const unsigned& source_column, Expression* expression, const std::vector<Statement*>& then_statements, const std::vector<Statement*>& else_statements);
    ~If();
    void* process(Walker* walker);
};

struct Print : public Statement {
    Expression* expression;

    Print(const unsigned& source_line, const unsigned& source_column, Expression* expression);
    ~Print();
    void* process(Walker* walker);
};

struct While : public Statement {
    Expression* expression;
    std::vector<Statement*> statements;

    While(const unsigned& source_line, const unsigned& source_column, Expression* expression, const std::vector<Statement*>& statements);
    ~While();
    void* process(Walker* walker);
};

} /* namespace statement */

class Walker {
public:
    virtual ~Walker();
    virtual void* process(ast::Program* node) = 0;
    virtual void* process(ast::statement::Assignment* node) = 0;
    virtual void* process(ast::statement::If* node) = 0;
    virtual void* process(ast::statement::Print* node) = 0;
    virtual void* process(ast::statement::While* node) = 0;
    virtual void* process(ast::expression::BinOp* node) = 0;
    virtual void* process(ast::expression::Identifier* node) = 0;
    virtual void* process(ast::expression::Numeral* node) = 0;
    virtual void* process(ast::expression::UnOp* node) = 0;
    std::string exception(const unsigned& line, const unsigned& column, const std::string& message);
protected:
    std::string unique_name(void* data);
};

} /* namespace ast */

} /* namespace lang0 */

#endif /* SYNTAXTREE_H_ */
