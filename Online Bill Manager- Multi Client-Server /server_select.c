// Server side C program
// 21CS60D04 Major Kunal Pilley
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>
#include<netdb.h>
#include<string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include<signal.h>
#include <errno.h>
#include<stdbool.h>

#define PORT 5099
#define SIZE 100000
#define TRUE 1

char buffer[SIZE];
FILE *filePointerO ;
FILE *filePointerT ;
FILE *file;
FILE *filePointerO1;
FILE *filePointerO2;
FILE *filePointerT1 ;
FILE *filePointerT2;

int client_id;
// for the action on the file 
int choice ;
int fd;
char message_sent[256];
char message_recd[256];

int parse_count;
int line_index;
int flag;


long f_atoi(const char S[]); // function for converting numeric string to integer 
                            // and to find out the if it is an alpha numric string 
void sort(FILE *,int);
int is_sorted(FILE *,int);

void similarity(FILE *,FILE *);
void merge_file(FILE *,FILE *,int);

int is_valid_date(int , int , int );
int is_valid_file(FILE *);

void recieve_file(int,FILE *);
void send_file(int,FILE * );

void append(FILE *, FILE *, FILE *);

char input [150];
char command[11],file1[20],file2[20],file3[20];
char field, catch [5];
struct record
{
    int day;
    int month;
    int year;
    char item[128];
    float price ;
};


