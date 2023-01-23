// Client side C program
// 21CS60D04 Major Kunal Pilley

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

//#define PORT 5099
#define Portnumber 5099
#define SIZE 512
#define MAX 100000


char buffer[MAX];


FILE *filePointerO1 ;
FILE *filePointerO2 ;
FILE *filePointerO3 ;
FILE *filePointerT ;
int parse_count;
void recieve_file(int sockfd,FILE *fp);
void send_file(FILE *fp, int socket_fd);

void send_file(FILE *fp, int socket_fd)
{   
    int n;
    char data[MAX] = {0};
	fscanf(fp,"%[^EOF]",data);
	//printf("%s",data);
	write(socket_fd, data, sizeof(data)); 
	printf("\nsent file length %lu -\n", strlen(data));
	bzero(data, MAX);
	fclose(fp);
	
    /*while(fgets(data, sizeof(data), fp) != NULL) 
    {	printf("--%s\n",data);
        if (send(socket_fd, data, sizeof(data), 0) == -1) 
        {
        perror("[-]Error in sending file.");
        exit(1);
        }
		//write(socket_fd, data, sizeof(data)); 
		//write(socket_fd, data, strlen(data));
        bzero(data, SIZE);
    }*/

}

// to recieve a file from the client 
void recieve_file(int sockfd,FILE *fp)
{
  int n,k ;
  char buffer[MAX];
  bzero(buffer, MAX);
  read(sockfd, buffer, sizeof(buffer));
  fwrite(buffer,sizeof(char),strlen(buffer),fp);
  printf(" file recv len %lu\n ",strlen(buffer));
 // printf("%s",buffer);
  bzero(buffer, MAX);

  printf("\n file recd successfully \n");
  fclose(fp);
}

