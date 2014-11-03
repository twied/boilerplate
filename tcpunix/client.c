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

static ssize_t send_whole(int fd, const char* buffer, size_t size, int flags) {
    size_t offset = 0;
    while(offset < size) {
        ssize_t bytes_send = send(fd, buffer + offset, size - offset, flags);
        if(bytes_send < 0) {
            break;
        }
        offset += bytes_send;
    }
    return offset;
}

int client(int argc, char* argv[]) {
    (void) argc; /* unused */
    /* create socket */
    printf("Creating socket...\n");
    int socket_fd;
    if(-1 == (socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("\tsocket() failed");
        return 1;
    } else {
        printf("\tCreated with fd %i\n", socket_fd);
    }

    /* resolve host name */
    printf("Resolving host name...\n");
    struct hostent *host;
    if(NULL == (host = gethostbyname(argv[2]))) {
        perror("\tgethostbyname() failed");
        return 1;
    } else {
        printf("\tOfficial name: %s\n", host->h_name);
        for(char** alias = host->h_aliases; *alias != NULL; ++alias) {
            printf("\tAlias: %s\n", *alias);
        }
        printf("\tType: %i\n", host->h_addrtype);
        printf("\tLength: %i\n", host->h_length);
        for(char** ips = host->h_addr_list; *ips != NULL; ++ips) {
            printf("\tIP: %u.%u.%u.%u\n", (*ips)[0] & 0xff, (*ips)[1] & 0xff, (*ips)[2] & 0xff, (*ips)[3] & 0xff);
        }
    }

    /* connect socket */
    printf("Connecting...\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(strtoul(argv[3], NULL, 0));
    memcpy(&addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
    if(-1 == connect(socket_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr))) {
        perror("\tconnect() failed");
        return 1;
    } else {
        printf("\tdone\n");
    }

    while(1) {
        char buffer[BUFSIZ] = {0};
        /* read input */
        ssize_t length = read(0, buffer, sizeof(buffer));
        if(length <= 0) {
            /* eof or error */
            break;
        }

        /* remove line break */
        while(buffer[length - 1] == '\r' || buffer[length - 1] == '\n') {
            length -= 1;
        }

        /* send input */
        if(send_whole(socket_fd, buffer, length, 0) != length) {
            perror("\tsend() failed");
            break;
        }
    }

    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);
    return 0;
}
