#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
	 
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
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}


void dostuff (int sock)
{
   int n;
   char buffer[256];
      
	bzero(buffer,256);
     n = read(sock,buffer,255);
     if (n < 0) error("ERROR reading from socket");
	 
	 // Buy order or cancel order.
	 else if (buffer[0] == 'b')
	 {
		// buy logic
		printf("Buy order processed\n");
		n = write(sock,"Buy order processed",19);
		if (n < 0) error("ERROR writing to socket");
	 }
	 else if (buffer[0] == 'c')
	 {
		// cancel order logic
		printf("Order cancelled\n");
		n = write(sock,"Order cancelled",15);
		if (n < 0) error("ERROR writing to socket");
	 }
	 else
	 {
		printf("ERROR: Unknown Command\n");
		n = write(sock,"ERROR: Unknown Command",22);
		if (n < 0) error("ERROR writing to socket");
	 }
}