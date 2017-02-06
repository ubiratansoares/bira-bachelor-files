

/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 3490    /* the port client will be connecting to */

#define MAXDATASIZE 100 /* max number of bytes we can get at once */

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; /* connector's address information */
	char msg[100];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL) {  /* get the host info */
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;         /* host byte order */
    their_addr.sin_port = htons(PORT);     /* short, network byte order */
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);        /* zero the rest of the struct */

    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

	while(1){
	    
		if(scanf("%s", &msg)){
				if(send(sockfd, msg, sizeof(msg), 0) == -1){
					perror("send");
					exit(1);
				}
			if (strcmp("quit", msg) == 0) break;
		}


		if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
    	    perror("recv");
        	exit(1);
	    }

    	if (numbytes != 0){ 
		 	buf[numbytes] = '\0';

    		printf("Received: %s\n",buf);
			fflush(stdout);
			if (strcmp("quit", buf) == 0) break;
		}
	

	}

    close(sockfd);

    return 0;
}



