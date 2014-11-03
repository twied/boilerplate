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

#include "VirtualMachine.h"

#include <map>
#include <sstream>
#include <string>

using namespace std;

namespace lang0 {

#define LABEL (-1)

template<typename T> static int to_int(const T& value) {
    stringstream stream;
    stream << value;
    int result;
    stream >> result;
    return result;
}

int length(const opcode_t& opcode) {
    switch(opcode) {
    case OPCODE_PUSH: /* fallthrough */
    case OPCODE_JUMPZ: /* fallthrough */
    case OPCODE_JUMP: /* fallthrough */
        return 2;
    default:
        return 1;
    }
}

struct Instruction {
    int opcode;
    string parameter;

    Instruction(const int& opcode, const string& parameter) : opcode(opcode), parameter(parameter) {
    }
};

Assembler0::Assembler0(istream* stream_in, ostream* stream_out) : stream_in(stream_in), stream_out(stream_out) {
}

int Assembler0::operator()() {
    vector<Instruction> instructions;

    /* read instructions */
    while(stream_in->good()) {
        string line;
        getline(*stream_in, line);

        /* remove comments */
        size_t mark = line.find("#");
        if(mark != string::npos) {
            line = line.substr(0, mark);
        }

        /* extract label */
        stringstream line_stream(line);
        string label, cmd, param;
        line_stream >> label >> cmd >> param;

        if(*label.rbegin() == ':') {
            label.erase(label.end() - 1);
            instructions.push_back(Instruction(LABEL, label));
        } else {
            param = cmd;
            cmd = label;
        }

        if(cmd.empty()) {
            continue;
        } else if(cmd == "print") {
            instructions.push_back(Instruction(OPCODE_PRINT, param));
        } else if(cmd == "store") {
            instructions.push_back(Instruction(OPCODE_STORE, param));
        } else if(cmd == "load") {
            instructions.push_back(Instruction(OPCODE_LOAD, param));
        } else if(cmd == "push") {
            instructions.push_back(Instruction(OPCODE_PUSH, param));
        } else if(cmd == "jumpz") {
            instructions.push_back(Instruction(OPCODE_JUMPZ, param));
        } else if(cmd == "jump") {
            instructions.push_back(Instruction(OPCODE_JUMP, param));
        } else if(cmd == "halt") {
            instructions.push_back(Instruction(OPCODE_HALT, param));
        } else if(cmd == "op_add") {
            instructions.push_back(Instruction(OPCODE_ADD, param));
        } else if(cmd == "op_sub") {
            instructions.push_back(Instruction(OPCODE_SUB, param));
        } else if(cmd == "op_mul") {
            instructions.push_back(Instruction(OPCODE_MUL, param));
        } else if(cmd == "op_div") {
            instructions.push_back(Instruction(OPCODE_DIV, param));
        } else if(cmd == "op_grt") {
            instructions.push_back(Instruction(OPCODE_GRT, param));
        } else if(cmd == "op_geq") {
            instructions.push_back(Instruction(OPCODE_GEQ, param));
        } else if(cmd == "op_les") {
            instructions.push_back(Instruction(OPCODE_LES, param));
        } else if(cmd == "op_leq") {
            instructions.push_back(Instruction(OPCODE_LEQ, param));
        } else if(cmd == "op_eql") {
            instructions.push_back(Instruction(OPCODE_EQL, param));
        } else if(cmd == "op_neq") {
            instructions.push_back(Instruction(OPCODE_NEQ, param));
        } else if(cmd == "op_and") {
            instructions.push_back(Instruction(OPCODE_AND, param));
        } else if(cmd == "op_or") {
            instructions.push_back(Instruction(OPCODE_OR, param));
        } else if(cmd == "op_neg") {
            instructions.push_back(Instruction(OPCODE_NEG, param));
        } else if(cmd == "op_not") {
            instructions.push_back(Instruction(OPCODE_NOT, param));
        } else {
            throw string("unknown opcode \"" + cmd + "\", param \"" + param + "\"");
        }
    }

    /* create symbol table */
    map<string, int> symbol_table;
    int instruction_counter = 0;
    for(vector<Instruction>::iterator it = instructions.begin(); it != instructions.end(); ++it) {
        if((*it).opcode == LABEL) {
            if(symbol_table.find((*it).parameter) == symbol_table.end()) {
                symbol_table[(*it).parameter] = instruction_counter;
            } else {
                throw string("symbol \"") + (*it).parameter + "\" defined more than once";
            }
        } else {
            instruction_counter += length((opcode_t)(*it).opcode);
        }
    }

    /* write output */
    for(vector<Instruction>::iterator it = instructions.begin(); it != instructions.end(); ++it) {
        if((*it).opcode == LABEL) {
            continue;
        }

        stream_out->write((char*) &(*it).opcode, sizeof((*it).opcode));
        if((*it).opcode == OPCODE_PUSH) {
            int value = to_int((*it).parameter);
            stream_out->write((char*) &value, sizeof(value));
        } else if ((*it).opcode == OPCODE_JUMPZ || (*it).opcode == OPCODE_JUMP) {
            if(symbol_table.find((*it).parameter) != symbol_table.end()) {
                int value = to_int(symbol_table[(*it).parameter]);
                stream_out->write((char*) &value, sizeof(value));
            } else {
                throw string("symbol \"") + (*it).parameter + "\" not found";
            }
        }
    }
    stream_out->flush();
    return 0;
}

VirtualMachine0::VirtualMachine0(istream* stream_in, ostream* stream_out) : stream_in(stream_in), stream_out(stream_out), reg_pc(0), reg_sp(-1), mem_stack(), mem_program() {
    mem_stack.reserve(VM_STACK_SIZE);
    while(stream_in->good()) {
        int value;
        stream_in->read((char*)&value, sizeof(value));
        mem_program.push_back(value);
    }
}

int VirtualMachine0::operator()() {
    while(mem_program[reg_pc] != OPCODE_HALT) {
        int value_lhs, value_rhs, address;
        switch(fetch()) {
        case OPCODE_PRINT:
            *stream_out << pop() << endl;
            break;
        case OPCODE_STORE:
            value_lhs = pop();
            address = pop();
            set(address, value_lhs);
            break;
        case OPCODE_LOAD:
            address = pop();
            value_rhs = get(address);
            push(value_rhs);
            break;
        case OPCODE_PUSH:
            value_rhs = fetch();
            push(value_rhs);
            break;
        case OPCODE_JUMPZ:
            address = fetch();
            value_rhs = pop();
            if(value_rhs == 0) {
                reg_pc = address;
            }
            break;
        case OPCODE_JUMP:
            address = fetch();
            reg_pc = address;
            break;
        case OPCODE_HALT:
            reg_pc -= 1;
            break;
        case OPCODE_ADD:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs + value_rhs);
            break;
        case OPCODE_SUB:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs - value_rhs);
            break;
        case OPCODE_MUL:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs * value_rhs);
            break;
        case OPCODE_DIV:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs / value_rhs);
            break;
        case OPCODE_GRT:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs > value_rhs ? 1 : 0);
            break;
        case OPCODE_GEQ:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs >= value_rhs ? 1 : 0);
            break;
        case OPCODE_LES:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs < value_rhs ? 1 : 0);
            break;
        case OPCODE_LEQ:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs <= value_rhs ? 1 : 0);
            break;
        case OPCODE_EQL:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs == value_rhs ? 1 : 0);
            break;
        case OPCODE_NEQ:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs != value_rhs ? 1 : 0);
            break;
        case OPCODE_AND:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs && value_rhs ? 1 : 0);
            break;
        case OPCODE_OR:
            value_rhs = pop();
            value_lhs = pop();
            push(value_lhs || value_rhs ? 1 : 0);
            break;
        case OPCODE_NEG:
            value_rhs = pop();
            push(-value_rhs);
            break;
        case OPCODE_NOT:
            value_rhs = pop();
            push(value_rhs ? 0 : 1);
            break;
        default:
            throw string("unknown opcode");
        }
    }
    return 0;
}

int VirtualMachine0::pop() {
    if(reg_sp < 0) {
        throw string("stack underflow");
    }
    int value = mem_stack[reg_sp];
    reg_sp -= 1;
    mem_stack.pop_back();
    return value;
}

void VirtualMachine0::push(const int& value) {
    if(reg_sp >= VM_STACK_SIZE) {
        throw string("stack overflow");
    }
    reg_sp += 1;
    mem_stack.push_back(value);
}

int VirtualMachine0::get(const int& address) {
    if(address < 0) {
        throw string("memory underflow");
    } else if (address >= VM_STACK_SIZE) {
        throw string("memory overflow");
    } else {
        return mem_stack[address];
    }
}

void VirtualMachine0::set(const int& address, const int& value) {
    if(address < 0) {
        throw string("memory underflow");
    } else if (address >= VM_STACK_SIZE) {
        throw string("memory overflow");
    } else {
        mem_stack[address] = value;
    }
}

int VirtualMachine0::fetch() {
    if(reg_pc < 0) {
        throw string("program underflow");
    } else if ((unsigned)reg_pc >= mem_program.size()) {
        throw string("program overflow");
    } else {
        return mem_program[reg_pc++];
    }
}

} /* namespace lang0 */
