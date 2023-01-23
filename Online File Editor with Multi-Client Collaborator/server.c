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
#include <sys/shm.h>
#include <sys/mman.h>
#include<signal.h>
#include <errno.h>
#include<sys/stat.h>
//****************************************************END OF HEADER FILES****************************************************************************************||


//*****************************************************************START OF MACROS*******************************************************************************||

//#define Portnumber 5100
#define MAX 10000
#define PORT 5100
#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS 5

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


//**********************************************************END OF MACROS**************************************************************************************||

//*******************************************************STRUCTURES and Global Variables************************************************************************||


int  client_id[100][2] = {0};
int file_create_count = 0;

struct permissions{
	int flag;
	char file_name[100];
	int client_id[5];
	char permission[5];
	int lines;
};
struct permissions file_permission[100];

struct invitation_record
{
    int from ;
    int to ;
    char file_name[100];
    char permission;
    
} ;
struct invitation_record invitation[10];

//************************************************************FUNCTIONS**************************************************************************||

void new_client_id( int );
void read_lines(char *buffer,char *input);
void files_summery(char *buffer);
void insert_lines(char* buffer,char *input);
void delete_lines(char *buffer,char *input);
void user_exit(int socket);
void active_users(int socket,char * buffer);
int add_permission_record( char * file_name, int owner_scoket);
void new_client_id( int socket);
int check_read_permission(int asking_client_id ,char *req_file_name);
int check_edit_permission(int asking_client_id ,char *req_file_name);
int Is_Owner(int socket,char *req_file_name);
int get_socket_id(int To_client);
int add_collaborator(int invited);
int reset_invitation(int invited);
void Record_Invitation_request(int from_client , int to_client , char *file_name , char permission);
int FileTransfer(char *FileName, char *buffer);
int GetFile(char *FileName, char *buffer);
int NLINEX(char *file_name);
void conver_to_multiple_lines(char *txt, char *amended_text);


int reset_invitation(int invited)
{
    int i;int inviter ;
    for(i =0 ; i<10 ; i++)
    {
        if(invitation[i].to == invited)
        {
            // printf("break 1 %d\n",i);
            // inviter = invitation[i].from ;
            // invitation[i].from = 0;
            // invitation[i].to = 0;
            // bzero(invitation[i].file_name,100);
            // invitation[i].permission = '\0';
            break ;
        }
    }

    if(i >= 10)
    {
        
        printf("\nno entry for invited client foundn\n");
        return -1;
    }
    else
    {
        printf("pending request \n");

        int inviter = invitation[i].from ;
        invitation[i].from = 0;
        invitation[i].to = 0;
        bzero(invitation[i].file_name,100);
        invitation[i].permission = '\0';
        
        for(int j = 0; j<10 ; j++)
        {
            if(invitation[j].from != 0)
            {
                printf("%d  %d  %s  %c\n",invitation[j].from +10000 , invitation[j].to +10000,invitation[j].file_name,invitation[j].permission);
            }
        }
        return inviter ;
    }
     
    
}

int NLINEX(char *file_name)
{
    FILE *filePointer = fopen(file_name,"r");
    if(filePointer==NULL)
      {
          printf("Error in opening file : %s\n",file_name);
          return -1;
      }
    char line[256];
    int line_count=0;

    while(fgets(line,256,filePointer)!= NULL) // counting the no of lines 
            line_count++;
    
    fclose(filePointer);
    return line_count;
}

void Record_Invitation_request(int from_client , int to_client , char *file_name , char permission)
{
    int i ;
    for(i = 0; i<10 ; i++)
    {
        if(invitation[i].from == 0)
        {
            break ;
        }
    }
    if(i < 10)
    {
        invitation[i].from = from_client ;
        invitation[i].to = to_client ;
        invitation[i].permission = permission;
        strcpy(invitation[i].file_name,file_name);
    }

     for(i = 0; i<10 ; i++)
    {
        if(invitation[i].from != 0)
        {
            printf("%d  %d  %s  %c\n",invitation[i].from , invitation[i].to,invitation[i].file_name,invitation[i].permission);
        }
    }

}

int GetFile(char *FileName, char *buffer)
{
    FILE *fp = fopen(FileName,"w");
    if(fp == NULL)
    {
        printf("error in creating file to write:\n");
        return -1;
    }
    int count = fwrite(buffer,sizeof(char),strlen(buffer),fp);
    /*if(count == MAX)
    {
        printf ("Failed to write the content:\n");
        bzero(buffer,MAX);
        fclose(fp);
        return -1;
    }*/
    fclose(fp);
    bzero(buffer,MAX);
    return 1;
}


void new_client_id( int socket)
{
    int client_random_id;
    client_random_id = 10000 + socket;

    for (int i = 0; i <5; i++)
    {
		if ( client_id[i][0] == 0)
        {
			client_id[i][0] = socket;
			client_id[i][1] = client_random_id;
			break;
		}
	}
}

