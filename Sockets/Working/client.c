#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <netdb.h> 
/*
Jordan Sanders
Charles Macon
Garrett Wescott
Julie Qiroz
*/

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int tickets[10];

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	srand(time(NULL));
	int i,j;
	int tickets[10];
	j=0;
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	for(i=0;i<10;i++){
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
		
	// THIS IS WHERE WE SEND THE BUY AND CANCEL COMMANDS.	
    bzero(buffer,256);
	
		int r = (rand() % 10);
		//if(r==9|| r>5|| j==0) printf("No tickets to cancel");
		if (r ==9 ||r > 7){
			if(j==0) printf("No tickets to cancel\n");
			else{
				printf("Cancelled %d\n",tickets[j]);
				sprintf(buffer,"c %d",tickets[j--]);
			}
		}
		else
		{
			buffer[0] = 'b';
			printf("Bought\n");
		}
		
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) 
         error("ERROR writing to socket");
		 
		// THIS IS WHERE WE GET MESSAGES BACK FROM THE SERVER
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) 
			 error("ERROR reading from socket");
		if(buffer[0]!= 'C'){
			tickets[j]+=10000*(buffer[0]+'0');
			tickets[j]+=1000*(buffer[1]+'0');
			tickets[j]+=100*(buffer[2]+'0');
			tickets[j]+=10*(buffer[3]+'0');
			tickets[j]+=(buffer[4]+'0');
			j++;
		}
			
		printf("%s\n",buffer);
		bzero(buffer,256);	
	}
	
	// CLEANUP
    close(sockfd);
    return 0;
}
