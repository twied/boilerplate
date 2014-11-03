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

#include "FrontEnd.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace lang0 {

using namespace ast;
using namespace ast::statement;
using namespace ast::expression;

static bool is_whitespace(const int c) {
    switch(c) {
    case ' ':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
        return true;
    default:
        return false;
    }
}

static bool is_alpha(const int c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

static bool is_numeral(const int c) {
    return c >= '0' && c <= '9';
}

const char* token_name(const Token& token) {
    switch(token) {
    case BRACKET_CURLY_LEFT:
        return "{";
    case BRACKET_CURLY_RIGHT:
        return "}";
    case BRACKET_ROUND_LEFT:
        return "(";
    case BRACKET_ROUND_RIGHT:
        return ")";
    case KEYWORD_CODE:
        return "CODE";
    case KEYWORD_ELSE:
        return "ELSE";
    case KEYWORD_FALSE:
        return "FALSE";
    case KEYWORD_IF:
        return "IF";
    case KEYWORD_PRINT:
        return "PRINT";
    case KEYWORD_TRUE:
        return "TRUE";
    case KEYWORD_VARIABLES:
        return "VARIABLES";
    case KEYWORD_WHILE:
        return "WHILE";
    case TOKEN_AND:
        return "&&";
    case TOKEN_ASSIGNMENT:
        return "=";
    case TOKEN_DIVIDE:
        return "/";
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_EQUALS:
        return "==";
    case TOKEN_GREATEROREQUAL:
        return ">=";
    case TOKEN_GREATERTHAN:
        return ">";
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_LESSOREQUAL:
        return "<=";
    case TOKEN_LESSTHAN:
        return "<";
    case TOKEN_MINUS:
        return "-";
    case TOKEN_MULTIPLY:
        return "*";
    case TOKEN_NOT:
        return "!";
    case TOKEN_NOTEQUALS:
        return "!=";
    case TOKEN_NUMERAL:
        return "NUMERAL";
    case TOKEN_OR:
        return "||";
    case TOKEN_PLUS:
        return "+";
    case TOKEN_SEMICOLON:
        return ";";
    default:
        return "???";
    }
}

Lexer::Lexer(istream* stream) : current_line(1), current_column(1), current_identifier(), current_char(' '), stream(stream) {
}

Lexer::~Lexer() {
}

void Lexer::get_char() {
    current_column += 1;

    const int next_char = stream->get();
    if(next_char == '\n' || next_char == '\r') {
        current_line += 1;
        current_column = 1;
    }
    current_char = next_char;
}

Token Lexer::operator()() {
    /* remove whitespace */
    while(is_whitespace(current_char)) {
        get_char();
    }

    /* end of file */
    if(current_char == -1) {
        return TOKEN_EOF;
    }

    /* comment */
    if(current_char == '#') {
        while(!!stream && current_char != '\n' && current_char != '\r') {
            get_char();
        }
        return operator()();
    }

    if(is_alpha(current_char)) {
        current_identifier.clear();
        while(is_alpha(current_char) || is_numeral(current_char)) {
            current_identifier += current_char;
            get_char();
        }

        if(current_identifier == "code") {
            return KEYWORD_CODE;
        } else if (current_identifier == "else") {
            return KEYWORD_ELSE;
        } else if (current_identifier == "false") {
            return KEYWORD_FALSE;
        } else if (current_identifier == "if") {
            return KEYWORD_IF;
        } else if (current_identifier == "print") {
            return KEYWORD_PRINT;
        } else if (current_identifier == "true") {
            return KEYWORD_TRUE;
        } else if (current_identifier == "variables") {
            return KEYWORD_VARIABLES;
        } else if (current_identifier == "while") {
            return KEYWORD_WHILE;
        } else {
            return TOKEN_IDENTIFIER;
        }
    }

    if(is_numeral(current_char)) {
        current_identifier.clear();
        while(is_numeral(current_char)) {
            current_identifier += current_char;
            get_char();
        }
        return TOKEN_NUMERAL;
    }

    const int last_char = current_char;
    get_char();
    switch(last_char) {
    case '{':
        return BRACKET_CURLY_LEFT;
    case '}':
        return BRACKET_CURLY_RIGHT;
    case '(':
        return BRACKET_ROUND_LEFT;
    case ')':
        return BRACKET_ROUND_RIGHT;
    case ';':
        return TOKEN_SEMICOLON;
    case '+':
        return TOKEN_PLUS;
    case '-':
        return TOKEN_MINUS;
    case '*':
        return TOKEN_MULTIPLY;
    case '/':
        return TOKEN_DIVIDE;
    case '&':
        if(current_char == '&') {
            get_char();
            return TOKEN_AND;
        } else {
            return TOKEN_UNKNOWN;
        }
    case '|':
        if(current_char == '|') {
            get_char();
            return TOKEN_OR;
        } else {
            return TOKEN_UNKNOWN;
        }
    case '>':
        if(current_char == '=') {
            get_char();
            return TOKEN_GREATEROREQUAL;
        } else {
            return TOKEN_GREATERTHAN;
        }
    case '<':
        if(current_char == '=') {
            get_char();
            return TOKEN_LESSOREQUAL;
        } else {
            return TOKEN_LESSTHAN;
        }
    case '=':
        if(current_char == '=') {
            get_char();
            return TOKEN_EQUALS;
        } else {
            return TOKEN_ASSIGNMENT;
        }
    case '!':
        if(current_char == '=') {
            get_char();
            return TOKEN_NOTEQUALS;
        } else {
            return TOKEN_NOT;
        }
    }
    return TOKEN_UNKNOWN;
}

Parser::Parser(istream* stream) : lexer(stream), current_token(lexer()) {
}

Parser::~Parser() {
}

Program* Parser::operator()() {
    vector<string> variables;
    vector<Statement*> statements;

    eat(KEYWORD_VARIABLES);
    eat(BRACKET_CURLY_LEFT);
    while(current_token != BRACKET_CURLY_RIGHT) {
        variables.push_back(parse_identifier());
        eat(TOKEN_SEMICOLON);
    }
    eat(BRACKET_CURLY_RIGHT);
    eat(KEYWORD_CODE);
    eat(BRACKET_CURLY_LEFT);
    while(current_token != BRACKET_CURLY_RIGHT) {
        statements.push_back(parse_statement());
    }
    eat(BRACKET_CURLY_RIGHT);
    expect(TOKEN_EOF);
    return new Program(lexer.current_line, lexer.current_column, variables, statements);
}

void Parser::expect(const Token& token) {
    if(current_token != token) {
        stringstream stream;
        stream << lexer.current_line << ":" << lexer.current_column << ": expected: " << token_name(token) << ", found: " << token_name(current_token) << "(\"" << lexer.current_identifier << "\")";
        throw stream.str();
    }
}

void Parser::eat(const Token& token) {
    expect(token);
    current_token = lexer();
}

string Parser::parse_identifier() {
    string identifier = lexer.current_identifier;
    eat(TOKEN_IDENTIFIER);
    return identifier;
}

int Parser::parse_numeral() {
    stringstream stream(lexer.current_identifier);
    int num;
    stream >> num;
    eat(TOKEN_NUMERAL);
    return num;
}

Statement* Parser::parse_statement() {
    switch(current_token) {
    case KEYWORD_IF:
        return parse_statement_if();
    case KEYWORD_PRINT:
        return parse_statement_print();
    case KEYWORD_WHILE:
        return parse_statement_while();
    default:
        return parse_statement_assignment();
    }
}

Statement* Parser::parse_statement_assignment() {
    string identifier = parse_identifier();
    eat(TOKEN_ASSIGNMENT);
    Expression* expression = parse_expression();
    eat(TOKEN_SEMICOLON);
    return new Assignment(lexer.current_line, lexer.current_column, identifier, expression);
}

Statement* Parser::parse_statement_if() {
    eat(KEYWORD_IF);
    eat(BRACKET_ROUND_LEFT);
    Expression* expression = parse_expression();
    eat(BRACKET_ROUND_RIGHT);
    eat(BRACKET_CURLY_LEFT);
    vector<Statement*> then_statements;
    while(current_token != BRACKET_CURLY_RIGHT) {
        then_statements.push_back(parse_statement());
    }
    eat(BRACKET_CURLY_RIGHT);
    vector<Statement*> else_statements;
    if(current_token == KEYWORD_ELSE) {
        eat(KEYWORD_ELSE);
        eat(BRACKET_CURLY_LEFT);
        while(current_token != BRACKET_CURLY_RIGHT) {
            else_statements.push_back(parse_statement());
        }
        eat(BRACKET_CURLY_RIGHT);
    }
    return new If(lexer.current_line, lexer.current_column, expression, then_statements, else_statements);
}

Statement* Parser::parse_statement_print() {
    eat(KEYWORD_PRINT);
    Expression* expression = parse_expression();
    eat(TOKEN_SEMICOLON);
    return new Print(lexer.current_line, lexer.current_column, expression);
}

Statement* Parser::parse_statement_while() {
    eat(KEYWORD_WHILE);
    eat(BRACKET_ROUND_LEFT);
    Expression* expression = parse_expression();
    eat(BRACKET_ROUND_RIGHT);
    eat(BRACKET_CURLY_LEFT);
    vector<Statement*> statements;
    while(current_token != BRACKET_CURLY_RIGHT) {
        statements.push_back(parse_statement());
    }
    eat(BRACKET_CURLY_RIGHT);
    return new While(lexer.current_line, lexer.current_column, expression, statements);
}

Expression* Parser::parse_expression() {
    Expression* lhs = parse_expression_operand();
    switch(current_token) {
    case TOKEN_EQUALS:
        eat(TOKEN_EQUALS);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::EQU, lhs, parse_expression_operand());
    case TOKEN_NOTEQUALS:
        eat(TOKEN_NOTEQUALS);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::NEQ, lhs, parse_expression_operand());
    case TOKEN_LESSTHAN:
        eat(TOKEN_LESSTHAN);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::LES, lhs, parse_expression_operand());
    case TOKEN_LESSOREQUAL:
        eat(TOKEN_LESSOREQUAL);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::LEQ, lhs, parse_expression_operand());
    case TOKEN_GREATERTHAN:
        eat(TOKEN_GREATERTHAN);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::GRT, lhs, parse_expression_operand());
    case TOKEN_GREATEROREQUAL:
        eat(TOKEN_GREATEROREQUAL);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::GEQ, lhs, parse_expression_operand());
    default:
        return lhs;
    }
}