int add_permission_record( char * file_name, int owner_scoket)
{
    file_create_count++;
    file_permission[file_create_count].flag = 1;
	strcpy(file_permission[file_create_count].file_name, file_name);

    file_permission[file_create_count].client_id[0] = owner_scoket;
    file_permission[file_create_count].permission[0] = 'O';

    for(int i = 1;i<=4;i++)
    { 
        file_permission[file_create_count].client_id[i] = 0;
        file_permission[file_create_count].permission[i] = '\0';

    }

    //printf("%d\n",file_permission[file_create_count].flag);
    printf("%s\n",file_permission[file_create_count].file_name);

    for(int i = 0;i<=4;i++)
    { 
        printf("socket - %d ",file_permission[file_create_count].client_id[i])  ;  //0;
        printf("permission - '%c'\n",file_permission[file_create_count].permission[i]);  //'\0';

    }

    return 1;
}

int check_read_permission(int asking_client_id ,char *req_file_name)
{
    for(int i=1 ; i<=file_create_count;i++)
    { 
        if(file_permission[i].flag == 1 && !(strcmp(file_permission[i].file_name,req_file_name)))
        {
            
            for(int j = 0;j<=4;j++)
            { 
                if((file_permission[i].client_id[j] == asking_client_id) && (file_permission[i].permission[j] =='O'||file_permission[i].permission[j] == 'E'||file_permission[i].permission[j] == 'V'))
                return 1;
            }

        }
    }
    return 0;
}

int check_edit_permission(int asking_client_id ,char *req_file_name)
{
    for(int i=1 ; i<=file_create_count;i++)
    { 
        if(file_permission[i].flag == 1 && !(strcmp(file_permission[i].file_name,req_file_name)))
        {
            
            for(int j = 0;j<=4;j++)
            { 
                if((file_permission[i].client_id[j] == asking_client_id) && (file_permission[i].permission[j] =='O'||file_permission[i].permission[j] == 'E'))
                return 1;

            }

        }
    }
    return 0;
}
//=================== Prepare the summary for all avilable files at server ==========================================//
void files_summery(char *buffer)
{
    printf("\n in file summary\n");
    char line[512];
    FILE *tmp = tmpfile();
    int flag2 = 0;
    for(int i=1 ; i<=file_create_count;i++)
    { 
        
        if(file_permission[i].flag == 1)
        {
            sprintf(line,"%s\t||",file_permission[i].file_name);
            fprintf(tmp,"%s",line);
            
            for(int j = 0;j<=4;j++)
            { 
                if((file_permission[i].client_id[j] !=0) && (file_permission[i].permission[j] =='O'||file_permission[i].permission[j] == 'E'||file_permission[i].permission[j] == 'V'))
                {
                    sprintf(line,"  %d  '%c'  ||",file_permission[i].client_id[j]+10000,file_permission[i].permission[j]);
                    fprintf(tmp,"%s",line);
                }

            }

            file_permission[i].lines = NLINEX(file_permission[i].file_name);
            sprintf(line,"  line count : %d\n",file_permission[i].lines);
            fprintf(tmp,"%s",line);

            flag2 = 1;

        }
    }
    rewind(tmp);
    int count = fread(buffer, sizeof(char), MAX,tmp);

    if(count == 0 && flag2 == 0)
    sprintf(buffer,"No file uploaded at server till now by any client");
    else if(count == 0)
    sprintf(buffer,"Error in reading files data.");


    printf("byte sent :%d\n ",count);
    return;

}
//===============================================================================================================================//

int get_socket_id(int To_client)
{
    for(int i = 0 ; i<5 ; i++)
    {
        if(client_id[i][1] == (To_client))
        {
            return client_id[i][0] ; // returning socket id
        }
       
    }
    return -1;
}

int Is_Owner(int socket,char *req_file_name)
{
    for(int i=1 ; i<=file_create_count;i++)
    { 
        if(file_permission[i].flag == 1 && !(strcmp(file_permission[i].file_name,req_file_name))&& (file_permission[i].client_id[0] == socket) && (file_permission[i].permission[0] == 'O'))
        return 1;          
    }  
    return 0; 
}

void user_exit(int socket)
{
	int id;

    // deleted the client entry from the clients record 

	for (int i = 0; i < 5; i++)
    {
		if (client_id[i][0] == socket)
        {

			client_id[i][0] = 0;
			client_id[i][1] = 0;
		}
	}

    // deleted the client entry from the files  record as well 
	for (int i = 1; i < 100; i++)
    {
		if (file_permission[i].flag == 1)
        {

			if(file_permission[i].client_id[0]==socket)
            {
                file_permission[i].flag = 0;
                remove(file_permission[i].file_name);
                bzero(file_permission[i].file_name, 100);
                for (int j = 0; j < 5; j++)
                {
                file_permission[i].client_id[j] = 0;
                file_permission[i].permission[j] = '\0';
                }

            } 
            else
            {
                for (int j = 1; j < 5; j++)
                {
                    if(file_permission[i].client_id[j] == socket)
                    {
                        file_permission[i].client_id[j] = 0;
                        file_permission[i].permission[j] = '\0';
                    }
                }

            }
		}
	}

	return;
}

//=================================== gives the list of the active users to client==============================//

void active_users(int socket,char *buffer)
{
    char line[256];
    FILE *tmp = tmpfile();
    
    for (int i = 0; i < 5; i++)
    {
		if (client_id[i][1] != 0)
        {
            if(client_id[i][0] == socket)
            {sprintf(line,"%d (own id)\n",client_id[i][1]);
            fprintf(tmp,"%s",line);}
            else
            {sprintf(line,"%d\n",client_id[i][1]);
            fprintf(tmp,"%s",line);}

		}
	}
    rewind(tmp);
    int count = fread(buffer, sizeof(char), MAX,tmp);
    // printf("-%d- ",count);
    if(count==0)
    sprintf(buffer,"\n Error in reading users data\n");

    return;
}
//=================================== end===============================================================//


