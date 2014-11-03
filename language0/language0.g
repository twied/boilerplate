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

grammar language0;

program
    : block_variables? block_code EOF
    ;

block_variables
    : 'variables' '{' (Identifier ';')+ '}'
    ;


block_code
    : 'code' '{' statement* '}'
    ;

statement
    : statement_assignment
    | statement_if
    | statement_print
    | statement_while
    ;

statement_assignment
    : Identifier '=' expression ';'
    ;

statement_if
    : 'if' '(' expression ')' '{' statement* '}' ('else' '{' statement* '}')?
    ;

statement_print
    : 'print' expression ';'
    ;

statement_while
    : 'while' '(' expression ')' '{' statement* '}'
    ;

expression
    : expression_relation
    ;
    
expression_relation
    : expression_operand (operator_relation expression_operand)?
    ;

expression_operand
    : expression_term (operator_disjunction expression_term)?
    ;

expression_term
    : expression_unary (operator_conjunction expression_unary)?
    ;

expression_unary
    : operator_unary? expression_factor
    ;

expression_factor
    : Identifier
    | Numeral
    | 'true'
    | 'false'
    | '(' expression ')'
    ;
    
operator_conjunction
    : '&&'
    | '*'
    | '/'
    ;

operator_disjunction
    : '||'
    | '+'
    | '-'
    ;

operator_relation
    : '=='
    | '!='
    | '<'
    | '<='
    | '>'
    | '>='
    ;

operator_unary
    : '!'
    | '-'
    ;

Identifier
    : Letter (Letter | Digit)*
    ;

Numeral
    : Digit Digit*
    ;

fragment Letter
    : 'a'..'z'
    | 'A'..'Z';

fragment Digit
    : '0'..'9'
    ;
    
Whitespace
    :  (' ' | '\f' | '\n' | '\r'| '\t')+ {$channel = HIDDEN;}
    ;

Comment
    : '#' (~('\n'|'\r'))* ('\n'|'\r'('\n')?)? {$channel = HIDDEN;}
    ;
