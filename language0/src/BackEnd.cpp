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

#include "BackEnd.h"

using namespace std;

namespace lang0 {

using namespace ast;
using namespace ast::expression;
using namespace ast::statement;

namespace backend {

CC0::CC0(ostream* stream) : stream(stream), variables() {
}

CC0::~CC0() {
}

void* CC0::process(Program* node) {
    *stream << "# variables" << endl;
    unsigned offset = 0;
    for(vector<string>::iterator it = node->variables.begin(); it != node->variables.end(); ++it) {
        *stream << "\tpush\t0\t# " << *it << endl;
        variables[*it] = offset;
        offset += 1;
    }

    *stream << "# code" << endl;
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }

    *stream << "\thalt" << endl;
    return NULL;
}

void* CC0::process(Assignment* node) {
    *stream << "\tpush\t" << variables[node->identifier] << "\t# address of " << node->identifier << endl;
    node->expression->process(this);
    *stream << "\tstore" << endl << endl;
    return NULL;
}

void* CC0::process(If* node) {
    node->expression->process(this);
    *stream << "\tjumpz\t" << unique_name(&node->then_statements) << endl;
    for(vector<Statement*>::iterator it = node->then_statements.begin(); it != node->then_statements.end(); ++it) {
        (*it)->process(this);
    }

    if(node->else_statements.empty()) {
        *stream << unique_name(&node->then_statements) << ":" << endl << endl;
    } else {
        *stream << "\tjump\t" << unique_name(&node->else_statements) << endl;
        *stream << unique_name(&node->then_statements) << ":" << endl;
        for(vector<Statement*>::iterator it = node->else_statements.begin(); it != node->else_statements.end(); ++it) {
            (*it)->process(this);
        }
        *stream << unique_name(&node->else_statements) << ":" << endl << endl;
    }
    return NULL;
}

void* CC0::process(Print* node) {
    node->expression->process(this);
    *stream << "\tprint" << endl << endl;
    return NULL;
}

void* CC0::process(While* node) {
    *stream << unique_name(node) << ":\t\t#while -> " << unique_name(&node->statements) << endl;
    node->expression->process(this);
    *stream << "\tjumpz\t" << unique_name(&node->statements) << endl;
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }
    *stream << "\tjump\t" << unique_name(node) << endl;
    *stream << unique_name(&node->statements) << ":" << endl << endl;
    return NULL;
}

void* CC0::process(BinOp* node) {
    node->lhs_expression->process(this);
    node->rhs_expression->process(this);
    switch(node->oper) {
    case BinOp::ADD:
        *stream << "\top_add" << endl;
        break;
    case BinOp::SUB:
        *stream << "\top_sub" << endl;
        break;
    case BinOp::MUL:
        *stream << "\top_mul" << endl;
        break;
    case BinOp::DIV:
        *stream << "\top_div" << endl;
        break;
    case BinOp::AND:
        *stream << "\top_and" << endl;
        break;
    case BinOp::OR:
        *stream << "\top_or" << endl;
        break;
    case BinOp::EQU:
        *stream << "\top_eql" << endl;
        break;
    case BinOp::NEQ:
        *stream << "\top_neq" << endl;
        break;
    case BinOp::LES:
        *stream << "\top_les" << endl;
        break;
    case BinOp::LEQ:
        *stream << "\top_leq" << endl;
        break;
    case BinOp::GRT:
        *stream << "\top_grt" << endl;
        break;
    case BinOp::GEQ:
        *stream << "\top_geq" << endl;
        break;
    default:
        throw exception(node->source_line, node->source_column, "unknown binop operator");
    }
    return NULL;
}

void* CC0::process(Identifier* node) {
    *stream << "\tpush\t" << variables[node->identifier] << "\t# address of " << node->identifier << endl;
    *stream << "\tload" << endl;
    return NULL;
}

void* CC0::process(Numeral* node) {
    *stream << "\tpush\t" << node->numeral << "\t# value" << endl;
    return NULL;
}

void* CC0::process(UnOp* node) {
    node->expression->process(this);
    switch(node->oper) {
    case UnOp::NEG:
        *stream << "\top_neg" << endl;
        break;
    case UnOp::NOT:
        *stream << "\top_not" << endl;
        break;
    default:
        throw exception(node->source_line, node->source_column, "unknown unop operator");
    }
    return NULL;
}

G32::G32(ostream* stream) : stream(stream) {
}

G32::~G32() {
}

void* G32::process(Program* node) {
    *stream << "\t.data" << endl;
    *stream << "string: .string \"%d\\n\"" << endl;
    for(vector<string>::iterator it = node->variables.begin(); it != node->variables.end(); ++it) {
        *stream << "var_" << *it << ": .long 0" << endl;
    }
    *stream << endl << "\t.text" << endl;
    *stream << "\t.globl\tmain" << endl;
    *stream << "main:" << endl;
    *stream << "\tpushl\t%ebp" << endl;
    *stream << "\tmovl\t%esp,\t%ebp" << endl << endl;
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }
    *stream << "\tmovl\t$0,\t%eax\t# return 0" << endl;
    *stream << "\tleave" << endl;
    *stream << "\tret" << endl;
    *stream << endl;
    return NULL;
}