int execute_command(char *buffer , int newsocket)
{
    int parse_count;
    char command[10] = "", catch[5] = "";
    char file_name[20] = "";
    int client_id;
    char permission = '\0';
    int start_idx, end_idx,idx;
    char message[256] = "";
    char instruction[512] = "";
    int n = 0;
    int n1 = 0;

    printf( "Command from client is :  %s \n\n" , buffer);
    parse_count = sscanf(buffer,"/%s",command);

    if(!strcmp(command,"users") ) // valid command with only no other argument
    {   
        printf("\nServer: excecuting users:\n");
        bzero(buffer,MAX);
        active_users(newsocket,buffer);
        n = write(newsocket,buffer,strlen(buffer)); // file transfer  
        printf("\nServer: Response sent to client\n");
        return USERS;

    }
    else if (!strcmp(command,"files"))
    {
        printf("\nServer: excecuting files:\n");
        bzero(buffer,MAX);
        files_summery(buffer);
        n = write(newsocket,buffer,strlen(buffer)); // file transfer  
        printf("\nServer: Response sent to client\n");
        return FILES;

    }
    else if (!strcmp(command,"upload"))
    {
        printf("\nServer: excecuting upload:\n");
        sscanf(buffer,"/%s %s",command,file_name);
        struct stat buf;
        int exist = stat(file_name,&buf);
        bzero(buffer,MAX);
        if(exist == -1)
        { 
            printf("Valid file name - requesting client to upload\n");
            snprintf(buffer,200,"Valid : please upload the file\n");
            write(newsocket,buffer,strlen(buffer));
            
        }
        else if(exist == 0)
        {   
            printf("INVALID : filename already exists on server\n");
            snprintf(buffer,200,"INVALID : filename already exists on server");
            write(newsocket,buffer,strlen(buffer));
            bzero(buffer,MAX);
            return UPLOAD;
        }
        bzero(buffer,MAX);// acknowledging command parsing.
        n = read(newsocket,buffer,MAX); // downloading file

        if(GetFile(file_name,buffer))
        {
            bzero(buffer,MAX);
            snprintf(buffer,200,"\nfile recd successfully by server");
            n = write(newsocket,buffer,strlen(buffer));
            n1 = add_permission_record(file_name,newsocket);
            if(n1)
            printf("\nfile persmssion record added.\n");
        }
        else
        {
            bzero(buffer,MAX);
            snprintf(buffer,200,"file not recd successfully : try again\n");
            n = write(newsocket,buffer,strlen(buffer));
            printf("\nfile not recd successfully : try again.\n");


        }
        bzero(buffer,MAX);
        printf("\nServer: Response sent to client\n");
        return UPLOAD;


    }
    else if (!strcmp(command,"download"))
    { 
        int ret;
        printf("\nServer: excecuting Download:\n");
        sscanf(buffer,"/%s %s",command,file_name);
        struct stat buf;
        int exist = stat(file_name,&buf);
        bzero(buffer,MAX);
        if(exist == -1)
        { 
            printf("INVALID : file doesn't exist\n");
            snprintf(buffer,200,"INVALID : file doesn't exist\n");
            write(newsocket,buffer,strlen(buffer));
            return DOWNLOAD;
        }
        ret = check_read_permission(newsocket,file_name);
        //printf(" \n==ret %d\n", ret);
        if( ret ==0 )
        {
            /* access priviladge check*/
            printf("INVALID : file access not authorised\n");
            snprintf(buffer,200,"INVALID : file access not authorised\n");
            write(newsocket,buffer,strlen(buffer));
            return DOWNLOAD;
        }
        else if(exist == 0)
        {   
            bzero(buffer,MAX);
            if( FileTransfer(file_name,buffer) == 0)
            n = write(newsocket,buffer,strlen(buffer)); // file transfer  
            else
            {
                bzero(buffer,MAX);
                snprintf(buffer,200,"Fail to send file/FILE EMPTY plz try again:\n");
                write(newsocket,buffer,strlen(buffer));
                return DOWNLOAD;
            }
            printf("\n%s sent successfully to the client \n",file_name);

        }

    }
    else if (!strcmp(command,"invite"))
    {
        printf("server: excecuting invite:\n");
        sscanf(buffer,"/%s %s %d %c",command,file_name,&client_id,&permission);
        bzero(buffer,MAX);
        struct stat buf;
        int exist = stat(file_name,&buf);
        
        if(exist == -1)
        { 
            printf("INVALID : file doesn't exist\n");
            snprintf(buffer,200,"INVALID : file doesn't exist\n");
            write(newsocket,buffer,strlen(buffer));
            return INVITE;
        }
        int to_socket = get_socket_id(client_id);
        if(to_socket == newsocket)
        { 
            printf("INVALID : from and to client are same\n");
            snprintf(buffer,200,"INVALID : from and to client are same\n");
            write(newsocket,buffer,strlen(buffer));
            return INVITE;
        }

        if(to_socket == -1)
        { 
            printf("INVALID : Target client not active\n");
            snprintf(buffer,200,"INVALID : Target client not active\n");
            write(newsocket,buffer,strlen(buffer));
            return INVITE;
        }
        if(Is_Owner(newsocket,file_name) == 0)
        { 
            printf("INVALID :  your are not the Owner\n");
            snprintf(buffer,200,"INVALID : your are not the Owner\n");
            write(newsocket,buffer,strlen(buffer));
            return INVITE;
        }
        snprintf(buffer,200,"Collabaration request recieved and authenticated\n");
        write(newsocket,buffer,strlen(buffer));
        Record_Invitation_request(newsocket,to_socket,file_name,permission);
        bzero(buffer,MAX);
        snprintf(buffer,200,"Client id :\"%d\" Invited you as a collaborator for File:\" %s\" with permission as:'%c' :\nPlease enter \"/yes\" or \"/no\" :\n",newsocket+10000,file_name,permission);
        write(to_socket,buffer,strlen(buffer));
        bzero(buffer,MAX);// might remove 2
        read(newsocket,buffer,MAX); // i--"Waiting for Invitation status";ncoming client request to process result 
        puts(buffer);
        bzero(buffer,MAX);
        snprintf(buffer,200," Invitation request is registered successfully, waiting for response :\n");
        write(newsocket,buffer,strlen(buffer)); // sending invitation status result
        return INVITE ;
    }
    else if (!strcmp(command,"read"))
    {
        printf("server: excecuting read:\n");
        strcpy(instruction,buffer);
        sscanf(buffer,"/%s %s",command,file_name);
        struct stat buf;
        int exist = stat(file_name,&buf);
        bzero(buffer,MAX);
        if(exist == -1)
        { 
            printf("INVALID : %s doesn't exist\n",file_name);
            snprintf(buffer,200,"INVALID : file doesn't exist\n");
            write(newsocket,buffer,strlen(buffer));
            return READ;
        }
        else if(check_read_permission(newsocket,file_name) == 0)
        {
            /* access priviladge check*/
            printf("INVALID : file access not authorised\n");
            snprintf(buffer,200,"INVALID : file access not authorised\n");
            write(newsocket,buffer,strlen(buffer));
            return READ;
        }
        else if(exist == 0)
        {   
            bzero(buffer,MAX);
            read_lines(buffer,instruction);
            n = write(newsocket,buffer,strlen(buffer)); // file transfer  
            printf("bytes sent - %d\n",n);
            bzero(buffer,MAX);
            printf("\nRequired lines sent successfully to the client \n");
            return READ;
        }

    }
    else if (!strcmp(command,"insert"))
    {
        printf("server: excecuting insert:\n");
        strcpy(instruction,buffer);
        sscanf(buffer,"/%s %s",command,file_name);
        struct stat buf;
        int exist = stat(file_name,&buf);
        bzero(buffer,MAX);
        if(exist == -1)
        { 
            printf("INVALID : %s doesn't exist\n",file_name);
            snprintf(buffer,200,"INVALID : file doesn't exist\n");
            write(newsocket,buffer,strlen(buffer));
            return INSERT;
        }
        else if(check_edit_permission(newsocket,file_name) == 0)
        {
            printf("INVALID : file access not authorised\n");
            snprintf(buffer,200,"INVALID : file access not authorised\n");
            write(newsocket,buffer,strlen(buffer));
            return INSERT;
        }
        else if(exist == 0)
        {   
            bzero(buffer,MAX);
            insert_lines(buffer,instruction);
            n = write(newsocket,buffer,strlen(buffer)); // file transfer  
            printf("data - %d\n",n);
            bzero(buffer,MAX);
            printf("\nRequired lines inserted successfully in file \n");
            return INSERT;
        }
    }
    else if (!strcmp(command,"delete"))
    {
        printf("server: excecuting delete:\n");
        strcpy(instruction,buffer);
        sscanf(buffer,"/%s %s",command,file_name);
        struct stat buf;
        int exist = stat(file_name,&buf);
        bzero(buffer,MAX);
        if(exist == -1)
        { 
            printf("INVALID : %s doesn't exist\n",file_name);
            snprintf(buffer,200,"INVALID : file doesn't exist\n");
            write(newsocket,buffer,strlen(buffer));
            return DELETE;
        }
        else if(check_edit_permission(newsocket,file_name) == 0)
        {            
            printf("INVALID : file access not authorised\n");
            snprintf(buffer,200,"INVALID : file access not authorised\n");
            write(newsocket,buffer,strlen(buffer));
            return DELETE;
        }
        else if(exist == 0)
        {   
            bzero(buffer,MAX);
            delete_lines(buffer,instruction);
            n = write(newsocket,buffer,strlen(buffer)); // file transfer  
            printf("bytes sent - %d\n",n);
            bzero(buffer,MAX);
            printf("\nRequired lines/file deleted successfully \n");
            return DELETE;
        }
        
    }
    else if (!strcmp(command,"exit"))
    {
        printf("server: excecuting exit:\n");
        sscanf(buffer,"/%s",command);
        user_exit(newsocket);
        return EXIT;
    }
    else if (!strcmp(command,"yes"))
    {
        printf("server: excecuting yes:\n");
        int from_client = add_collaborator(newsocket);
        if(from_client == -1)
        {
            snprintf(buffer,200,"Error in adding as collaborator\n");
            write(newsocket,buffer,strlen(buffer));
            reset_invitation(newsocket);
            return YES;
        }
        bzero(buffer,MAX);
        snprintf(buffer,200,"You are added as a collaborator\n");
        write(newsocket,buffer,strlen(buffer));
        bzero(buffer,MAX);
        snprintf(buffer,200,"client with id %d accepted Your invitation \nEnter Command:\n",newsocket+10000);
        write(from_client,buffer,strlen(buffer));
        reset_invitation(newsocket);
        bzero(buffer,MAX);
        return YES;
    }
    else if (!strcmp(command,"no"))
    {
        printf("server: excecuting no:\n");
        int from_client = reset_invitation(newsocket);
        bzero(buffer,MAX);
        snprintf(buffer,200,"Invitation rejected successfully\n");
        write(newsocket,buffer,strlen(buffer));
        bzero(buffer,MAX);
        snprintf(buffer,200,"client with id %d rejcted Your invitation \nEnter Command:\n",newsocket+10000);
        write(from_client,buffer,strlen(buffer));
        bzero(buffer,MAX);
        return NO;
    }

    return 1;

}

