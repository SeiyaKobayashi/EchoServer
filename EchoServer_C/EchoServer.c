/*
Author: Seiya Kobayashi
Date: Jan 11, 2018

Description(Basic Flow):
1: socket() -> Create an endpoint and return a scoket descriptor.
2: bind() -> Associate the socket with its local address.
3: listen() -> Set a flag in the internal socket structure
               so that the socket can start recieving connections from clients.
4: accept() -> Ask the socket to accept the next incoming connection
               (Create a socket in a sense).
5: recv() & send() -> Recieve & send (read & write) data.
6: close() -> Close a socket.

*/

#include <stdlib.h>     // exit()
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>     // socket()
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 4096      // Max text line length
#define SERV_PORT 3000      // Port
#define LISTENQ 8     // Maximum number of client connections

int main (int argc, char **argv) {
    int listenfd, connfd, n;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;

    // Creation of the socket
    listenfd = socket (AF_INET, SOCK_STREAM, 0);

    // Preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen (listenfd, LISTENQ);

    printf("%s\n", "Server running...waiting for connections.");

    for ( ; ; ) {
        clilen = sizeof(cliaddr);
        connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s\n","Received request...");

        while ( (n = recv(connfd, buf, MAXLINE, 0)) > 0)  {
            printf("%s\n", "String received from and sent to the client: ");
            puts(buf);
            send(connfd, buf, n, 0);
        }

        if (n < 0) {
            perror("Read error: ");
            printf("%d\n", errno);
            exit(1);
        }
        close(connfd);      // Close TCP session
    }

    close(listenfd);      // Close listening socket
}
