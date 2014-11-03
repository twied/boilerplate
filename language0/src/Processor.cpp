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


#include "Processor.h"

using namespace std;

namespace lang0 {

using namespace ast;
using namespace ast::expression;
using namespace ast::statement;

namespace processor {

void* VariableUsage::process(ast::Program* node) {
    for(size_t i = 0; i < node->variables.size(); ++i) {
        for(size_t j = i + 1; j < node->variables.size(); ++j) {
            if(node->variables[i] == node->variables[j]) {
                throw string("duplicate variable " + node->variables[i]);
            }
            variables.push_back(node->variables[i]);
        }
    }
    return NULL;
}

void* VariableUsage::process(ast::statement::Assignment* node) {
    node->expression->process(this);
    return NULL;
}

void* VariableUsage::process(ast::statement::If* node) {
    node->expression->process(this);
    for(vector<Statement*>::iterator it = node->then_statements.begin(); it != node->then_statements.end(); ++it) {
        (*it)->process(this);
    }
    for(vector<Statement*>::iterator it = node->else_statements.begin(); it != node->else_statements.end(); ++it) {
        (*it)->process(this);
    }
    return NULL;
}

void* VariableUsage::process(ast::statement::Print* node) {
    node->expression->process(this);
    return NULL;
}

void* VariableUsage::process(ast::statement::While* node) {
    node->expression->process(this);
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }
    return NULL;
}

void* VariableUsage::process(ast::expression::BinOp* node) {
    node->lhs_expression->process(this);
    node->rhs_expression->process(this);
    return NULL;
}

void* VariableUsage::process(ast::expression::Identifier* node) {
    for(vector<string>::iterator it = variables.begin(); it != variables.end(); ++it) {
        if(*it == node->identifier) {
            return NULL;
        }
    }
    throw exception(node->source_line, node->source_column, "unknown variable" + node->identifier);
}

void* VariableUsage::process(ast::expression::Numeral*) {
    return NULL;
}

void* VariableUsage::process(ast::expression::UnOp* node) {
    node->expression->process(this);
    return NULL;
}

void* ConstantFolding::process(Program* node) {
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }
    return NULL;
}

void* ConstantFolding::process(Assignment* node) {
    optimize(&node->expression);
    return NULL;
}

void* ConstantFolding::process(If* node) {
    optimize(&node->expression);
    for(vector<Statement*>::iterator it = node->then_statements.begin(); it != node->then_statements.end(); ++it) {
        (*it)->process(this);
    }
    for(vector<Statement*>::iterator it = node->else_statements.begin(); it != node->else_statements.end(); ++it) {
        (*it)->process(this);
    }
    return NULL;
}

void* ConstantFolding::process(Print* node) {
    optimize(&node->expression);
    return NULL;
}

void* ConstantFolding::process(While* node) {
    optimize(&node->expression);
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }
    return NULL;
}

void* ConstantFolding::process(BinOp* node) {
    optimize(&node->lhs_expression);
    optimize(&node->rhs_expression);

    int* ptr_lhs = (int*) node->lhs_expression->process(this);
    int* ptr_rhs = (int*) node->rhs_expression->process(this);

    if(ptr_lhs == NULL || ptr_rhs == NULL) {
        return NULL;
    }

    switch(node->oper) {
    case BinOp::ADD:
        *ptr_lhs = *ptr_lhs + *ptr_rhs;
        return ptr_lhs;
    case BinOp::SUB:
        *ptr_lhs = *ptr_lhs - *ptr_rhs;
        return ptr_lhs;
    case BinOp::MUL:
        *ptr_lhs = *ptr_lhs * *ptr_rhs;
        return ptr_lhs;
    case BinOp::DIV:
        *ptr_lhs = *ptr_lhs / *ptr_rhs;
        return ptr_lhs;
    case BinOp::AND:
        *ptr_lhs = *ptr_lhs && *ptr_rhs;
        return ptr_lhs;
    case BinOp::OR:
        *ptr_lhs = *ptr_lhs || *ptr_rhs;
        return ptr_lhs;
    case BinOp::EQU:
        *ptr_lhs = *ptr_lhs == *ptr_rhs;
        return ptr_lhs;
    case BinOp::NEQ:
        *ptr_lhs = *ptr_lhs != *ptr_rhs;
        return ptr_lhs;
    case BinOp::LES:
        *ptr_lhs = *ptr_lhs < *ptr_rhs;
        return ptr_lhs;
    case BinOp::LEQ:
        *ptr_lhs = *ptr_lhs <= *ptr_rhs;
        return ptr_lhs;
    case BinOp::GRT:
        *ptr_lhs = *ptr_lhs > *ptr_rhs;
        return ptr_lhs;
    case BinOp::GEQ:
        *ptr_lhs = *ptr_lhs >= *ptr_rhs;
        return ptr_lhs;
    default:
        throw exception(node->source_line, node->source_column, "unknown binop operator");
    }
}

