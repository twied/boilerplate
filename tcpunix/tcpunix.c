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

#include "tcpunix.h"

#define CMD_SERVER "server"
#define CMD_CLIENT "client"

int main(int argc, char* argv[]) {
    if(3 == argc && 0 == strncmp(argv[1], CMD_SERVER, sizeof(CMD_SERVER))) {
        return server(argc, argv);
    } else if (4 == argc && 0 == strncmp(argv[1], CMD_CLIENT, sizeof(CMD_CLIENT))) {
        return client(argc, argv);
    }

    fprintf(stderr, "Usage: %s server PORT\n", argv[0]);
    fprintf(stderr, "   or: %s client HOSTNAME PORT\n", argv[0]);
    return 1;
}