int main()
{
	//Initialise the socket
  
    int opt = TRUE;
    int  addrlen , new_socket , client_socket[5] , 
    max_clients = 5 , activity, i , valread , sd;
    int max_sd;  
    int total_client = 0;

    struct sockaddr_in address; // struct sockaddr_in address;
    fd_set readfds; 
    socklen_t addr_size;
         
    //char buffer[SIZE];
	int server_sock_id = socket(AF_INET, SOCK_STREAM, 0);

    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;  
    } 

	// //error check for socket open
	if (server_sock_id < 0)
	{
        printf("\nSocket could not be opened \n\n");
        exit(1);
	}
    else
	    printf("\nServer master Socket open success with id : %d\n\n", server_sock_id);
	
    //set server socket to allow multiple connections ,
	// this helps server to get the same port 
	if( setsockopt(server_sock_id, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

 	//Iniptialise env for sockaddr structure
	struct sockaddr_in server;
    memset(&address, '\0', sizeof(address));
	address.sin_family = AF_INET; // IPv4 family
	address.sin_port = htons(PORT); // host to network , converts short integer to network byte order
	address.sin_addr.s_addr =  INADDR_ANY; // my machine adress will be taken 
	                                    //  if specified, socket is bind to any available local IP address//inet_addr( "127.0.0.1") ;  
	//memset ( &(server.sin_zero), 0, 8 );


	//bind IP address and port number to create a socket
    // bind function binds the socket to the address and port number specified in addr
	int server_bind_id;
	server_bind_id = bind( server_sock_id , (struct sockaddr*) &address, sizeof(address) );

	if (server_bind_id < 0)
	{
		printf("Failed to bind\n\n");
        exit(2);
	}
	else
		printf("Server socket  binding successful \n\n"); 
	 

	/*  
        Listen for request fm client,puts the server socket in a passive mode,
        where it waits for the client to approach the server to make a connection
        first argument specifies socket descriptor where information from client will be stored
        Second argument defines the maximum length to which the queue of pending connections may grow

    */

	int listen_id = listen(server_sock_id, 5);

  	 if (listen_id < 0)
       {
		printf("Failed to listen to the \n\n");
        exit(3);
	   }
     else
	 	printf("Listening to the local port.... \n\n");
	 
    addrlen = sizeof(address); 
	//wait for new request and proceed
	struct sockaddr_in client;
	socklen_t client_size;
	client_size = sizeof(client);
	//sd = accept(server_sock_id, (struct sockaddr*)&client, &client_size );

    int total_connections = 0;

     while(1)
    {
        //clear the socket set 
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(server_sock_id, &readfds);  
        max_sd = server_sock_id; 

        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
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

        printf("Waiting before select\n");
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}

        if (FD_ISSET(server_sock_id, &readfds))  
        {    printf("INSIDE THIS \n");
            new_socket = accept(server_sock_id, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            if (new_socket<0)  
            {  
                perror("accept");  
                exit(1);  
            }  
            total_client++;
            //inform user of socket number - used in send and receive commands 
            if(total_client <= max_clients)
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  

            //printf("New socket %d\n", new_socket);
			printf("Connection from client est\n");

            total_connections++;
			if (total_connections > 5)
            {
				char quit_msg[] = "Max clients connected, try again later\n";
				write(new_socket, quit_msg, sizeof(quit_msg));
				close(new_socket);
				continue;

			}
            else
            {
				char welcome[] = "GREETINGS, server is ready to serve your operations\n";
				write(new_socket, welcome, sizeof(welcome));
			}
           
            for (int i = 0; i < max_clients; i++) 
            {
                    
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);
                        
                    break;
                }
                
            }

		}
        else
        {
                //else its some IO operation on some other socket :)
			for (int i = 0; i < max_clients; i++) 
			{
				sd = client_socket[i];
				
                if (FD_ISSET( sd , &readfds)) 
                {
                    if ((valread = read( sd ,message_recd, 256)) == 0)
				    {
					    //Somebody disconnected , get his details and print
					    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
					    printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					    //Close the socket and mark as 0 in list for reuse
					    close( sd );
					    client_socket[i] = 0;
                        total_client--;
				    }
                    else
                    {
                             // while from single client
                            { 
                                
                                printf( "\nWaiting for command \n\n");
                                fflush(stdin);
                                fflush(stdout);
                                // while (1)
                                // {
                                //     read(sd, message_recd, 200);//<<--

                                //     if(strlen(message_recd)>0)
                                //     break;
                                // }
                            

                                printf( "Command from client is :  %s \n\n" , message_recd);
                                parse_count = sscanf(message_recd,"/%s",command);

                                if(!strcmp(command,"sort") ) // valid command with only no other argument
                                {  
                                    sscanf(message_recd,"/%s %s %c",command,file1,&field);
                                    int col  = field;
                                    printf("command : %s \n",command);
                                    printf("command LEN: %lu \n",strlen(command));
                                    printf("filename : %s \n", file1);
                                    printf("command LEN: %lu \n",strlen(file1));
                                    printf("fd : %c \n", field);
                                    strcpy(message_sent,"command recd for sort operation\n\n ");// 3 -->
                                    write(sd, message_sent, sizeof(message_sent));           
                                
                                    FILE *fp = fopen("temp_sort.txt", "w");
                                    recieve_file(sd,fp);
                                    fclose(fp);

                                    fp = fopen("temp_sort.txt", "r");            
                                    
                                    if(!is_valid_file(fp))
                                    {
                                        printf("\ninvalid file terminating the operations\n ");
                                        strcpy(message_sent,"Error");
                                        write(sd, message_sent, sizeof(message_sent));
                                        continue;
                                    }
                                    else 
                                    {
                                        printf("\n valid file \n");
                                        strcpy(message_sent,"Valid");
                                        write(sd, message_sent, sizeof(message_sent));

                                    }
                                    fclose(fp);
                                    


                                    fp = fopen("temp_sort.txt", "r");
                                    printf("value of col **%d/n",col);
                                    sort(fp,col);
                                    fclose(fp);

                                    fp = fopen("sorted.txt", "r");
                                    send_file(sd,fp);
                                    fclose(fp);
                                    printf("sorted File sent successfully to client.\n");
                                    
                                // printf("%s",buffer);
                                    //read(sd, buffer, sizeof(buffer));
                                    //fprintf(fp, "%s", buffer);
                                    //fwrite(buffer,sizeof(char),strlen(buffer),fp);
                                    //fclose(fp);

                                }
                                else if(!strcmp(command,"merge") ) // 
                                {
                                    parse_count = sscanf(message_recd,"/%s %s %s %s %c",command,file1,file2,file3,&field);
                                    int col  = field;

                                    strcpy(message_sent,"Command recd for Merge operation");
                                    write(sd, message_sent, sizeof(message_sent));  //-->>
                                    strcpy(message_recd,"");strcpy(message_sent,"");         

                                    FILE *fp1 = fopen("temp_mer1.txt", "w");
                                    recieve_file(sd,fp1); // <<---
                                    fclose(fp1);

                                    fp1 = fopen("temp_mer1.txt", "r");
                                    
                                    if((!is_valid_file(fp1)) )//|| (!is_sorted(fp1,col))
                                    {
                                        printf("\ninvalid file terminating the operations\n ");
                                        strcpy(message_sent,"Error");
                                        write(sd, message_sent, sizeof(message_sent));
                                        continue;
                                    }
                                    fclose(fp1);

                                    FILE *fp11 = fopen("temp_mer1.txt", "r");
                                    if( (!is_sorted(fp11,col)))
                                    {   
                                        printf("\n file 1 not sorted terminating the operations\n ");
                                        strcpy(message_sent,"Sort");
                                        write(sd, message_sent, sizeof(message_sent));
                                        continue;

                                    }
                                    else
                                    {
                                        printf("\n file 1 valid file \n");
                                        strcpy(message_sent,"Valid");
                                        write(sd, message_sent, sizeof(message_sent));

                                    }
                                
                                    fclose(fp11);


                                    FILE *fp2 = fopen("temp_mer2.txt", "w");
                                    recieve_file(sd,fp2);//  <<---
                                    fclose(fp2);
                                    
                                    fp2 = fopen("temp_mer2.txt", "r");
                                    if((!is_valid_file(fp2)) ) // || (!is_sorted(fp2,col))
                                    {
                                        printf("\nInvalid file 2 terminating the operations\n ");
                                        strcpy(message_sent,"Error");
                                        write(sd, message_sent, sizeof(message_sent));
                                        continue;
                                    }
                                    fclose(fp2);

                                    FILE *fp22 = fopen("temp_mer2.txt", "r");
                                    if( (!is_sorted(fp22,col)))
                                    {   
                                        printf("\n file 2 not sorted terminating the operations\n ");
                                        strcpy(message_sent,"Sort");
                                        write(sd, message_sent, sizeof(message_sent));
                                        continue;

                                    }
                                    else 
                                    {
                                        printf("\n file 2 valid file \n");
                                        strcpy(message_sent,"Valid");
                                        write(sd, message_sent, sizeof(message_sent));

                                    }
                                    

                                    printf("\n two temp file opened\n ");
                                
                                    fp1 = fopen("temp_mer1.txt", "r");
                                    printf("\n1\n");
                                    fp2 = fopen("temp_mer2.txt", "r");
                                    printf("\n2\n");

                                    merge_file(fp1,fp2,col);
                                    fclose(fp1);fclose(fp2);
                                    read(sd, message_recd, sizeof(message_recd));//<<--
                                    printf("\n-%s-\n",message_recd);

                                    FILE *fp = fopen("sorted.txt","r");
                                    send_file(sd,fp); // ---->>
                                    fclose(fp);
                                    printf("Merged File sent successfully.\n");

                                }
                                else if(!strcmp(command,"similarity")) // valid command for INSERTX
                                {	
                                    parse_count = sscanf(message_recd,"/%s %s %s",command,file1,file2);
                                
                                    strcpy(message_sent,"command recd for similarity operation");
                                    write(sd, message_sent, sizeof(message_sent));  // -->>
                                    strcpy(message_recd,"");strcpy(message_sent,"");         

                                    FILE *fp1 = fopen("temp_sim1.txt", "w");
                                    recieve_file(sd,fp1); // <---
                                    fclose(fp1);

                                    fp1 = fopen("temp_sim1.txt", "r");
                                    if(!is_valid_file(fp1))
                                    {
                                        printf("\ninvalid file terminating the operations\n ");
                                        strcpy(message_sent,"Error");
                                        write(sd, message_sent, sizeof(message_sent));
                                        continue;
                                    }
                                    else 
                                    {
                                        printf("\n file 1 valid file \n");
                                        strcpy(message_sent,"Valid");
                                        write(sd, message_sent, sizeof(message_sent));

                                    }
                                    fclose(fp1);

                                    FILE *fp2 = fopen("temp_sim2.txt", "w");
                                    recieve_file(sd,fp2); // <---
                                    fclose(fp2);
                                    fp2 = fopen("temp_sim2.txt", "r");
                                    if(!is_valid_file(fp2))
                                    {
                                        printf("\ninvalid file terminating the operations\n ");
                                        strcpy(message_sent,"Error");
                                        write(sd, message_sent, sizeof(message_sent));
                                        continue;
                                    }
                                    else 
                                    {
                                        printf("\n file 2 valid file \n");
                                        strcpy(message_sent,"Valid");
                                        write(sd, message_sent, sizeof(message_sent));
                                    }
                                    fclose(fp2);

                                    fp1 = fopen("temp_sim1.txt", "r");
                                    fp2 = fopen("temp_sim2.txt", "r");

                                    similarity(fp1,fp2);
                                    fclose(fp1);fclose(fp2);
                                    
                                    read(sd, message_recd, sizeof(message_recd));//<<--
                                    printf("-%s-",message_recd);

                                    FILE *fp = fopen("similarity.txt","r");
                                    send_file(sd,fp);  // ---->>
                                    fclose(fp);
                                    printf("similarity File sent successfully.\n");

                                }

                                bzero(message_recd,256);bzero(message_sent,256);
                                bzero(command,11);

                            }

                    
                    
                    
                    
                    }
                                 


                }
            }    
        }   
    } 
    printf(" \n\n ================== end of  main ==================\n\n");
    close(server_sock_id);
    return 0;
}

