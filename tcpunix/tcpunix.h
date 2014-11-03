/*
 * tcpunix: TCP client / server demonstration for unix.
 * Written in 2014 by Tim Wiederhake
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef TCPUNIX_H_
#define TCPUNIX_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int server(int argc, char* argv[]);
int client(int argc, char* argv[]);

#endif /* TCPUNIX_H_ */
