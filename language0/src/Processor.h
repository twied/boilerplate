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

#ifndef PROCESSORS_H_
#define PROCESSORS_H_

#include "SyntaxTree.h"

#include <string>
#include <vector>

namespace lang0 {

namespace processor {

class VariableUsage : public ast::Walker {
public:
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
    std::vector<std::string> variables;
};

class ConstantFolding : public ast::Walker {
public:
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
    void optimize(ast::Expression** node);
};

class DeadCodeRemoval : public ast::Walker {
public:
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
    std::vector<ast::Statement*> statements;
};

} /* namespace processor */
} /* namespace lang0 */

#endif /* PROCESSORS_H_ */
