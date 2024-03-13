#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

main()
{
	int sockid;
	int connectid;
	struct sockaddr_in servaddr;
	struct sockaddr_in client;
	int newsockid;
	int clientlen;
        int n;

	char msg[1000];
	int sendid, recvid;
	int port_id = 6000;
        


	sockid = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&servaddr, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(port_id);
	connectid = connect(sockid, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in));

	if(connectid < 0)
		printf("error \n");


        while(1){

        printf("\n write to the server:");
        n=0;
        while((msg[n++]=getchar())!='\n');
           sendto(sockid,msg,sizeof(msg),0,(struct sockaddr*)&servaddr, sizeof(struct sockaddr_in));

        bzero(msg,sizeof(msg));
    
        // recvid = recvfrom(sockid, msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, &clientlen);
        // printf("%s \n", msg);

        if(strncmp("bye",msg,3)==0)
         {
           printf("Exit session...\n");
           break;
         }
        } // end of while loop
	return 0;
}