void read_lines(char *buffer,char *input)
{   
    int parse_count,line_count;
    char command[10] = "";
    char file_name[20] = "";
    int start_idx, end_idx,idx;
    char message[256] = "";
    char line[256] = "";
    parse_count = sscanf(input,"/%s %s %d %d",command,file_name,&start_idx,&end_idx);
    
    FILE *filePointer = fopen(file_name, "r") ;
	if ( filePointer == NULL ) // check for successful file opening
    {
        printf( "\n%sfile failed to open in READ module.\n",file_name ) ;
		sprintf(buffer, "Fail: %s file failed to open in READ module.",file_name);	
		//write(client_id, message_sent, sizeof(message_sent)); 
        return;
    }
    else 
     {  printf( "\n%sfile opened in READ module.\n",file_name ) ;
        line_count = 0;
    
        while(fgets(line,256,filePointer)!= NULL) // counting the no of lines 
            line_count++;

        printf("\nNo of lines in file : %d \n", line_count);
        rewind(filePointer);

        if(parse_count == 4) // for start and end index
        {
            if( ((start_idx >= (-1 * line_count ) ) && (start_idx < line_count))&&((end_idx >= (-1 * line_count ) ) && (end_idx < line_count))) // line index valid range --> [-N,N)
            {    
                printf("\nCorrect index  in 4\n");
                int temp ;
                if(start_idx < 0) // if indexing is reversed and negative
                {
                    start_idx = start_idx + line_count;
                    end_idx = end_idx + line_count;
                }

                FILE *tmp = tmpfile();// temp file to copy the contents 
                for(int i = 0; i <= end_idx ; i++) // copying required line to a temp file
                {
                    fgets(line,256,filePointer);
                    if( i>=start_idx && i<= end_idx)            
                        fprintf(tmp,"%s",line);
                }
                rewind(tmp);

                int count = fread(buffer, sizeof(char), MAX,tmp);
                printf("\n count - %d",count);
                fclose(tmp);
            }
            else
            {
                sprintf(buffer,"Fail: Either index not in correct in range READ module-4.");
                printf("Fail: Either index not in correct in range READ module-4.");
            }
            fclose(filePointer);
            return;
        }

        if(parse_count == 3) // for only start index
        {
            printf("\nCorrect index in 3\n");
            if( (start_idx >= (-1 * line_count ) ) && (start_idx < line_count)) // line index valid range --> [-N,N)
            {    
                printf("correct index \n");
                int temp ;
                if(start_idx < 0) // if indexing is reversed and negative
                    temp = start_idx + line_count; 
                else
                    temp= start_idx; // if indexing is forward and positive

                for(int i = 0; i <= temp ; i++)
                fgets(line,256,filePointer);// getting the required line 
                strcpy(buffer,line);
                //write(client_id, message_sent, sizeof(message_sent));
                
            }
            else
            {
                sprintf(buffer,"Fail: Index not in correct range READ module.-3");
                printf("Fail: Index not in correct range READ module.-3");
            }
            fclose(filePointer);
            return;
        }

        if(parse_count == 2) // for entire file content
        {   bzero(buffer,MAX);
            printf("\n No index specified for Read \n");
            int count = fread(buffer, sizeof(char), MAX, filePointer);
            buffer[count] = '\n';
            printf("\n count - %d",count+1);
            fclose(filePointer);
            return;
        }

        fclose(filePointer);
    }

}
void insert_lines(char *buffer,char *input)
{   
    int parse_count,line_count;
    char command[10] = "";
    char file_name[20] = "";
    int start_idx;
    char message[512] = "";
    char message1[512] = "";
    char line[256] = "";
    parse_count = sscanf(input,"/%s %s %d \"%[^\"]\"",command,file_name,&start_idx,message);


    FILE *filePointer = fopen(file_name, "r+") ;
	if ( filePointer == NULL ) // check for successful file opening
    {
        printf( "\n%s file failed to open in INSERT module.\n",file_name ) ;
		sprintf(buffer, "Fail: %s file failed to open in INSERT module.",file_name);	
        return;
    }
    else 
     {  printf( "\n%sfile opened in INSERT module.\n",file_name ) ;
        line_count = 0;

        while(fgets(line,256,filePointer)!= NULL) // counting the no of lines 
            line_count++;

        printf("\nNo of lines in file : %d \n", line_count);
        rewind(filePointer); //fclose(filePointer);

        FILE *tmp1 = tmpfile();
        FILE *tmp2 = tmpfile();

        for ( int j = 0 ; j < line_count; j++) // creating the temp file for insertion operations
        {
            fgets(line, sizeof(line), filePointer);
            fprintf(tmp1,"%s",line);
            bzero(line,256);
        }                
        rewind(filePointer);
        rewind(tmp1);

        if(parse_count == 4)
        {
            conver_to_multiple_lines(message, message1);
            if(line_count ==0)
            {    printf("\n**No of lines in file is zero so putting at start**\n##Irrespective of the index given##\n");
                fputs(message1,filePointer);
                fclose(filePointer);

                filePointer = fopen(file_name, "r");
                bzero(buffer,MAX);
                int count = fread(buffer, sizeof(char), MAX, filePointer);
                
                printf("buffer count %d\n", count);
                printf("\n%s : file updated in INSERT module.-4 \n",file_name);
                fclose(filePointer);
                fclose(tmp1);
                return;
                
            }
            
            if( (start_idx >= (-1 * line_count ) ) && (start_idx < line_count))
            { 
                printf("Correct index in 4 \n");
                
                if(start_idx < 0)
                    start_idx = start_idx + line_count;
                
                for(int i=1;i<=(line_count+1);i++)
                {
                    if( i == start_idx+1)
                    {              
                        fputs(message1,filePointer);
                        fputs ( "\n", filePointer) ;
                        //fprintf()
                    }
                    else
                    {
                        fgets(line,sizeof(line),tmp1);  
                        fprintf(filePointer,"%s",line);                  
                    }

                }   
                rewind(filePointer);
                fclose(filePointer);

                filePointer = fopen(file_name, "r");
                bzero(buffer,MAX);
                int count = fread(buffer, sizeof(char), MAX, filePointer);
                
                printf("buffer count %d\n", count);
                printf("\n%s : file updated in INSERT module.-4 \n",file_name);
                
            }
            else
            {
            sprintf(buffer, "\nINVALID : Index out of range in INSERT module. -2\n");

            }
            fclose(filePointer);
            fclose(tmp1);
            return;
        }
        


        fclose(filePointer);
        filePointer = fopen(file_name, "w");

        if(parse_count == 2) // for inserting at the end of the file 
        {
            printf("\n No index \n");
            parse_count = sscanf(input,"/%s %s \"%[^\"]\"",command,file_name,message);
            conver_to_multiple_lines(message, message1);
            
              if(line_count ==0)
            {   printf("\nNo of lines in file is zero so putting at start\n");
                fputs(message1,filePointer);
                fclose(filePointer);

                filePointer = fopen(file_name, "r");
                bzero(buffer,MAX);
                int count = fread(buffer, sizeof(char), MAX, filePointer);
                
                printf("buffer count %d\n", count);
                printf("\n%s : file updated in INSERT module.-4 \n",file_name);
                fclose(filePointer);
                fclose(tmp1);
                return;
                
            }

            int i = 0;
            for(i=1;i<=(line_count+1);i++)
            {
                    
                if( i == line_count+1)
                {   
                    
                    fputs ( "\n", filePointer ) ;
                    fputs(message1,filePointer);                         

                }
                else
                {
                    fgets(line,sizeof(line),tmp1);  
                    fprintf(filePointer,"%s",line);                                  

                }

            }  
            //sprintf(buffer, "\n%s file updated in INSERT module. -2\n",file_name);
            rewind(filePointer);
            fclose(filePointer);
            filePointer = fopen(file_name, "r");
            bzero(buffer,MAX);
            int count = fread(buffer, sizeof(char), MAX, filePointer);
            printf("buffer count %d\n", count);
            //buffer[count] = '\n';
            printf("\n%s file updated in INSERT module. -2\n",file_name);
            
            fclose(tmp1);
            fclose(filePointer);
            return;
        }

        fclose(tmp1);
        fclose(filePointer);

    }
    

}

