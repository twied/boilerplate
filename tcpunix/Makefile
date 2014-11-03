all: tcpunix

tcpunix: tcpunix.o server.o client.o
	gcc -std=c99 -Wall -Wextra -pedantic -o tcpunix $^

%.o: %.c tcpunix.h
	gcc -std=c99 -Wall -Wextra -pedantic -c -o $@ $<

.PHONY: clean
clean:
	rm -f tcpunix.o server.o client.o
	rm -f tcpunix