void* G32::process(Assignment* node) {
    node->expression->process(this);
    *stream << "\tpop\tvar_" << node->identifier << endl << endl;
    return NULL;
}

void* G32::process(If* node) {
    node->expression->process(this);
    *stream << "\tpop\t%eax" << endl;
    *stream << "\tcmpl\t$0,\t%eax" << endl;
    *stream << "\tje\t" << unique_name(&node->then_statements) << endl;
    for(vector<Statement*>::iterator it = node->then_statements.begin(); it != node->then_statements.end(); ++it) {
        (*it)->process(this);
    }
    if(node->else_statements.empty()) {
        *stream << unique_name(&node->then_statements) << ":" << endl << endl;
    } else {
        *stream << "\tjmp\t" << unique_name(&node->else_statements) << endl;
        *stream << unique_name(&node->then_statements) << ":" << endl;
        for(vector<Statement*>::iterator it = node->else_statements.begin(); it != node->else_statements.end(); ++it) {
            (*it)->process(this);
        }
        *stream << unique_name(&node->else_statements) << ":" << endl << endl;
    }
    return NULL;
}

void* G32::process(Print* node) {
    node->expression->process(this);
    *stream << "\tpush\t$string" << endl;
    *stream << "\tcall\tprintf" << endl;
    *stream << "\taddl\t$8,\t%esp" << endl << endl;
    return NULL;
}

void* G32::process(While* node) {
    *stream << unique_name(node) << ":\t\t#while -> " << unique_name(&node->statements) << endl;
    node->expression->process(this);
    *stream << "\tpop\t%eax" << endl;
    *stream << "\tcmp\t$0,\t%eax" << endl;
    *stream << "\tje\t" << unique_name(&node->statements) << endl << endl;

    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
    }
    *stream << "\tjmp\t" << unique_name(node) << endl;
    *stream << unique_name(&node->statements) << ":\t\t#end while <- " << unique_name(node) << endl << endl;
    return NULL;
}

void* G32::process(BinOp* node) {
    node->lhs_expression->process(this);
    node->rhs_expression->process(this);
    *stream << "\tpop\t%ebx" << endl;
    *stream << "\tpop\t%eax" << endl;
    switch(node->oper) {
    case BinOp::ADD:
        *stream << "\taddl\t%ebx,\t%eax" << endl;
        break;
    case BinOp::SUB:
        *stream << "\tsubl\t%ebx,\t%eax" << endl;
        break;
    case BinOp::MUL:
        *stream << "\timull\t%ebx,\t%eax" << endl;
        break;
    case BinOp::DIV:
        *stream << "\tcltd" << endl;
        *stream << "\tidivl\t%ebx" << endl;
        break;
    case BinOp::AND:
        *stream << "\tcmpl\t$0,\t%eax" << endl;
        *stream << "\tsetne\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        *stream << "\tcmpl\t$0,\t%ebx" << endl;
        *stream << "\tsetne\t%bl" << endl;
        *stream << "\tmovzbl\t%bl,\t%ebx" << endl;
        *stream << "\tandl\t%ebx,\t%eax" << endl;
        break;
    case BinOp::OR:
        *stream << "\tcmpl\t$0,\t%eax" << endl;
        *stream << "\tsetne\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        *stream << "\tcmpl\t$0,\t%ebx" << endl;
        *stream << "\tsetne\t%bl" << endl;
        *stream << "\tmovzbl\t%bl,\t%ebx" << endl;
        *stream << "\torl\t%ebx,\t%eax" << endl;
        break;
    case BinOp::EQU:
        *stream << "\tcmpl\t%ebx,\t%eax" << endl;
        *stream << "\tsete\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        break;
    case BinOp::NEQ:
        *stream << "\tcmpl\t%ebx,\t%eax" << endl;
        *stream << "\tsetne\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        break;
    case BinOp::LES:
        *stream << "\tcmpl\t%ebx,\t%eax" << endl;
        *stream << "\tsetl\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        break;
    case BinOp::LEQ:
        *stream << "\tcmpl\t%ebx,\t%eax" << endl;
        *stream << "\tsetle\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        break;
    case BinOp::GRT:
        *stream << "\tcmpl\t%ebx,\t%eax" << endl;
        *stream << "\tsetg\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        break;
    case BinOp::GEQ:
        *stream << "\tcmpl\t%ebx,\t%eax" << endl;
        *stream << "\tsetge\t%al" << endl;
        *stream << "\tmovzbl\t%al,\t%eax" << endl;
        break;
    default:
        throw exception(node->source_line, node->source_column, "unknown binop operator");
    }
    *stream << "\tpush\t%eax" << endl;
    return NULL;
}

void* G32::process(Identifier* node) {
    *stream << "\tpush\t(var_" << node->identifier << ")" << endl;
    return NULL;
}

void* G32::process(Numeral* node) {
    *stream << "\tpush\t$" << node->numeral << endl;
    return NULL;
}

