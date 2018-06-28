/*
Author: Seiya Kobayashi
Date: Jan 11, 2018

Description(Basic Flow):
1: socket() -> Create an endpoint and return a socket descriptor.
2: connect() -> Secure connection to the server (Associate the socket with a remote address).
3: send() & recv() -> Send & receive (read & write) data.
4: close() -> Close the socket.
*/

#include <stdlib.h>     // exit()
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>     // socket()
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#define MAXLINE 4096      // Max text line length
#define SERV_PORT 3000      // Port

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    // Basic check of the arguments
    // Additional checks can be inserted
    if (argc != 2) {
        perror("Usage: TCPClient <IP address of the server");
        exit(1);
    }

    // Create a socket for the client
    // If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Problem in creating the socket: ");
        printf("%d\n", errno);
        exit(2);
    }

    // Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));     // Zero out
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(argv[1]);
    servaddr.sin_port =  htons(SERV_PORT);      // Convert to big-endian order

    // Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Problem in connecting to the server: ");
        printf("%d\n", errno);
        exit(3);
    }

    while (fgets(sendline, MAXLINE, stdin) != NULL) {

        send(sockfd, sendline, strlen(sendline), 0);

        if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
            // Error: server terminated prematurely
            perror("The server terminated prematurely");
            exit(4);
        }
        printf("%s", "String received from the server: ");
        fputs(recvline, stdout);
    }

    exit(0);
}