void* ConstantFolding::process(Identifier*) {
    return NULL;
}

void* ConstantFolding::process(Numeral* node) {
    return &node->numeral;
}

void* ConstantFolding::process(UnOp* node) {
    optimize(&node->expression);
    int* ptr = (int*) node->expression->process(this);
    if(ptr != NULL) {
        switch(node->oper) {
        case UnOp::NEG:
            *ptr = - *ptr;
            return ptr;
        case UnOp::NOT:
            *ptr = ! *ptr;
            return ptr;
        default:
            throw exception(node->source_line, node->source_column, "unknown unop operator");
        }
    }
    return ptr;
}

void ConstantFolding::optimize(Expression** node) {
    int* ptr = (int*) (*node)->process(this);
    if(ptr == NULL) {
        return;
    }

    int line = (*node)->source_line;
    int column = (*node)->source_column;
    int value = *ptr;

    delete *node;
    *node = new Numeral(line, column, value);
}

void* DeadCodeRemoval::process(Program* node) {
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }
    node->statements.assign(statements.begin(), statements.end());
    return NULL;
}

void* DeadCodeRemoval::process(Assignment* node) {
    statements.push_back(node);
    return NULL;
}

void* DeadCodeRemoval::process(If* node) {
    DeadCodeRemoval body_then;
    for(vector<Statement*>::iterator it = node->then_statements.begin(); it != node->then_statements.end(); ++it) {
        (*it)->process(&body_then);
    }
    node->then_statements.assign(body_then.statements.begin(), body_then.statements.end());

    DeadCodeRemoval body_else;
    for(vector<Statement*>::iterator it = node->else_statements.begin(); it != node->else_statements.end(); ++it) {
        (*it)->process(&body_else);
    }
    node->else_statements.assign(body_else.statements.begin(), body_else.statements.end());

    int* ptr = (int*) node->expression->process(this);
    if(ptr == NULL) {
        statements.push_back(node);
    } else if(*ptr == 0) {
        statements.insert(statements.end(), body_else.statements.begin(), body_else.statements.end());
        node->else_statements.clear();
        delete node;
    } else {
        statements.insert(statements.end(), body_then.statements.begin(), body_then.statements.end());
        node->then_statements.clear();
        delete node;
    }
    return NULL;
}

void* DeadCodeRemoval::process(Print* node) {
    statements.push_back(node);
    return NULL;
}

void* DeadCodeRemoval::process(While* node) {
    DeadCodeRemoval body;
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(&body);
    }
    node->statements.assign(body.statements.begin(), body.statements.end());

    int* ptr = (int*) node->expression->process(this);
    if(ptr == NULL || (ptr != NULL && *ptr != 0)) {
        statements.push_back(node);
    } else {
        delete node;
    }
    return NULL;
}

void* DeadCodeRemoval::process(BinOp*) {
    return NULL;
}

void* DeadCodeRemoval::process(Identifier*) {
    return NULL;
}

void* DeadCodeRemoval::process(Numeral* node) {
    return &node->numeral;
}

void* DeadCodeRemoval::process(UnOp*) {
    return NULL;
}

} /* namespace processor */

} /* namespace lang0 */
