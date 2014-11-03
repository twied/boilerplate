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

#ifndef VIRTUALMACHINE_H_
#define VIRTUALMACHINE_H_

#include <istream>
#include <ostream>
#include <vector>

namespace lang0 {

#define VM_STACK_SIZE (1024 * 1024)

enum opcode_t {
    OPCODE_PRINT,   /* pop i; print i; */

    OPCODE_STORE,   /* pop value; pop position; stack[position] = value; */
    OPCODE_LOAD,    /* pop position; push stack[position]; */

    OPCODE_PUSH,    /* push $parameter; */

    OPCODE_JUMPZ,   /* pop value; if (value == 0) then PC = $parameter; */
    OPCODE_JUMP,    /* PC = $parameter; */
    OPCODE_HALT,    /* exit 0; */

    OPCODE_ADD,     /* pop b; pop a; push (a + b); */
    OPCODE_SUB,     /* pop b; pop a; push (a - b); */
    OPCODE_MUL,     /* pop b; pop a; push (a * b); */
    OPCODE_DIV,     /* pop b; pop a; push (a / b); */

    OPCODE_GRT,     /* pop b; pop a; if (a > b) then PUSH 1 else PUSH 0; */
    OPCODE_GEQ,     /* pop b; pop a; if (a >= b) then PUSH 1 else PUSH 0; */
    OPCODE_LES,     /* pop b; pop a; if (a < b) then PUSH 1 else PUSH 0; */
    OPCODE_LEQ,     /* pop b; pop a; if (a <= b) then PUSH 1 else PUSH 0; */
    OPCODE_EQL,     /* pop b; pop a; if (a = b) then PUSH 1 else PUSH 0; */
    OPCODE_NEQ,     /* pop b; pop a; if (a != b) then PUSH 1 else PUSH 0; */

    OPCODE_AND,     /* pop b; pop a; PUSH (a && b); */
    OPCODE_OR,      /* pop b; pop a; PUSH (a || b); */
    OPCODE_NEG,     /* pop a; PUSH (-a); */
    OPCODE_NOT      /* pop a; PUSH (!a); */
};

int length(const opcode_t& opcode);

class Assembler0 {
public:
    Assembler0(std::istream* stream_in, std::ostream* stream_out);
    int operator()();
private:
    std::istream* stream_in;
    std::ostream* stream_out;
};

class VirtualMachine0 {
public:
    VirtualMachine0(std::istream* stream_in, std::ostream* stream_out);
    int operator()();
private:
    int pop();
    void push(const int& value);
    int get(const int& address);
    void set(const int& address, const int& value);
    int fetch();

    std::istream* stream_in;
    std::ostream* stream_out;
    int reg_pc;
    int reg_sp;
    std::vector<int> mem_stack;
    std::vector<int> mem_program;
};

} /* namespace lang0 */

#endif /* VIRTUALMACHINE_H_ */