Expression* Parser::parse_expression_operand() {
    Expression* lhs = parse_expression_term();
    switch(current_token) {
    case TOKEN_OR:
        eat(TOKEN_OR);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::OR, lhs, parse_expression_term());
    case TOKEN_PLUS:
        eat(TOKEN_PLUS);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::ADD, lhs, parse_expression_term());
    case TOKEN_MINUS:
        eat(TOKEN_MINUS);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::SUB, lhs, parse_expression_term());
    default:
        return lhs;
    }
}

Expression* Parser::parse_expression_term() {
    Expression* lhs = parse_expression_unary();
    switch(current_token) {
    case TOKEN_AND:
        eat(TOKEN_AND);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::AND, lhs, parse_expression_unary());
    case TOKEN_MULTIPLY:
        eat(TOKEN_MULTIPLY);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::MUL, lhs, parse_expression_unary());
    case TOKEN_DIVIDE:
        eat(TOKEN_DIVIDE);
        return new BinOp(lexer.current_line, lexer.current_column, BinOp::DIV, lhs, parse_expression_unary());
    default:
        return lhs;
    }
}

Expression* Parser::parse_expression_unary() {
    if(current_token == TOKEN_MINUS) {
        eat(TOKEN_MINUS);
        return new UnOp(lexer.current_line, lexer.current_column, UnOp::NEG, parse_expression_factor());
    } else if(current_token == TOKEN_NOT) {
        eat(TOKEN_NOT);
        return new UnOp(lexer.current_line, lexer.current_column, UnOp::NOT, parse_expression_factor());
    } else {
        return parse_expression_factor();
    }
}

Expression* Parser::parse_expression_factor() {
    if(current_token == TOKEN_NUMERAL) {
        return new Numeral(lexer.current_line, lexer.current_column, parse_numeral());
    } else if (current_token == TOKEN_IDENTIFIER) {
        return new Identifier(lexer.current_line, lexer.current_column, parse_identifier());
    } else if (current_token == KEYWORD_TRUE) {
        return new Numeral(lexer.current_line, lexer.current_column, 1);
    } else if (current_token == KEYWORD_FALSE) {
        return new Numeral(lexer.current_line, lexer.current_column, 0);
    } else {
        eat(BRACKET_ROUND_LEFT);
        Expression* expression = parse_expression();
        eat(BRACKET_ROUND_RIGHT);
        return expression;
    }
}

} /* namespace lang0 */
