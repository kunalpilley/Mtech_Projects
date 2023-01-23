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
#include<stdbool.h>
#include<sys/stat.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/mman.h>
//*************************************************************MACROS***************************************************************************||

#define Portnumber 5100
#define MAX 10000

#define USERS 1
#define FILES 2
#define UPLOAD 3
#define DOWNLOAD 4
#define INVITE 5
#define READ 6
#define INSERT 7
#define DELETE 8
#define EXIT 9
#define YES 10
#define NO  11

//***************************************************************STRUCTURES**********************************************************************||

char file_name[20];
int execute_command(char *input , int sockfd);
int parse_command(char *input);

//***************************************************************END OF STRUCTURE****************************************************************||

//******************************GetFile_function_*****************************************
void GetFile(char *FileName, char *buffer)
{
    FILE *fp = fopen(FileName,"w");
    if(fp == NULL)
    {
        printf("error in creating file to write:\n");
        return;
    }
    int count = fwrite(buffer,sizeof(char),strlen(buffer),fp);
    if(count == MAX)
    {
        printf ("Failed to write the content:\n");
        bzero(buffer,MAX);
        fclose(fp);
        return ;
    }
    fclose(fp);
    bzero(buffer,MAX);
    return;
}

//****************************************************************************************

int FileTransfer(char *FileName, char *buffer)
{
   FILE *fp = fopen(FileName,"r");
   if(fp == NULL)
   {
     printf("Failed to open the file \"%s\"\n",FileName);
     return -1;
   } 
   bzero(buffer,MAX);
   int count = fread(buffer, sizeof(char), MAX, fp);
   if(count == MAX && getc(fp) != EOF)
   {
    printf("Size of the file  Exceededs the size of the buffer, failed to transfer:\n");
    bzero(buffer,MAX);
    fclose(fp);
    return -1;
   }
   fclose(fp);
   return 0;
}

