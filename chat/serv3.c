#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_CLIENTS 5

struct ThreadData
{
    int newsockid;
};

void *listenToClient(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    int newsockid = data->newsockid;
    char msg[1000];
    int recvid;

    while (1)
    {
        recvid = recv(newsockid, msg, sizeof(msg), 0);
        if (recvid <= 0)
        {
            printf("Client disconnected\n");
            break;
        }

        printf("%s \n", msg);
        // Process the received message if needed
    }

    close(newsockid);
    free(data);
    pthread_exit(NULL);
}

void *sendToClient(void *arg)
{
    struct ThreadData *data = (struct ThreadData *)arg;
    int newsockid = data->newsockid;
    char msg[1000];

    while (1)
    {
        printf("\n write to the client:");
        fgets(msg, sizeof(msg), stdin);
        send(newsockid, msg, sizeof(msg), 0);

        if (strncmp("bye", msg, 3) == 0)
        {
            printf("Exit session...\n");
            break;
        }
    }

    close(newsockid);
    free(data);
    pthread_exit(NULL);
}

int main()
{
    int sockid, bindid;
    struct sockaddr_in myaddr, client;
    int clientlen;
    int port_id = 6000;

    sockid = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char *)&myaddr, sizeof(struct sockaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port_id);
    bindid = bind(sockid, (struct sockaddr *)&myaddr, sizeof(struct sockaddr_in));

    if (bindid < 0)
    {
        printf("error \n");
        return 1;
    }

    listen(sockid, MAX_CLIENTS);

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        clientlen = sizeof(struct sockaddr_in);
        int newsockid = accept(sockid, (struct sockaddr *)&client, &clientlen);
        if (newsockid < 0)
        {
            printf("error 2\n");
            continue;
        }

        struct ThreadData *data = malloc(sizeof(struct ThreadData));
        data->newsockid = newsockid;

        pthread_t listenThread, sendThread;
        pthread_create(&listenThread, NULL, listenToClient, (void *)data);
        pthread_create(&sendThread, NULL, sendToClient, (void *)data);

        pthread_join(listenThread, NULL);
        pthread_join(sendThread, NULL);
    }

    close(sockid);

    return 0;
}
