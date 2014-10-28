/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	 
	 // Ticket array
	 int tickArray[10];
	 int tickArrayPopulatorIndex;
	 srand(time(NULL));
	 
	 for (tickArrayPopulatorIndex = 0; tickArrayPopulatorIndex < 10; tickArrayPopulatorIndex++)
	 {
		// Hacking this together here to make sure the number you get is 5 digits long.
		int r = (rand () % 89999) + 10000;
		
		tickArray[tickArrayPopulatorIndex] = r;
	 }
	 
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
	 
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
	 if (n < 0) error("ERROR writing to socket");
	 
	 // THIS IS THE GENERAL AREA WHERE THINGS SHOULD HAPPEN.
		// RIGHT AROUND HERE SOMEWHERE
	 
	 // CLEANUP
     close(newsockfd);
     close(sockfd);
     return 0; 
}