// to recieve a file from the client 
void recieve_file(int sockfd,FILE *fp)
{
  int n,k ;
  char buffer[SIZE];
  bzero(buffer, SIZE);
  read(sockfd, buffer, sizeof(buffer));
  fwrite(buffer,sizeof(char),strlen(buffer),fp);
  printf("%s",buffer);
  bzero(buffer, SIZE);
  printf("\n file recd successfully \n ");
  fclose(fp);

  /*char data[200];
  while (1) 
  {
   //n = recv(sockfd, buffer, sizeof(buffer), 0);
    n = read(sockfd, buffer, sizeof(buffer));
    if (n <= 0){
        
        printf(" problem encountered %d --\n\n",n);
      break;
     
    }
    
    fprintf(fp, "%s", buffer);
   // k = fputs(buffer,fp);
   printf("--%s\n",buffer);
    
  }//return;
  */
}

void send_file(int socket_fd,FILE *fp )
{   
    int n;
    char data[SIZE] = {0};
   // int count = fread(buffer, sizeof(char), strlen(buffer), fp);
	fscanf(fp,"%[^EOF]",data);
	
	write(socket_fd, data, sizeof(data)); 
    printf("++++++++++++++++\n\n");
    printf("%s",data);
    printf("flen : %lu \n\n",strlen(data));
    printf("file sent : %s \n\n",file1);
    bzero(data, SIZE);
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
// function to sort file
void sort(FILE *fp,int col)
{   
    char line[256];
    int line_count = 0;

    printf("value of col **%d/n",col);

    // count the no of the records to declare a structure
    
    while(fgets(line,256,fp)!= NULL) // counting the no of lines ,for further operations 
        line_count++;

    strcpy(line,""); 
    printf("line count for sort function is : %d -\n\n", line_count); 
    rewind(fp);


    struct record rec[line_count];
    int i = 0;
   // printf("printing the recieved file\n\n"); 

    // coppying record in the structure and printing 

     while(fgets(line, sizeof(line), fp) != NULL) 
    {   
		sscanf(line,"%d.%d.%d %[^\t] %f",&rec[i].day,&rec[i].month,&rec[i].year,rec[i].item,&rec[i].price);
        
        bzero(line, 256);

       // printf("%d record : %d.%d.%d\t%s\t%f\n",i+1,rec[i].day,rec[i].month,rec[i].year,rec[i].item,rec[i].price);
       
        i++;
    }
    rewind(fp);
    fclose(fp);

    
    struct record temp;
    // sort function implementation for date
    if( col == 68)
    {  printf("sorting on D\n");
        for(i=0;i<line_count;i++)
        {
            for(int j=i+1;j<line_count;j++)
            {
              flag = 0;

                if(rec[i].year>rec[j].year)
                {
                    flag =1;
                }
                else if(rec[i].year == rec[j].year)
                { 
                    if(rec[i].month>rec[j].month)
                    {
                        flag = 1;
                    }
                    else if(rec[i].month == rec[j].month)
                    {
                        if(rec[i].day>rec[j].day)
                        {
                            flag = 1;
                        }

                    }
                }           

            

                if(flag)
                {
                    temp = rec[i];
                    rec[i] = rec[j];
                    rec[j] = temp;
                    
                }
            }    
        }
    }

    // sort functionality implementation for Item Name
    if( col == 78)
    {  printf("sorting on N\n");
        for(i=0;i<line_count;i++)
        {
            for(int j=i+1;j<line_count;j++)
            {
                if(strcmp(rec[i].item,rec[j].item)>0)
                {
                    temp = rec[i];
                    rec[i] = rec[j];
                    rec[j] = temp;
                    
                }
            }
        }
    }

    // sort functionality implementation for Price
    if( col == 80)
    {   printf("sorting on P\n");
        for(i=0;i<line_count;i++)
        {
            for(int j=i+1;j<line_count;j++)
            {
                if(rec[i].price > rec[j].price)
                {
                    temp = rec[i];
                    rec[i] = rec[j];
                    rec[j] = temp;
                    
                }
            }
        }
    }
    
    printf("\n\n====================== sorted list ============================\n\n");
    printf("\n\n===============================================================\n\n");
    FILE *fp1 = fopen("sorted.txt","w");

    for( i = 0; i<line_count;i++)
    {
     printf("%d record : %d.%d.%d\t%s\t%f\n",i,rec[i].day,rec[i].month,rec[i].year,rec[i].item,rec[i].price);
     sprintf(line, "%d.%d.%d\t%s\t%f\n",rec[i].day,rec[i].month,rec[i].year,rec[i].item,rec[i].price);
     fprintf(fp1,"%s",line);
     strcpy(line,"");
    }

    fclose(fp1);// fclose(filePointerT);strcpy(line,"");
    printf("\n\n================================================================\n\n");

}


int is_valid_date(int day, int mon, int year)    
{
    int is_valid = 1, is_leap = 0;

    if (year >= 1800 && year <= 9999) 
    {

        //  check whether year is a leap year
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
        {
            is_leap = 1;
        }

        // check whether mon is between 1 and 12
        if(mon >= 1 && mon <= 12)
        {
            // check for days in feb
            if (mon == 2)
            {
                if (is_leap && day == 29) 
                {
                    is_valid = 1;
                }
                else if(day > 28) 
                {
                    is_valid = 0;
                }
            }

            // check for days in April, June, September and November
            else if (mon == 4 || mon == 6 || mon == 9 || mon == 11) 
            {
                if (day > 30)
                {
                    is_valid = 0;
                }
            }

            // check for days in rest of the months 
            // i.e Jan, Mar, May, July, Aug, Oct, Dec
            else if(day > 31)
            {            
                is_valid = 0;
            }        
        }

        else
        {
            is_valid = 0;
        }

    }
    else
    {
        is_valid = 0;
    }

    return is_valid;

}

int is_valid_file(FILE *fp)
{   
    int parse_count,valid_date = 1;
    int line_count = 0 ;
    char line[256]; char *ch;char *s;
    int i = 0;

    while(fgets(line,256,fp)!= NULL) // counting the no of lines ,for further operations 
       line_count++;
    
    strcpy(line,""); 
    printf("line count is : %d \n\n", line_count); 
    rewind(fp);

    struct record rec[line_count];
    while(fgets(line, sizeof(line), fp) != NULL) 
    { 
		parse_count = sscanf(line,"%d.%d.%d %[^\t] %f",&rec[i].day,&rec[i].month,&rec[i].year,rec[i].item,&rec[i].price);
        //printf("%d record : %d.%d.%d\t%s\t%f\n",i+1,rec[i].day,rec[i].month,rec[i].year,rec[i].item,rec[i].price);
        
        if(parse_count!=5)
        {
            printf("invalid data in the data set at row = %d, and parse count = %d\n",i+1,parse_count);
            return 0; 
        }

        valid_date = is_valid_date(rec[i].day,rec[i].month,rec[i].year);
        if(!valid_date)
        {
            printf("invalid date in the data set at row = %d",i+1);
            return 0;
        }
        
		/*parse_count = sscanf(line,"%d.%d.%d %[^\t] %s",&rec[i].day,&rec[i].month,&rec[i].year,rec[i].item,ch);
         s =  strchr(".",ch);

         if(strlen(s)>3)
         { 
             printf(" more than two digit after decimal  in the data set at row = %d",i+1);
             return 0;
         }
         */

        bzero(line, 256);
        i++;
    }

    return 1;

}
void merge_file(FILE *filePointerO1,FILE *filePointerO2, int col)
{

    printf("inside merge %c-\n",field);
    char line1[256],line2[256], final_line[512];
    
    int line_count1 = 0;
    int line_count2 = 0;
    struct record temp;
    filePointerT = fopen("temp_for_append.txt","w");
   
    
        printf("\ninside of is merge function \n");

        //if( is_sorted(filePointerO1,col) && is_sorted(filePointerO2,col))
        
            rewind(filePointerO1);
            rewind(filePointerO2);
            
            while(fgets(line1, sizeof(line1), filePointerO1) != NULL) 
            {
                fprintf(filePointerT,"%s",line1);
            }
            rewind(filePointerO1);

            while(fgets(line2, sizeof(line2), filePointerO2) != NULL) 
            {
                fprintf(filePointerT,"%s",line2);
            }
            rewind(filePointerO1);

            fclose( filePointerO1);
            fclose( filePointerO2);
            fclose( filePointerT);

            filePointerT = fopen("temp_for_append.txt","r");
            sort(filePointerT,col);
            fclose(filePointerT);
        


    

}

void similarity(FILE *filePointerO1,FILE *filePointerO2)
{   
    printf("inside similarity");
    
    char line1[256],line2[256], final_line[512];
    int line_count1 = 0;
    int line_count2 = 0;
    struct record temp;

    while(fgets(line1,256,filePointerO1)!= NULL) // counting the no of lines ,for further operations 
        line_count1++;
    strcpy(line1,""); 
    printf("\nline count-1 is : %d \n\n", line_count1); 
    rewind(filePointerO1);

    rewind(filePointerO2);
    while(fgets(line2,256,filePointerO2)!= NULL) // counting the no of lines ,for further operations 
        line_count2++;
    strcpy(line2,""); 
    printf("\nline count-2 is : %d \n\n", line_count2); 
    rewind(filePointerO2);

    /*if(!is_valid_file(filePointerO1))
        {
            printf("invalid data in file terminating the operations ");
            return;
        }
    rewind(filePointerO1);
    //printf("\ninside similarity2");
    if(!is_valid_file(filePointerO2))
        {
            printf("invalid data in  file terminating the operations ");
            return;
        }
    rewind(filePointerO2);
    printf("\ninside similarity2\n");*/

    // count the no of the records to declare a structure

    struct record rec1[line_count1];
    struct record rec2[line_count2];

    int i = 0;

     while(fgets(line1, sizeof(line1), filePointerO1) != NULL) 
    {
		sscanf(line1,"%d.%d.%d %[^\t] %f",&rec1[i].day,&rec1[i].month,&rec1[i].year,rec1[i].item,&rec1[i].price);
        bzero(line1, 256);
       // printf("%d record : %d.%d.%d\t%s\t%f\n",i+1,rec1[i].day,rec1[i].month,rec1[i].year,rec1[i].item,rec1[i].price);
        i++;
    }
    i =0; 
    rewind(filePointerO1);
     while(fgets(line2, sizeof(line2), filePointerO2) != NULL) 
    {
		sscanf(line2,"%d.%d.%d %[^\t] %f",&rec2[i].day,&rec2[i].month,&rec2[i].year,rec2[i].item,&rec2[i].price);
        bzero(line2, 256);
       // printf("%d record : %d.%d.%d\t%s\t%f\n",i+1,rec2[i].day,rec2[i].month,rec2[i].year,rec2[i].item,rec2[i].price);
        i++;
    }
    rewind(filePointerO2);

    filePointerT = fopen("similarity.txt","w");


    for(int i = 0;i<line_count1;i++)
    for(int j = 0; j<line_count2;j++)
    {
        if((rec1[i].day==rec2[j].day && rec1[i].month==rec2[j].month && rec1[i].year==rec2[j].year)|| rec1[i].item==rec2[j].item||rec1[i].price==rec2[j].price)
        {
            sprintf(final_line, "%d.%d.%d\t%50s\t%0.2f\t|\t%d.%d.%d\t%50s\t%0.2f\n", rec1[i].day, rec1[i].month, rec1[i].year,
						rec1[i].item, rec1[i].price, rec2[j].day, rec2[j].month, rec2[j].year,
						rec2[j].item, rec2[j].price );
            fprintf(filePointerT,"%s",final_line);
        }
    }

    fclose(filePointerT);
    fclose(filePointerO1);fclose(filePointerO2);


}

// function to check if the file is sorted .
int is_sorted(FILE *filePointerO,int col)
{
    char line[256];
    int line_count = 0;
   // filePointerO = fopen("sort_price.txt","r");

   

    // count the no of the records to declare a structure
    while(fgets(line,256,filePointerO)!= NULL) // counting the no of lines ,for further operations 
        line_count++;
    strcpy(line,""); 
    printf("line count is : %d \n\n", line_count); 
    rewind(filePointerO);


    struct record rec[line_count];
    int i = 0;

    // coppying record in the structure and printing 

     while(fgets(line, sizeof(line), filePointerO) != NULL) 
    {
        
		sscanf(line,"%d.%d.%d %[^\t] %f",&rec[i].day,&rec[i].month,&rec[i].year,rec[i].item,&rec[i].price);
        bzero(line, 256);
       // printf("%d record : %d.%d.%d\t%s\t%f\n",i+1,rec[i].day,rec[i].month,rec[i].year,rec[i].item,rec[i].price);
        i++;
    }

    struct record temp;

    int flag = 0;

    // on date
    if(col==68)
    {
         for(i=0;i<line_count-1;i++)
        {
            if(rec[i].year>rec[i+1].year)
            {
                flag =1;break;
            }
            else if(rec[i].year == rec[i+1].year)
            { 
                if(rec[i].month>rec[i+1].month)
                {
                    flag = 1;break;
                }
                else if(rec[i].month == rec[i+1].month)
                {
                    if(rec[i].day>rec[i+1].day)
                    {
                        flag = 1;break;
                    }

                }
            }

        }
    }
    // on name 
    if(col == 78)
    {
        for(i=0;i<line_count-1;i++)
        {
            if(strcmp(rec[i].item,rec[i+1].item)>0)
            {
                
                flag = 1;
                break;  
            }
        }

    }
   // on price  
    if(col == 80)
    {
        for(i=0;i<line_count-1;i++)
        {
            if(rec[i].price>rec[i+1].price)
            {
                flag = 1;break;  
            }
        }
    }

    if (flag==0){
    printf("\nfile is  sorted in ascending order\n");
    return 1;}
    else{ 
    printf("\nfile is not sorted in ascending order in line --%d--\n",i+2);
    return 0;}
    
}
