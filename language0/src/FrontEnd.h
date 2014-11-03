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


#ifndef FRONTEND_H_
#define FRONTEND_H_

#include "SyntaxTree.h"

namespace lang0 {

enum Token {
    BRACKET_CURLY_LEFT,
    BRACKET_CURLY_RIGHT,
    BRACKET_ROUND_LEFT,
    BRACKET_ROUND_RIGHT,
    KEYWORD_CODE,
    KEYWORD_ELSE,
    KEYWORD_FALSE,
    KEYWORD_IF,
    KEYWORD_PRINT,
    KEYWORD_TRUE,
    KEYWORD_VARIABLES,
    KEYWORD_WHILE,
    TOKEN_AND,
    TOKEN_ASSIGNMENT,
    TOKEN_DIVIDE,
    TOKEN_EOF,
    TOKEN_EQUALS,
    TOKEN_GREATEROREQUAL,
    TOKEN_GREATERTHAN,
    TOKEN_IDENTIFIER,
    TOKEN_LESSOREQUAL,
    TOKEN_LESSTHAN,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_NOT,
    TOKEN_NOTEQUALS,
    TOKEN_NUMERAL,
    TOKEN_OR,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_UNKNOWN
};

const char* token_name(const Token& token);

class Lexer {
public:
    Lexer(std::istream* stream);
    ~Lexer();

    Token operator()();
    unsigned current_line;
    unsigned current_column;
    std::string current_identifier;
private:
    void get_char();
    int current_char;
    std::istream* stream;
};

class Parser {
public:
    Parser(std::istream* stream);
    ~Parser();

    ast::Program* operator()();

private:
    Lexer lexer;
    Token current_token;

    void expect(const Token& token);
    void eat(const Token& token);

    std::string parse_identifier();
    int parse_numeral();

    ast::Statement* parse_statement();
    ast::Statement* parse_statement_assignment();
    ast::Statement* parse_statement_if();
    ast::Statement* parse_statement_print();
    ast::Statement* parse_statement_while();

    ast::Expression* parse_expression();
    ast::Expression* parse_expression_operand();
    ast::Expression* parse_expression_term();
    ast::Expression* parse_expression_unary();
    ast::Expression* parse_expression_factor();
};

} /* namespace lang0 */

#endif /* FRONTEND_H_ */
