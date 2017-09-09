/*
** udpclient.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define MAXBUFLEN 100

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;

    if(argc < 2){
        printf("usage: %s port", argv[0]);
        return 0;
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("udpclient: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("udpclient: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "udpclient: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("udpclient: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
        &their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("udpclient: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("udpclient: packet contains \"%s\"\n", buf);

    close(sockfd);

    return 0;
}