void* G32::process(UnOp* node) {
    node->expression->process(this);
    *stream << "\tpop\t%eax" << endl;
    switch(node->oper) {
    case UnOp::NEG:
        *stream << "\tneg\t%eax" << endl;
        break;
    case UnOp::NOT:
        *stream << "cmpl\t$0,\t%eax" << endl;
        *stream << "sete\t%al"<<endl;
        *stream << "movzbl\t%al,\t%eax" << endl;
        break;
    default:
        throw exception(node->source_line, node->source_column, "unknown unop operator");
    }
    *stream << "\tpush\t%eax" << endl;
    return NULL;
}

DOT::DOT(ostream* stream) : stream(stream) {
}

DOT::~DOT() {
}

template<typename T> void DOT::emit_node(void* node, const T& label) {
    *stream << "node_" << unique_name(node) << " [label=\"" << label << "\"];" << endl;
}

void DOT::emit_edge(void* from_node, void* to_node, const std::string& label) {
    *stream << "node_" << unique_name(from_node) << " -> node_" << unique_name(to_node) << " [label=\"" << label << "\"];" << endl;
}

void* DOT::process(Program* node) {
    *stream << "digraph G {" << endl;
    emit_node(node, "Program");
    emit_node(&node->variables, "Variables");
    emit_edge(node, &node->variables);

    for(vector<string>::iterator it = node->variables.begin(); it != node->variables.end(); ++it) {
        emit_node(&*it, *it);
        emit_edge(&node->variables, &*it);
    }

    emit_node(&node->statements, "Code");
    emit_edge(node, &node->statements);
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
        emit_edge(&node->statements, *it);
    }
    *stream << "}" << endl;
    return NULL;
}

void* DOT::process(Assignment* node) {
    emit_node(node, "=");
    emit_node(&node->identifier, node->identifier);
    emit_edge(node, &node->identifier);
    node->expression->process(this);
    emit_edge(node, node->expression);
    return NULL;
}

void* DOT::process(If* node) {
    emit_node(node, "if");
    node->expression->process(this);
    emit_edge(node, node->expression, "condition");

    emit_node(&node->then_statements, "then");
    emit_edge(node, &node->then_statements);
    for(vector<Statement*>::iterator it = node->then_statements.begin(); it != node->then_statements.end(); ++it) {
        (*it)->process(this);
        emit_edge(&node->then_statements, *it);
    }

    if(!node->else_statements.empty()) {
        emit_node(&node->else_statements, "else");
        emit_edge(node, &node->else_statements);
        for(vector<Statement*>::iterator it = node->else_statements.begin(); it != node->else_statements.end(); ++it) {
            (*it)->process(this);
            emit_edge(&node->else_statements, *it);
        }
    }
    return NULL;
}

void* DOT::process(Print* node) {
    emit_node(node, "print");
    node->expression->process(this);
    emit_edge(node, node->expression);
    return NULL;
}

void* DOT::process(While* node) {
    emit_node(node, "while");
    node->expression->process(this);
    emit_edge(node, node->expression, "condition");
    emit_node(&node->statements, "statements");
    emit_edge(node, &node->statements);
    for(vector<Statement*>::iterator it = node->statements.begin(); it != node->statements.end(); ++it) {
        (*it)->process(this);
        emit_edge(&node->statements, *it);
    }
    return NULL;
}

void* DOT::process(BinOp* node) {
    switch(node->oper) {
    case BinOp::ADD:
        emit_node(node, "+");
        break;
    case BinOp::SUB:
        emit_node(node, "-");
        break;
    case BinOp::MUL:
        emit_node(node, "*");
        break;
    case BinOp::DIV:
        emit_node(node, "/");
        break;
    case BinOp::AND:
        emit_node(node, "&&");
        break;
    case BinOp::OR:
        emit_node(node, "||");
        break;
    case BinOp::EQU:
        emit_node(node, "==");
        break;
    case BinOp::NEQ:
        emit_node(node, "!=");
        break;
    case BinOp::LES:
        emit_node(node, "<");
        break;
    case BinOp::LEQ:
        emit_node(node, "<=");
        break;
    case BinOp::GRT:
        emit_node(node, ">");
        break;
    case BinOp::GEQ:
        emit_node(node, ">=");
        break;
    default:
        emit_node(node, "??");
        break;
    }

    node->lhs_expression->process(this);
    emit_edge(node, node->lhs_expression, "lhs");
    node->rhs_expression->process(this);
    emit_edge(node, node->rhs_expression, "rhs");
    return NULL;
}

void* DOT::process(Identifier* node) {
    emit_node(node, node->identifier);
    return NULL;
}

void* DOT::process(Numeral* node) {
    emit_node(node, node->numeral);
    return NULL;
}

void* DOT::process(UnOp* node) {
    switch(node->oper) {
    case UnOp::NEG:
        emit_node(node, "-");
        break;
    case UnOp::NOT:
        emit_node(node, "!");
        break;
    default:
        emit_node(node, "??");
        break;
    }
    node->expression->process(this);
    emit_edge(node, node->expression);
    return NULL;
}

} /* namespace backend */

} /* namespace lang0 */