int main()
{
    int client_socket_id = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *server; 
    server = gethostbyname("localhost"); //this command resolves host address to corresponding IP address
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr)); // initializes buffer
    serv_addr.sin_family = AF_INET; // for IPv4 family
    bcopy((char *)server->h_addr, (char *) /* copy server IP address*/&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(Portnumber); //defining port number
	
    if (connect(client_socket_id,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 ) 
    { 
        printf("ERROR connecting"); 
    } // initiating connect request to the server
    // char buffer[MAX];
    // int n,m; // client buffer to forward request to the server
	// // int client_socket_id;
	// client_socket_id = socket (AF_INET, SOCK_STREAM, 0); // int return type = socket(domain, type, protocol)

	// // to check if socket is succesfully open
	// if (client_socket_id < 0)
	// {
	// 	printf("\nSocket could not be opened \n\n");
	// 	exit(1);

	// }else{
	// 	printf("\nSocket open success with id : %d\n\n", client_socket_id);
	// }

	// struct sockaddr_in server;

	// server.sin_addr.s_addr = INADDR_ANY;
	// server.sin_family = AF_INET;
	// server.sin_port = htons(PORT);
	// memset (&(server.sin_zero) , 0 ,8);

	// 	if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)<=0)
	// {
	// 	printf("\nInvalid address/ Address not supported \n");
	// 	return -1;
	// }

	// establishing connection between client and server

	// if (connect(client_socket_id, (struct sockaddr*)&server, sizeof(struct sockaddr) < 0 ))
    // {
    //     printf("\nConnection Failed \n");
    //     return -1;
    // }else{

	// 	printf("\nConnection Successful....\n\n");
	// }

	char message_sent[256];
	char message_recd[256];
	char choice[6];
	char input [150];
	char command[11],file1[20],file2[20],file3[20];
	char field[2], catch [5];

	read(client_socket_id, message_recd, sizeof(message_recd));
	if ( !strcmp(message_recd, "Max clients connected, try again later\n") ){
		printf("\n\nMessage from server : %s\n", message_recd);
		return 0;
	}
	printf("\n\nMessage from server : %s\n", message_recd);

 	while(1)
 	{	strcpy(input,"");strcpy(command,"");strcpy(file1,"");
		strcpy(file2,"");strcpy(file3,"");strcpy(field,""); strcpy(catch,"");
		bzero(message_recd,256);bzero(message_sent,256);

		printf("\n\n Enter Command : \n\n");
		scanf("%[^\n]", input);
		fflush ( stdin ) ;fflush(stdout);

	 	parse_count = sscanf(input,"/%s",command);
        // parsing recd message in respective variable 
        //parse count to decide the no of inputs sent and the validity of the same

        if(!strcmp(command,"sort") ) // valid command with only no other argument
        {  
			parse_count = sscanf(input,"/%s %s %s %[^\n]",command,file1,field,catch);

			/*printf("Parse count is : %d\n", parse_count);
			printf("command : %s \n",command);
			printf("command LEN: %lu \n",strlen(command));
			printf("filename : %s \n", file1);
			printf("command LEN: %lu \n",strlen(file1));
			printf("fd : %s \n", field);
			printf("fdlen: %lu \n",strlen(field));
			printf("catch : %s \n", catch);
			printf("fdlen: %lu \n",strlen(catch));*/

			if(parse_count <3 || parse_count>3 || strlen(field)==0 || strlen(field)>1 || (strcmp(field,"D")&&strcmp(field,"N")&&strcmp(field,"P")))
			{
				printf("\nINVALID command in SORT : PLEASE TRY AGAIN \n\n");
				continue;
			}
			else
			{
				filePointerO1 = fopen(file1 , "r") ;

				if ( filePointerO1 == NULL ) // check for sucessfull file opening 
				{
					printf( "%s file failed to open. Try Again",file1) ;
					continue;
					sprintf(message_sent, "server_file.txt file failed to open. ");

				}
				else
				{
					printf( "\n%s  file opened.\n",file1 ) ;
					strcpy(message_sent,input);
					write(client_socket_id, message_sent, sizeof(message_sent)); // --> 1
					printf("\nCommand sent to server \n");

					read(client_socket_id, message_recd, sizeof(message_recd)); // <-- 4
					printf("\nMessage from server:%s\n",message_recd);
					strcpy(message_recd,"");strcpy(message_sent,"");
					
					send_file(filePointerO1, client_socket_id);
					fclose(filePointerO1);
					printf("File data sent successfully to server.\n");

					read(client_socket_id, message_recd, sizeof(message_recd)); // <-- 4
					if (!strcmp(message_recd, "Error")){
					printf("File is invalid ");
					continue;
					}else if(!strcmp(message_recd, "Valid"))
					{
					printf("File 1 is valid ");
					}

					filePointerO1 = fopen(file1, "w") ;
					if ( filePointerO1 == NULL ) // check for sucessfull file opening 
					printf( "\nfile failed to open for over write.\n") ;
					else
					printf( "\nfile opened for over write.\n") ;
					bzero(buffer,SIZE);
					
					recieve_file(client_socket_id,filePointerO1);
					/*read(client_socket_id, buffer, sizeof(buffer));
					printf("%s",buffer);
					fwrite(buffer,sizeof(char),strlen(buffer),filePointerO1);
					bzero(buffer, SIZE);
					fclose(filePointerO1);*/
				}

			}
        
            
        }
        else if(!strcmp(command,"merge") ) // VALID COMMAND with only upto one argumemt as index
        {
            parse_count = sscanf(input,"/%s %s %s %s %s %[^\n]",command,file1,file2,file3,field,catch);
			//printf("Parse count is : %d\n", parse_count);
			printf("command : %s \n",command);
			printf("filename1 : %s \n", file1);
			//printf("command LEN: %lu \n",strlen(file1));
			printf("filename2 : %s \n", file2);
			//printf("command LEN: %lu \n",strlen(file2));
			printf("filename3 : %s \n", file3);
			//printf("command LEN: %lu \n",strlen(file3));
			printf("field : %s \n", field); 
			//printf("fdlen: %lu \n",strlen(field));
			if(parse_count <5 || parse_count >5 || strlen(field)==0 || strlen(field)>1 || (strcmp(field,"D")&&strcmp(field,"N")&&strcmp(field,"P")))
			{
				printf("\nINVALID command in Merge : PLEASE TRY AGAIN \n\n");
				continue;
			}
			else
			{	
				filePointerO1 = fopen(file1 , "r") ;
				filePointerO2 = fopen(file2 , "r") ;

				if ( filePointerO1 == NULL ) // check for sucessfull file opening 
				{
					printf( "%s - file failed to open.Try Again\n",file1) ;
					continue;
				}
				printf( "\n%s - file opened sucessfully.\n",file1 ) ;
				if ( filePointerO2 == NULL ) // check for sucessfull file opening 
				{
					printf( "%s file failed to open.Try Again\n",file2) ;
					continue;
				}
				printf( "\n%s - file opened sucessfully.\n",file2 ) ;

				strcpy(message_sent,input);
					write(client_socket_id, message_sent, sizeof(message_sent)); // writing to client 
					printf("Command sent to server \n\n");

					read(client_socket_id, message_recd, sizeof(message_recd)); // reading from client
					printf("Message from server:%s\n",message_recd);
					strcpy(message_recd,"");
					strcpy(message_sent,"");

					send_file(filePointerO1, client_socket_id);
					fclose(filePointerO1);
					printf("File - 1 data sent successfully.\n");

					read(client_socket_id, message_recd, sizeof(message_recd)); // <-- 4
					if (!strcmp(message_recd, "Error"))
					{
						printf("File 1 is invalid - %s \n",file1);
						continue;
					}else if(!strcmp(message_recd, "Sort"))
					{
						printf("File 1 is not sorted - %s \n",file1);
						continue;

					}
					else if(!strcmp(message_recd, "Valid"))
					{
					printf("File 1 is valid - %s \n",message_recd);
					}
					bzero(message_recd,256);
					

					send_file(filePointerO2, client_socket_id);
					fclose(filePointerO2);
					printf("File - 2 data sent successfully.\n");

					read(client_socket_id, message_recd, sizeof(message_recd));
					if (!strcmp(message_recd, "Error"))
					{
						printf("File 2 is invalid - %s \n",file2);
						continue;
					}else if(!strcmp(message_recd, "Sort"))
					{
						printf("File 2 is not sorted - %s \n",file2);
						continue;

					}
					else if(!strcmp(message_recd, "Valid"))
					{
					printf("File 2 is valid - %s \n",message_recd);
					}
					bzero(message_recd,256);
					

					strcpy(message_sent,"test");
					write(client_socket_id, message_sent, sizeof(message_sent)); // writing to client 

					filePointerT = fopen(file3, "w") ;
					if ( filePointerT == NULL ) // check for sucessfull file opening 
					printf( "\nfile failed to open.\n") ;
					recieve_file(client_socket_id,filePointerT);
					fclose(filePointerT);




			}

        }
        else if(!strcmp(command,"similarity")) // valid command for similarity
        {	
			parse_count = sscanf(input,"/%s %s %s %[^\n]",command,file1,file2,catch);
			printf("Parse count is : %d\n", parse_count);
			printf("command : %s \n",command);
			printf("filename 1 : %s \n", file1);
			printf("filename 2 : %s \n", file2);
			if(parse_count <3 || parse_count>3 )
			{
				printf("\nINVALID command in similarity : PLEASE TRY AGAIN \n\n");
				continue;

			}
			else
			{	filePointerO1 = fopen(file1 , "r") ;
				filePointerO2 = fopen(file2 , "r") ;

				if ( filePointerO1 == NULL ) // check for sucessfull file opening 
				{
					printf( "%s - file failed to open.",file1) ;
					continue;
				}
				printf( "\n%s - file opened successfully.\n",file1 ) ;

				if ( filePointerO2 == NULL ) // check for sucessfull file opening 
				{
					printf( "%s file failed to open.",file2) ;
					continue;
				}
				printf( "\n%s  file opened successfuly.\n",file2 ) ;

					strcpy(message_sent,input);
					write(client_socket_id, message_sent, sizeof(message_sent)); // writing to client 
					printf("\nCommand sent to server\n");

					read(client_socket_id, message_recd, sizeof(message_recd)); // reading from client
					printf("\n Message from server:%s\n",message_recd);
					strcpy(message_recd,"");
					strcpy(message_sent,"");

					send_file(filePointerO1, client_socket_id);
					fclose(filePointerO1);
					printf("\nFile - 1 data sent successfully.\n");
					
					read(client_socket_id, message_recd, sizeof(message_recd)); // <-- 4
					if (!strcmp(message_recd, "Error")){
					printf("\n File 1 is invalid - %s \n",file1);
					continue;
					}
					else if(!strcmp(message_recd, "Valid"))
					{
					printf("\nFile 1 is valid - %s \n",message_recd);
					}
					bzero(message_recd,256);

					send_file(filePointerO2, client_socket_id);
					fclose(filePointerO2);
					printf("\2File - 2 data sent successfully.\n");

					read(client_socket_id, message_recd, sizeof(message_recd)); // <-- 4
					if (!strcmp(message_recd, "Error")){
					printf("File 2 is invalid - %s \n",file2);
					continue;
					}else if(!strcmp(message_recd, "Valid"))
					{
					printf("File 2 is valid - %s \n",message_recd);
					}
					bzero(message_recd,256);

					strcpy(message_sent,"test");
					write(client_socket_id, message_sent, sizeof(message_sent)); // writing to client 


					filePointerO1 = fopen("similarity.txt", "w") ;
					if ( filePointerO1 == NULL ) // check for sucessfull file opening 
					
					{printf( "\nfile failed to open.\n") ;
					continue;}
					usleep(100);
					recieve_file(client_socket_id,filePointerO1);
					fclose(filePointerO1);
					filePointerT = fopen("similarity.txt", "r") ;
					char data[MAX];
					fscanf(filePointerT,"%[^EOF]",data);
					printf(" output of similarity option \n\n");
					printf("%s\n\n",data);
					fclose(filePointerO1);
					bzero(data,MAX);

			}
        }
        else if(!strcmp(command,"exit"))
        {
           parse_count = sscanf(input,"/%s %[^\n]",command,catch);
		   if(parse_count > 1 )
			{
				printf("\nINVALID command in exit : PLEASE TRY AGAIN \n\n");
				continue;
			}

		    printf("\nClosing connection to server......\n");
			break;
            /*strcpy(message_sent, "INVALID COMMAND : PLEASE TRY AGAIN");
            //write(client_id, message_sent, sizeof(message_sent));
            printf("\nback to the client\n\n");
            strcpy(message_recd,"");
            strcpy(message_sent,"");*/
		}else
		{	printf("Parse count is : %d\n", parse_count);
			printf("command : %s \n",command);
			printf("\nINVALID COMMAND : PLEASE TRY AGAIN \n\n");
		}

 	}// back to while

close(client_socket_id);
return 0;

}