void conver_to_multiple_lines(char *txt, char *amended_text)
{
    printf("\noriginal text :%s \n", txt);
    
    int N = strlen(txt);
    int i = 0;
    FILE *file = tmpfile();
    while (i < N)
    {

        if (txt[i] == 92)
        {
            if (i == N - 1)
            {
                fputc(txt[i], file);
                i++;
            }
            else
            {
                if (txt[i + 1] == 'n')
                {
                    fputc('\n', file);
                    i = i + 2;
                }
                else
                {
                    fputc(txt[i], file);
                    i++;
                }
            }
        }
        else
        {
            fputc(txt[i], file);
            i++;
        }
    }
    rewind(file);
    int count = fread(amended_text, sizeof(char), MAX, file);
    printf("\namended text :%s \n", amended_text);
}


void delete_lines(char *buffer,char *input)
{   
    int parse_count,line_count;
    char command[10] = "";
    char file_name[20] = "";
    int start_idx, end_idx,idx;
    char message[256] = "";
    char line[256] = "";
    parse_count = sscanf(input,"/%s %s %d %d",command,file_name,&start_idx,&end_idx);
    
    FILE *filePointer = fopen(file_name, "r") ;
	if ( filePointer == NULL ) // check for successful file opening
    {
        printf( "\n%sfile failed to open in DELETE module.\n",file_name ) ;
		sprintf(buffer, "Fail: %s file failed to open in DELETE module.",file_name);	
        return;
    }
    else 
     {  printf( "\n%sfile opened in delete module.\n",file_name ) ;
        line_count = 0;
    
        while(fgets(line,256,filePointer)!= NULL) // counting the no of lines 
            line_count++;

        printf("\nNo of lines in file : %d \n", line_count);
        rewind(filePointer);
        if(line_count == 0)
        {
            bzero(buffer,MAX);
            sprintf(buffer, "\n%s : Line count is already zero.\n",file_name);
            int count = fread(buffer, sizeof(char), MAX, filePointer);
            printf("buffer count %d\n", count);
            printf("line count is zero !!! \n");
            fclose(filePointer);
            return;

        }

        FILE *tmp1 = tmpfile();
        FILE *tmp2 = tmpfile();
        for ( int j = 0 ; j < line_count; j++) // creating the temp file for deletion operations
        {
            fgets(line, sizeof(line), filePointer);
            fprintf(tmp1,"%s",line);
            bzero(line,256);
        }                
        rewind(filePointer);
        rewind(tmp1);
        fclose(filePointer);

        

        if(parse_count == 4) // for both index present
        {
            if( ((start_idx >= (-1 * line_count ) ) && (start_idx < line_count))&&((end_idx >= (-1 * line_count ) ) && (end_idx < line_count))) // line index valid range --> [-N,N)
            {   filePointer = fopen(file_name, "w") ; 
                printf("\nCorrect index  in 4\n");
                int temp ;
                if(start_idx < 0) // if indexing is reversed and negative
                {
                    start_idx = start_idx + line_count;
                    end_idx = end_idx + line_count;
                }
                
                for(int i=0;i<(line_count);i++)
                {
                    if( i>= start_idx && i<= end_idx)
                    {              
                        fgets(line,sizeof(line),tmp1);  
                        puts(line);
                    }
                    else
                    {
                        fgets(line,sizeof(line),tmp1);  
                        fprintf(filePointer,"%s",line); 
                        bzero(line,256);                 
                    }
                }
                //sprintf(buffer, "\n%s : line no %d to %d deleted in Delete module. -4\n",file_name,start_idx,end_idx);
                
                fclose(filePointer);
                filePointer = fopen(file_name, "r");
                bzero(buffer,MAX);
                int count = fread(buffer, sizeof(char), MAX, filePointer);
                printf("buffer count %d\n", count);
                printf("\nfile updated in Delete module. -4\n");
                
            }
            else
            {
                sprintf(buffer,"Fail: Either index is not in correct range in Delete module-4.");
            }
            fclose(filePointer);
            fclose(tmp1);

            return;
        }

       
         if (parse_count == 3)
        {
            if( start_idx >= (-1 * line_count )  && (start_idx < line_count) ) //line index valid range --> [-N,N)
            {    
                filePointer = fopen(file_name, "w") ; 
                printf("\nCorrect index  in 3\n");
                int temp ;
                if(start_idx < 0) // if indexing is reversed and negative
                    start_idx = start_idx + line_count;
                printf("\nDeleted line : \n");
                for(int i=0;i<(line_count);i++)
                {
                    if( i == start_idx)
                    {              
                        fgets(line,sizeof(line),tmp1);  
                        puts(line);  
                    }
                    else
                    {
                        fgets(line,sizeof(line),tmp1);  
                        fprintf(filePointer,"%s",line);                  
                    }

                }
               // sprintf(buffer, "\n%s : line no %d deleted in Delete module. -3\n",file_name,start_idx);
                fclose(filePointer);
                filePointer = fopen(file_name, "r");
                bzero(buffer,MAX);
                int count = fread(buffer, sizeof(char), MAX, filePointer);
                printf("buffer count %d\n", count);
                printf("\nfile updated in Delete module. -3\n");
            }
            else
            {
                sprintf(buffer,"Fail: Index not in correct range in Delete module-3.");
            }
            fclose(filePointer);
            fclose(tmp1);

            return;
        }

         if (parse_count == 2)
        {
            printf("\nNo index specified in delete - deleting lines\n");
            filePointer = fopen(file_name, "r") ; 
            bzero(buffer,MAX);
            int count = fread(buffer, sizeof(char), MAX, filePointer);
            printf("buffer count %d\n", count);
            fclose(filePointer);
            //sprintf(buffer, "\n%s : All lines deleted in Delete module. -2\n",file_name);
            filePointer = fopen(file_name, "w") ; 
            fclose(filePointer);
            return;
        }
        
        fclose(filePointer);
        fclose(tmp1);
        return;

     }


}

