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

#ifndef BACKEND_H_
#define BACKEND_H_

#include "SyntaxTree.h"

#include <map>
#include <ostream>

namespace lang0 {

namespace backend {

class CC0 : public ast::Walker {
public:
    CC0(std::ostream* stream);
    ~CC0();

    void* process(ast::Program* node);
    void* process(ast::statement::Assignment* node);
    void* process(ast::statement::If* node);
    void* process(ast::statement::Print* node);
    void* process(ast::statement::While* node);
    void* process(ast::expression::BinOp* node);
    void* process(ast::expression::Identifier* node);
    void* process(ast::expression::Numeral* node);
    void* process(ast::expression::UnOp* node);
private:
    std::ostream* stream;
    std::map<std::string, unsigned> variables;
};

class G32 : public ast::Walker {
public:
    G32(std::ostream* stream);
    ~G32();

    void* process(ast::Program* node);
    void* process(ast::statement::Assignment* node);
    void* process(ast::statement::If* node);
    void* process(ast::statement::Print* node);
    void* process(ast::statement::While* node);
    void* process(ast::expression::BinOp* node);
    void* process(ast::expression::Identifier* node);
    void* process(ast::expression::Numeral* node);
    void* process(ast::expression::UnOp* node);
private:
    std::ostream* stream;
};

class DOT : public ast::Walker {
public:
    DOT(std::ostream* stream);
    ~DOT();

    void* process(ast::Program* node);
    void* process(ast::statement::Assignment* node);
    void* process(ast::statement::If* node);
    void* process(ast::statement::Print* node);
    void* process(ast::statement::While* node);
    void* process(ast::expression::BinOp* node);
    void* process(ast::expression::Identifier* node);
    void* process(ast::expression::Numeral* node);
    void* process(ast::expression::UnOp* node);
private:
    std::ostream* stream;

    template<typename T> void emit_node(void* node, const T& label);
    void emit_edge(void* from_node, void* to_node, const std::string& label = "");
};

} /* namespace backend */

} /* namespace lang0 */

#endif /* BACKEND_H_ */
