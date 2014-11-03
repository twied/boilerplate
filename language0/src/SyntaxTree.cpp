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

#include "SyntaxTree.h"

#include <cstdarg>
#include <map>
#include <sstream>

using namespace std;

namespace lang0 {

namespace ast {

Node::Node(const unsigned& source_line, const unsigned& source_column) : source_line(source_line), source_column(source_column) {
}

Node::~Node() {
}

Expression::Expression(const unsigned& source_line, const unsigned& source_column) : Node(source_line, source_column) {
}

Expression::~Expression() {
}

Statement::Statement(const unsigned& source_line, const unsigned& source_column) : Node(source_line, source_column) {
}

Statement::~Statement() {
}

Program::Program(const unsigned& source_line, const unsigned& source_column, const vector<string>& variables, const vector<Statement*> statements) : Node(source_line, source_column), variables(variables), statements(statements) {
}

Program::~Program() {
    for(vector<Statement*>::iterator it = statements.begin(); it != statements.end(); ++it) {
        delete *it;
    }
}

void* Program::process(Walker* walker) {
    return walker->process(this);
}

namespace expression {

Identifier::Identifier(const unsigned& source_line, const unsigned& source_column, const std::string& identifier) : Expression(source_line, source_column), identifier(identifier) {
}

Identifier::~Identifier() {
}

void* Identifier::process(Walker* walker) {
    return walker->process(this);
}

Numeral::Numeral(const unsigned& source_line, const unsigned& source_column, const std::string& identifier) : Expression(source_line, source_column), numeral(0) {
    stringstream stream(identifier);
    stream >> numeral;
}

Numeral::Numeral(const unsigned& source_line, const unsigned& source_column, const int& numeral) : Expression(source_line, source_column), numeral(numeral) {
}

Numeral::~Numeral() {
}

void* Numeral::process(Walker* walker) {
    return walker->process(this);
}

UnOp::UnOp(const unsigned& source_line, const unsigned& source_column, const Operator& oper, Expression* expression) : Expression(source_line, source_column), oper(oper), expression(expression) {
}

UnOp::~UnOp() {
    delete expression;
}

void* UnOp::process(Walker* walker) {
    return walker->process(this);
}

BinOp::BinOp(const unsigned& source_line, const unsigned& source_column, const Operator& oper, Expression* lhs_expression, Expression* rhs_expression) : Expression(source_line, source_column), oper(oper), lhs_expression(lhs_expression), rhs_expression(rhs_expression) {
}

BinOp::~BinOp() {
    delete lhs_expression;
    delete rhs_expression;
}

void* BinOp::process(Walker* walker) {
    return walker->process(this);
}

} /* namespace expression */

namespace statement {

Assignment::Assignment(const unsigned& source_line, const unsigned& source_column, const std::string& identifier, Expression* expression) : Statement(source_line, source_column), identifier(identifier), expression(expression) {
}

Assignment::~Assignment() {
    delete expression;
}

void* Assignment::process(Walker* walker) {
    return walker->process(this);
}

If::If(const unsigned& source_line, const unsigned& source_column, Expression* expression, const std::vector<Statement*>& then_statements, const std::vector<Statement*>& else_statements) : Statement(source_line, source_column), expression(expression), then_statements(then_statements), else_statements(else_statements) {
}

If::~If() {
    delete expression;
    for(vector<Statement*>::iterator it = then_statements.begin(); it != then_statements.end(); ++it) {
        delete *it;
    }
    for(vector<Statement*>::iterator it = else_statements.begin(); it != else_statements.end(); ++it) {
        delete *it;
    }
}

void* If::process(Walker* walker) {
    return walker->process(this);
}

Print::Print(const unsigned& source_line, const unsigned& source_column, Expression* expression) : Statement(source_line, source_column), expression(expression) {
}

Print::~Print() {
    delete expression;
}

void* Print::process(Walker* walker) {
    return walker->process(this);
}

While::While(const unsigned& source_line, const unsigned& source_column, Expression* expression, const std::vector<Statement*>& statements) : Statement(source_line, source_column), expression(expression), statements(statements) {
}

While::~While() {
    delete expression;
    for(vector<Statement*>::iterator it = statements.begin(); it != statements.end(); ++it) {
        delete *it;
    }
}

void* While::process(Walker* walker) {
    return walker->process(this);
}

} /* namespace statement */

Walker::~Walker() {
}

string Walker::unique_name(void* data) {
    static map<void*, string> id_map;
    static unsigned next_id = 0;

    if(id_map.find(data) == id_map.end()) {
        stringstream stream;
        stream << "label_" << next_id++;
        return id_map[data] = stream.str();
    } else {
        return id_map[data];
    }
}

string Walker::exception(const unsigned& line, const unsigned& column, const std::string& message) {
    stringstream stream;
    stream << line << ":" << column << ": " << message;
    return stream.str();
}

} /* namespace ast */

} /* namespace lang0 */