int parse_command(char *input)
{ 

    int parse_count = 0;
    char command[10] = "", catch[5] = "";
    //file_name[20]; taken  as global variable 
    bzero(file_name,20);
    int client_id;
    char permission[2] = "";
    int start_idx, end_idx,idx;
    char message[256] = "";

    parse_count = sscanf(input,"/%s",command);
    //printf("\ncommand : %s\n",command);

     if(!strcmp(command,"users") ) // valid command with only no other argument
     { 
       
        parse_count = sscanf(input,"/%s %[^\n]",command,catch);
        if(parse_count>1)
        {
          printf("\nInvalid command in users : Try Again!!\n");
          return -1;
        }
        else
        { 
          printf("\nValid command in users\n");
          return USERS;
        }


     }
     else if(!strcmp(command,"files") )
     { 
       parse_count = sscanf(input,"/%s %[^\n]",command,catch);
       if(parse_count>1)
       {
         printf("\nInvalid command in files : Try Again!!");
         return -1;
       }
       else
       {
         printf("\nValid command in files\n");
        return FILES;
       }

     }
     else if(!strcmp(command,"upload") )
     {
        parse_count = sscanf(input,"/%s %s %[^\n]",command,file_name,catch);
        if(parse_count <2 || parse_count>2)
        {
          printf("\nINVALID command in UPLOAD : PLEASE TRY AGAIN \n\n");
          return -1;
        }
        else if( parse_count == 2)
        {
            struct stat buffer;
            int exist = stat(file_name,&buffer);
            if(exist == 0)
            { 
              printf("\nValid command in upload\n");
                return UPLOAD ;
            }
            else
            {   
                printf("\nInvalid command in upload - file %s doesnt exist\n", file_name);
                return -1;
            }
        }

     }
     else if(!strcmp(command,"download") )
     {
       parse_count = sscanf(input,"/%s %s %[^\n]",command,file_name,catch);

       /* printf("Parse count is : %d\n", parse_count);
        printf("command : %s \n",command);
        printf("command LEN: %lu \n",strlen(command));
        printf("filename : %s \n", file_name);
        printf("command LEN: %lu \n",strlen(file_name));
        printf("catch : %s \n", catch);
        printf("fdlen: %lu \n",strlen(catch));*/

        if(parse_count <2 || parse_count>2) //|| strlen(field)==0 || strlen(field)>1 
        {
          printf("\nINVALID command in Download : PLEASE TRY AGAIN \n\n");
          return -1;
        }
        else
        { 
        printf("\nValid command in download\n");
        return DOWNLOAD;
        }

     }
     else if(!strcmp(command,"invite") )
     {
       parse_count = sscanf(input,"/%s %s %d %s %[^\n]",command,file_name,&client_id,permission,catch);
		 	/*printf("Parse count is : %d\n", parse_count);
      
			printf("command : %s \n",command);
      printf("command len: %lu \n",strlen(command));

			printf("filename : %s \n", file_name);
			printf("file name  len : %lu \n",strlen(file_name));

			printf("client id  : %d \n", client_id);
			//printf("command LEN: %lu \n",strlen(file2));

			printf("permission : %s \n", permission);
			printf("permission  LEN: %lu \n",strlen(permission));

			printf("catch : %s \n", catch); 
			printf("catch len: %lu \n",strlen(catch));*/
      
			if(parse_count <4 || parse_count >4 || strlen(permission)==0 || strlen(permission)>1 || ((strcmp(permission,"V")&&strcmp(permission,"E"))))
			{
				printf("\nINVALID command in invite : PLEASE TRY AGAIN \n\n");
				return -1;
			}
			else
      { 
			 printf("\n in invite\n");
       return INVITE;
      }

     }
     else if(!strcmp(command,"read") )
     { 
        parse_count = sscanf(input,"/%s %s %d %d %[^\n]",command,file_name,&start_idx,&end_idx,catch);

        /*printf("Parse count is : %d\n", parse_count);
        
        printf("command : %s \n",command);
        printf("command len: %lu \n",strlen(command));

        printf("filename : %s \n", file_name);
        printf("file name  len : %lu \n",strlen(file_name));

        printf("start idx  : %d \n", start_idx);
        //printf("command LEN: %lu \n",strlen(file2));

        printf("end idx  : %d \n", end_idx);
        //printf("permission  LEN: %lu \n",strlen(permission));

        printf("catch : %s \n", catch); 
			  printf("catch len: %lu \n",strlen(catch));*/

        if(parse_count> 4 || parse_count< 2)
        {
          printf("\nINVALID command in read : PLEASE TRY AGAIN \n\n");
          return -1;

        }

        if(parse_count ==3)
        {
            parse_count = sscanf(input,"/%s %s %d %[^\n]",command,file_name,&start_idx,catch);
            if(parse_count>3)
            { 
            printf("\nINVALID command in read -3 : PLEASE TRY AGAIN \n\n");
            return -1;
            }
            else
            {
              printf("\nVALID command in read -3 \n\n");
              return READ;

            }
        }

        if(parse_count ==2)
        {
            parse_count = sscanf(input,"/%s %s %[^\n]",command,file_name,catch);
            if(parse_count>2)
            { 
            printf("\nINVALID command in read -2 : PLEASE TRY AGAIN \n\n");
            return -1;
            }
            else
            {
              printf("\nVALID command : read entire file \n\n");
              return READ;
            }

        }

        if(parse_count ==4)
        {
            if(start_idx<end_idx && ((start_idx<0&&end_idx<0)||(start_idx>=0&&end_idx>0)))
            {
              return READ;
            }
            else
            { 
            printf("\nINVALID index in READ : PLEASE TRY AGAIN \n\n");
            return -1;
            }
        }
        
     }
     else if(!strcmp(command,"insert") )
     {
        // parse_count = sscanf(input,"/%s %s %d \"%s\" %[^\n]",command,file_name,&start_idx,message,catch);
        parse_count = sscanf(input,"/%s %s %d \"%[^\"]\" %[^\n]",command,file_name,&start_idx,message,catch);


       /* printf("Parse count is : %d\n", parse_count);
        
        printf("command : %s \n",command);
        printf("command len: %lu \n",strlen(command));

        printf("filename : %s \n", file_name);
        printf("file name  len : %lu \n",strlen(file_name));

        printf("start idx  : %d \n", start_idx);
        //printf("command LEN: %lu \n",strlen(file2));

        printf("message: %s \n", message);
        printf("message LEN: %lu \n",strlen(message));

        printf("catch : %s \n", catch); 
			  printf("catch len: %lu \n",strlen(catch));*/

        if(parse_count> 4 || parse_count< 2 )// || strlen(message)==0
        {
          printf("\nINVALID command in Insert : PLEASE TRY AGAIN \n\n");

          return -1;

        }
        if(parse_count == 4)
        {
            printf("perfect command inside without index  \n\n") ;
            return INSERT;

        }
        if(parse_count == 2)
        {
          printf("\n inside 2 \n");
          parse_count = sscanf(input,"/%s %s \"%[^\"]\" %[^\n]",command,file_name,message,catch); 

          /*printf("Parse count is : %d\n", parse_count);
        
          printf("command : %s \n",command);
          printf("command len: %lu \n",strlen(command));

          printf("filename : %s \n", file_name);
          printf("file name  len : %lu \n",strlen(file_name));

          printf("message : %s \n", message);
          printf("message : %lu \n",strlen(message));

          printf("catch : %s \n", catch); 
          printf("catch len: %lu \n",strlen(catch));*/

          if(parse_count == 4)
          { printf("\nINVALID command in Insert  after message field : PLEASE TRY AGAIN \n\n");
            return -1;
          }
          else if(parse_count == 3)
          {
             printf("perfect command inside without index  \n\n") ;
             return INSERT;
          }
          else if(parse_count == 2)
          {
            printf("\nINVALID command in Insert - 2: PLEASE TRY AGAIN \n\n");
          return -1;

          }
          
        }

      }
     else if(!strcmp(command,"delete") )
      {  parse_count = sscanf(input,"/%s %s %d %d %[^\n]",command,file_name,&start_idx,&end_idx,catch);

       /*printf("Parse count is : %d\n", parse_count);
        
        printf("command : %s \n",command);
        printf("command len: %lu \n",strlen(command));

        printf("filename : %s \n", file_name);
        printf("file name  len : %lu \n",strlen(file_name));

        printf("start idx  : %d \n", start_idx);
        //printf("command LEN: %lu \n",strlen(file2));

        printf("end idx  : %d \n", end_idx);
        //printf("permission  LEN: %lu \n",strlen(permission));

        printf("catch : %s \n", catch); 
			  printf("catch len: %lu \n",strlen(catch));*/

        if(parse_count> 4 || parse_count< 2)
        {
          printf("\nINVALID command in read : PLEASE TRY AGAIN \n\n");
          return -1;

        }
        if(parse_count ==3)
        {
          parse_count = sscanf(input,"/%s %s %d %[^\n]",command,file_name,&start_idx,catch);
          if(parse_count>3)
          { 
          printf("\nINVALID command in delete -3 : PLEASE TRY AGAIN \n\n");
          return -1;
          }
          else
          {
            printf("\n valid in delete -3\n");
            return DELETE;
          }

        }
        if(parse_count ==2)
        {
          parse_count = sscanf(input,"/%s %s %[^\n]",command,file_name,catch);
          if(parse_count>2)
          { 
          printf("\nINVALID command in delete -2 : PLEASE TRY AGAIN \n\n");
          return -1;
          }
          else
          {
            printf("\n valid in delete -2\n");
            return DELETE;
          }

        }
        if(parse_count == 4)
        {
            if(start_idx<end_idx && ((start_idx<0&&end_idx<0)||(start_idx>=0&&end_idx>0)))
            {
              return DELETE;
            }
            else
            { 
            printf("\nINVALID index in delete : PLEASE TRY AGAIN \n\n");
            return -1;
            }
        }

      }
     else if(!strcmp(command,"exit") )
      {
          parse_count = sscanf(input,"/%s %[^\n]",command,catch);
          if(parse_count>1)
          { 
          printf("\nINVALID command in Exit \n\n");
          return -1;
          }
        return EXIT;

      }
     else if(!strcmp(command,"yes") )
      {
       parse_count = sscanf(input,"/%s %[^\n]",command,catch);
       if(parse_count>1)
        { 
          printf("\nINVALID command in yes \n\n");
          return -1;
         }
       printf("\n in yes \n");
       return YES;

      }
     else if(!strcmp(command,"no") )
      {
       
       printf("\n in No \n");
       if(parse_count>1)
       { 
        printf("\nINVALID command in NO \n\n");
        return -1;
       }
       return NO;

      }
     else 
      {
       printf("\nInvalid Command : Try Again!!");
       return -1;

      }

  return -1;
}

