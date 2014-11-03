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

static volatile sig_atomic_t server_running = 1;

static void server_signal(int signo) {
    (void) signo; /* unused */
    server_running = 0;
}

static void server_worker(int socket_fd, unsigned port) {
    while(1) {
        char buffer[BUFSIZ] = {0};
        ssize_t length = recv(socket_fd, buffer, sizeof(buffer), 0);
        if(length < 0) {
            snprintf(buffer, sizeof(buffer), "%u: recv() failed", port);
            perror(buffer);
            return;
        }
        if(length == 0) {
            return;
        }

        printf("%u: %s\n", port, buffer);
    }
}

int server(int argc, char* argv[]) {
    (void) argc; /* unused */
    /* set up signal handler */
    printf("Setting up signal handler...\n");
    if(SIG_ERR == signal(SIGHUP, server_signal)) {
        perror("\tsignal() failed");
    } else {
        printf("\tdone");
    }

    /* create socket */
    printf("Creating socket...\n");
    int server_socket_fd;
    if(-1 == (server_socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("\tsocket() failed");
        return 1;
    } else {
        printf("\tCreated with fd %i\n", server_socket_fd);
    }

    /* bind socket so local address */
    printf("Binding socket...\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(strtoul(argv[2], NULL, 0));
    addr.sin_addr.s_addr = INADDR_ANY;
    if(-1 == bind(server_socket_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr))) {
        perror("\tbind() failed");
        return 1;
    } else {
        printf("\tdone\n");
    }

    /* mark socket as listening socket */
    printf("Mark socket as listening socket...\n");
    if(-1 == listen(server_socket_fd, SOMAXCONN)) {
        perror("\tlisten() failed");
        return 1;
    } else {
        printf("\tQueue length: %i\n", SOMAXCONN);
    }

    /* accept connection and process */
    while(server_running) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_length = sizeof(client_addr);
        int socket_fd = accept(server_socket_fd, (struct sockaddr*) &client_addr, &client_addr_length);
        if(socket_fd < 0) {
            perror("\taccept() failed");
            break;
        }
        if(client_addr_length != sizeof(client_addr)) {
            printf("\tclient connected, addr information was truncated.\n");
        } else {
            printf("\tclient connected: %u.%u.%u.%u:%u\n", 0xff & (client_addr.sin_addr.s_addr >> 0), 0xff & (client_addr.sin_addr.s_addr >> 8), 0xff & (client_addr.sin_addr.s_addr >> 16), 0xff & (client_addr.sin_addr.s_addr >> 24), client_addr.sin_port);
        }

        pid_t pid = fork();
        switch(pid) {
        case -1:
            /* error */
            perror("fork() failed");
            return 1;

        case 0:
            /* child */
            close(server_socket_fd);
            server_worker(socket_fd, client_addr.sin_port);
            shutdown(socket_fd, SHUT_RDWR);
            close(socket_fd);
            printf("\tclient quit: %u\n", client_addr.sin_port);
            exit(0);

        default:
            /* parent */
            close(socket_fd);
        }
    }

    /* shutdown */
    shutdown(server_socket_fd, SHUT_RDWR);
    close(server_socket_fd);

    return 0;
}
