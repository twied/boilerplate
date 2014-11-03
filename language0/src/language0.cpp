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

#include "language0.h"

using namespace std;
using namespace lang0;

int main(int argc, char* argv[]) {
    if(argc != 4) {
        cout << "usage: " << argv[0] << " COMMAND INFILE OUTFILE" << endl;
        cout << endl << "COMMAND can be one of:" << endl;
        cout << "\tlex\tproduce raw token stream" << endl;
        cout << "\tdot\tcompile program into graphviz graph" << endl;
        cout << "\tcc0\tcompile program into language0 assembler" << endl;
        cout << "\tg32\tcompile program into gnu x86_32 assembler" << endl;
        cout << "\tas0\tcompile language0 assembly into language0 opcodes" << endl;
        cout << "\trun\trun language0 opcodes" << endl;
        cout << endl << "INFILE and OUTFILE can be \"--\" to read from stdin / write to stdout." << endl;
        return 1;
    }

    istream* stream_in = &cin;
    if(string(argv[2]) != "--") {
        stream_in = new ifstream(argv[2]);
    }
    if(!stream_in) {
        cout << "can not open input file " << argv[2] << endl;
        return false;
    }

    ostream* stream_out = &cout;
    if(string(argv[3]) != "--") {
        stream_out = new ofstream(argv[3]);
    }
    if(!stream_out) {
        cout << "can not open output file " << argv[3] << endl;
        return false;
    }

    try {
        string arg(argv[1]);
        if(arg == "lex") {
            Lexer lexer(stream_in);

            Token token;
            while((token = lexer()) != TOKEN_EOF) {
                *stream_out << token_name(token) << endl;
            }
        } else if(arg == "dot") {
            Parser parser(stream_in);
            backend::DOT dot(stream_out);

            ast::Program* program = parser();
            dot.process(program);

            delete program;
        } else if(arg == "cc0") {
            Parser parser(stream_in);
            backend::CC0 compiler(stream_out);

            ast::Program* program = parser();
            processor::VariableUsage().process(program);
            processor::ConstantFolding().process(program);
            processor::DeadCodeRemoval().process(program);
            compiler.process(program);

            delete program;
        } else if(arg == "g32") {
            Parser parser(stream_in);
            backend::G32 compiler(stream_out);

            ast::Program* program = parser();
            processor::VariableUsage().process(program);
            compiler.process(program);

            delete program;
        } else if(arg == "as0") {
            Assembler0(stream_in, stream_out)();
        } else if(arg == "run") {
            VirtualMachine0(stream_in, stream_out)();
        } else {
            cout << "unknown command: " << argv[1] << endl;
        }
    } catch (const string& s) {
        cout << s << endl;
    }

    if(stream_in != &cin) {
        delete stream_in;
    }

    if(stream_out != &cout) {
        delete stream_out;
    }

    return 0;
}