int add_collaborator(int invited)
{
    int inviter ;
    int i=0;
    for(i = 0 ; i < 10 ;i++)
    {
        if(invitation[i].to == invited)
            break ;
    }
    if(i > 9)
    {
        return -1;
    }
    int j = 1;
    for(j = 1; j<100 ; j++)
    {
        if(file_permission[j].flag == 1 && file_permission[j].client_id[0] == invitation[i].from && strcmp(file_permission[j].file_name,invitation[i].file_name) == 0)
        {
            break ;
        }
        
    }
    if(j >= 99)
    {
        return -1;
    }
    int k;
    for( k = 1 ; k<5 ; k++)
    {
        if(file_permission[j].client_id[k] == 0 || file_permission[j].permission[k] == '\0')
        {
            file_permission[j].client_id[k] = invited;
            file_permission[j].permission[k] = invitation[i].permission;
            break;
        }
    }
    if(k > 4)
    {
        return -1;
    }
  
    inviter = invitation[i].from ;
    return inviter;
    
}


int FileTransfer(char *FileName, char *buffer)
{
   FILE *fp = fopen(FileName,"r+");
   if(fp == NULL)
   {
     printf("Failed to open the file \"%s\"\n",FileName);
     return -1;
   } 
   bzero(buffer,MAX);
   int count = fread(buffer, sizeof(char), MAX, fp);
   if(count == MAX && getc(fp) != EOF)
   {
    printf("Size of the file  Exceededs the size of the buffer, failed to transfer%d:\n",count);
    bzero(buffer,MAX);
    fclose(fp);
    return -1;
   }
   if(count == 0)
   {
    printf("Requested file is blank\n");
    return -1;
   }

   fclose(fp);
   return 0;
}