int execute_command(char *input , int sockfd)
{
   int n;
   char message_recd[256];
   char message_sent[256];
   char buffer[MAX];
   int choice = parse_command(input);
   //printf("\nparse value choice = %d\n", choice);// hide  
   switch(choice)
   {
     case USERS : 
     
        printf("\nClient : Executing USERS \n\n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX);
        printf("\nServer Respons\n");
        puts(buffer);
        return USERS;

     case FILES :

        printf("\nClient : Executing FILES \n"); 
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX);
        printf("\nServer Respons\n");
        puts(buffer);
        return FILES;

     case UPLOAD : 

        printf("\nClient : executing UPLOAD \n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(message_recd,256);
        n = read(sockfd,message_recd,256);
        printf("\nserver response : ");
        printf("%s",message_recd);
        if(strcmp(message_recd,"INVALID : filename already exists on server")==0)
        {
           printf("\nTerminating operations\n");
           return UPLOAD;
        }
        bzero(buffer,MAX);
        FileTransfer(file_name,buffer);
        n = write(sockfd,buffer,strlen(buffer)); // transfering file to server
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX); // reding acknowledgement if file is sucessfully uploaded
        printf("\nserver response : \n\n");
        puts(buffer);
        printf("\nfile transfer complete\n");
        bzero(buffer,MAX);
        return UPLOAD;

     case DOWNLOAD : 

        printf("\nclient : executing DOWNLOAD \n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX); // reding acknowledgement if file is sucessfully uploaded 
        if(strncmp(buffer,"INVALID",7)==0 || (strncmp(buffer,"Fail",4) == 0))
        {
           puts(buffer);
           printf("\nTerminating operations\n");
           return -1;   
        }
        GetFile(file_name,buffer); // saving file
        puts("File downloaded sucessfully");
        bzero(buffer,MAX);  
        return DOWNLOAD ;
        
     case INVITE :

        printf("\nclient : executing Invite \n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX);
        if(strncmp(buffer,"INVALID",7)==0)
        {
           puts(buffer);
           printf("\nTerminating operations\n");
           return -1;   
        }
        printf("\nserver response : \n"); //remove in case of problem 1
        puts(buffer);// message - Collabaration request recieved and authenticated
        bzero(buffer,MAX);
        snprintf(buffer,200,"Client Response :\nWaiting for Invitation status :\n");
        write(sockfd,buffer,strlen(buffer));
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX);
        puts(buffer);
        return INVITE ;

     case READ : 

        printf("\nclient : executing READ \n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX);
        printf("bytes recived = %d\n",n); 
        puts("\nmessage from server :\n");
        if(strncmp(buffer,"INVALID",7)==0 || (strncmp(buffer,"Fail",4) == 0))
        {
           puts(buffer);
           return -1;   
        }
        int i = 0;
        while (i < n)
        {
          putchar(buffer[i]);
          i++;
        }
        i =0;
        return READ ;

     case INSERT : 

        printf("\nClient : executing INSERT \n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX); // reading acknowledgement if file is sucessfully uploaded
        
        printf("\nResponse from the sever:\n");
        i =0;
        while (i < n)
        {
          putchar(buffer[i]);
          i++;
        }
        i =0;
        //puts(buffer);
        printf("\nbytes recieved - %d\n",n);

        return INSERT;

     case DELETE :  

        printf("\nclient : executing DELETE \n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        bzero(buffer,MAX);
        n = read(sockfd,buffer,MAX); // reading acknowledgement if file is sucessfully uploaded 
        printf("\nbytes recieved - %d\n",n);
        printf("\nResponse from the sever :\n");
        i =0;
        while (i < n)
        {
          putchar(buffer[i]);
          i++;
        }
        i =0;
        
        return DELETE;

     case EXIT :

        printf("\nclient : executing Exit \n");
        n = write(sockfd,input,strlen(input));
        printf("\nInstructions sent to the sever\n");
        return EXIT;

     case YES : 
         
        printf("\nclient : executing YES \n");
        n = write(sockfd,input,strlen(input));
        bzero(buffer,MAX);
        read(sockfd,buffer,MAX);   // reply of acceptance 
        puts(buffer);
        return YES;

     case NO : 

        printf("\nclient : executing NO \n");
        n = write(sockfd,input,strlen(input));
        bzero(buffer,MAX);
        read(sockfd,buffer,MAX);   // reply of acceptance 
        puts(buffer);
        return NO ;

     default : 

        printf("\n in default of execution\n");
        return -1;


   }
  
}



int main()
{
    int listen_fd = 0;
    int new_socket = 0;
    int max_fd = 0;
    fd_set readfds;
    char buffer[MAX];
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct hostent *server; 
    server = gethostbyname("localhost"); //this command resolves host address to corresponding IP address
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr)); // initializes buffer
    serv_addr.sin_family = AF_INET; // for IPv4 family
    bcopy((char *)server->h_addr, (char *) /* copy server IP address*/&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(Portnumber); //defining port number
    if (connect(listen_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0 ) 
    { 
        printf("ERROR Connecting"); 
    } 

    // initiating connect request to the server

   

  char message_sent[256];
	char message_recd[256];
  char input[1024];
  read(listen_fd, message_recd, sizeof(message_recd));
	if ( !strcmp(message_recd, "Max clients connected, try again later\n") )
  {
		printf("\n\nMessage from server : %s\n", message_recd);
		return 0;
	}
	printf("\n\nMessage from server : %s\n", message_recd);
     int k; 

 while (1)
    {
        max_fd = listen_fd;
        FD_ZERO(&readfds);
        char reply[100];
        FD_SET(listen_fd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
        bzero(buffer, MAX);
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (activity == -1 || activity == 0)
        {
            perror("ERROR: select");
            exit(0);
        }

        if (FD_ISSET(listen_fd, &readfds))
        {
           
            if (read(listen_fd, buffer, MAX) > 0)
            {
                puts(buffer);//printf(" --%s",buffer);
                /*if (strncmp(buffer, "Client with socket id :", 23) == 0)
                {
                    bzero(reply, 100);
                    while (strcmp(reply, "YES") == 0 || strcmp(reply, "NO") == 0)
                    {
                        bzero(reply,100);
                        if (FD_ISSET(STDIN_FILENO, &readfds))
                        {
                            read(0,reply,100);
                        }
                    }
                    execute_command(reply,listen_fd);
                }*/
            }
        }
          else if(FD_ISSET(STDIN_FILENO, &readfds))
                  {
                   if(scanf("%[^\n]", buffer)>0)// if (read(0, buffer, MAX) > 0)
                     {
                        fflush ( stdin ) ;fflush(stdout);
                        if(execute_command(buffer,listen_fd)== EXIT)
                        break;
                        printf("\nEnter Command :\n");
                        //fseek(stdin, 0, SEEK_END);
                        bzero(buffer, MAX);
                        // write(listen_fd,buffer,strlen(buffer));
                     }
                  }
              
    }

    close(listen_fd);
    return 0;

}
