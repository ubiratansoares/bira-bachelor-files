/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define MYPORT 3490    /* the port users will be connecting to */

#define BACKLOG 10     /* how many pending connections queue will hold */

#define MAXDATASIZE 100 /* max number of bytes we can get at once */

main()
{
    int sockfd, new_fd;  /* listen on sock_fd, new connection on new_fd */
    struct sockaddr_in my_addr;    /* my address information */
    struct sockaddr_in their_addr; /* connector's address information */
    int sin_size;
    int saida;
    char msg[100];

    int  numbytes;  
    char buf[MAXDATASIZE];

	
	char *some_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;         /* host byte order */
    my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* automatically fill with my IP */
    bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }
	printf("Bind\n");

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
	printf("Listen\n");

    saida =1;

    while(saida) {  /* main accept() loop */
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }
		
	printf("New socket\n");
	printf("From: %s %d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));

		while(saida){

			 if ((numbytes=recv(new_fd, buf, MAXDATASIZE, 0)) == -1) {
			        perror("recv");
			        exit(1);
   			 
			 } 
				 if (numbytes !=0){
					buf[numbytes]='\0';
					printf("Received: %s\n",buf);
					fflush(stdout);
					
					if (strcmp("quit",buf)== 0) { 
						saida=0 ;
						break; 
					}
				 }

			

				if (scanf("%s", &msg)){
					if (send(new_fd, msg, sizeof(msg), 0) == -1){
						perror("send");
						exit(1);
					} 
				if (strcmp("quit",msg)== 0) { 
						saida=0 ;
					break; }
				}
			 
  		}

        close(new_fd);  

    }
}


