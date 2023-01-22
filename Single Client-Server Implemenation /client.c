// Client side C program
// 21CS60D04 Major Kunal Pilley

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 3421

int main()
{
	int client_socket_id;
	client_socket_id = socket (AF_INET, SOCK_STREAM, 0); // int return type = socket(domain, type, protocol)

	// to check if socket is succesfully open
	if (client_socket_id < 0)
	{
	printf("\nSocket could not be opened \n\n");
	}else{
	printf("\nSocket open success with id : %d\n\n", client_socket_id);
	}

	struct sockaddr_in server;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	memset (&(server.sin_zero) , 0 ,8);

	// establishing connection between client and server

	int server_id = connect(client_socket_id, (struct sockaddr*)&server, sizeof(struct sockaddr)  );

	if (server_id < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }else
		printf("\nConnection Successful....\n\n");

	char message_sent[256];
	char message_recd[256];
	char choice[6];
	
	printf("==================================================================\n\n");
	printf("-------------FOLLOWING IS THE COMMAND INPUT FORMAT----------------\n\n");
	printf("==================================================================\n\n");
	printf("NLINEX - return no of lines in remote file\n\n");
	printf("READX <K> - to read a Kth index from remote file\n");
	printf("default K is first line\n\n");
	printf("INSERTX <K> <MESSAGE> - to INSERT at a Kth index on remote file\n");
	printf("default K index is last line\n\n");
	printf("Permissible range of K is [-N, N) where N is total no of lines in file\n\n");
	printf("==================================================================\n\n");
	printf("==================================================================\n\n");

	while(1)
	{

		printf("\n\nEnter Command : \n\n");
		scanf("%[^\n]", message_sent);
		fflush ( stdin ) ;

		write(client_socket_id, message_sent, sizeof(message_sent)); // writing to client 

		printf("Command sent to server \n\n");

		read(client_socket_id, message_recd, sizeof(message_recd)); // reading from client

		printf("Message recd from  server:\n");

		printf("-> %s\n\n", message_recd);

		strcpy(message_recd,"");
		strcpy(message_sent,"");

		printf("\nTo close connection to server enter \"exit\" as command or enter anything to continue :");
		scanf("%s",choice);
		fflush ( stdin ) ; // flus the enter key stroke

		if (!strcmp(choice, "exit"))
		{
				printf("\nClosing connection to server......\n");
				break;
		}


	}

	close(client_socket_id);
	
	return 0;
}
