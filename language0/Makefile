all: language0

language0: src/language0.o src/BackEnd.o src/FrontEnd.o src/Processor.o src/SyntaxTree.o src/VirtualMachine.o
	g++ -Wall -Wextra -pedantic -o language0 $^

src/%.o: src/%.cpp src/%.h
	g++ -Wall -Wextra -pedantic -c -o $@ $<

.PHONY: clean
clean:
	rm -f src/language0.o src/BackEnd.o src/FrontEnd.o src/Processor.o src/SyntaxTree.o src/VirtualMachine.o
	rm -f language0