int main()
{
    int opt = TRUE;
	int addrlen , new_socket ,  valread ;
    int total_client = 0;
   struct sockaddr_in address;
   
   fd_set readfds; // fd_set is a structure contains two things 1. fd count 2. an array of unsigned integer which represent the ids of fd set

   char buffer[MAX] = "SERVER IS READY TO PROCESS REQUEST: \r\n";

   int client_socket[MAX_CLIENTS] ; // array to store socket ids of clients in case connected any.
   //intitalise all clients to 0 which is not a valid socket id and tells no client is connected.
   for (int i = 0; i < MAX_CLIENTS; i++)
	{
		client_socket[i] = 0;
	}
   
   //create a server socket
   // All the connection request from several clients will be reflected here.
    int server_socket =0;
	if( (server_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		printf("\nSocket could not be opened \n\n");
		exit(EXIT_FAILURE);
	}
    else
    { 
	    printf("\nServer master Socket open success with id : %d\n\n", server_socket);
    }

    //set server socket to allow multiple connections ,
	// this helps server to get the same port 
	if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

    //Initialising server socket adress faimily
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

    //bind the socket to localhost port 5000
	if (bind(server_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		printf("Failed to bind\n\n");
		exit(EXIT_FAILURE);
	}
    else
    printf("Server socket binding successful \n\n"); 

	printf("Listener on port %d \n", PORT);

    //try to specify maximum of 5 backlogs for the server_socket
    int listen_id = listen(server_socket, 5);
	if (listen_id < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
    else
    printf("Listening to the local port.... \n\n");

    //accept the incoming connection
	addrlen = sizeof(address);

    while(1)
    {
        //reset fd_set
        FD_ZERO(&readfds);
        //putting server  socket to set
		FD_SET(server_socket, &readfds);
		int sd , max_sd = server_socket;

        // we need to put client  socket descriptor back to fds, because each time select is call as it reset all fds

        for (int i = 0 ; i < MAX_CLIENTS ; i++)
		{
			//socket descriptor
			sd = client_socket[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}

        //wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
         printf("\nWaiting for new request\n");

        int activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf(" Error in select");
		}
        //If read from server_socket  returns some positive value ,
		//then its a connection request from new client.
        if (FD_ISSET(server_socket, &readfds))
        {
            new_socket = accept(server_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen);
            if (new_socket < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
            total_client++;
         

          
			if (total_client > MAX_CLIENTS)
            {
				char quit_msg[] = "Max clients connected, try again later\n";
				write(new_socket, quit_msg, sizeof(quit_msg));
				close(new_socket);
                total_client = MAX_CLIENTS;
				continue;

			}
            else
            {
			    printf("New connection request from client , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                char welcome[] = "GREETINGS, server is ready to serve your operations\nEnter Command :";
				write(new_socket, welcome, sizeof(welcome));
			}

            puts("\nSERVER STATUS IS SENT TO THE CLIENT :\n");

            //add new socket to array of sockets
			for (int i = 0; i <MAX_CLIENTS; i++)
			{
				//if position is empty
				if( client_socket[i] == 0 )
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as no :%d\n" , i+1);
						
					break;
				}
			}
            //Assign client id and save socket number
			printf("Assigning random client id\nCurrent Active Client summery:\n");
			// new_client_id(client_id, new_socket);
			new_client_id(new_socket);

			for (int i = 0; i < 5; i++)
            {
				printf("socket fd/id : %d\t random id : %d\n", client_id[i][0], client_id[i][1]);
			}

        }
        else
        {
            // else it means there is some processing request from some clients
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                sd = client_socket[i];

                if (FD_ISSET( sd , &readfds))
                {
                    //if any of client terminates abdruptly.
                    // closing the connection with the client reseting the corresponding fd.
                    bzero(buffer,MAX);
                    if ((valread = read( sd , buffer, 1024)) == 0)
				    {
					    //Somebody disconnected , get his details and print
					    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
					    printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					    //Close the socket and mark as 0 in list for reuse
                        user_exit(sd);
					    client_socket[i] = 0;
                        total_client--;
                        close( sd );
				    }
                    else
                    {
                        //if(ExcecuteCommand(buffer,sd) == _EXIT )
                        if(execute_command(buffer,sd)== EXIT)
                        {
                            total_client= total_client-1;
                            close(sd);
                            client_socket[i] = 0;
                        }
                    }
                }
            }
        }

    }

    return 0;
}

