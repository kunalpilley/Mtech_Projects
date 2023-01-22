// Server side C program
// 21CS60D04 Major Kunal Pilley
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

#define PORT 3421

FILE *filePointerO ;
FILE *filePointerT ;
FILE *file;

int client_id;
char message_sent[256];
char message_recd[256];

int parse_count;
char command[20];
char movie_name[200];
int line_index;
int flag;



void f_nlinex(); // function for NLINEX
void f_readx(); // function for READX
void f_insertx(); // function for INSERTX
long f_atoi(const char S[]); // function for converting numeric string to integer 
                            // and to find out the if it is an alpha numric string 


int main()
 {

	//Initialise the socket
	int server_sock_id = socket(AF_INET, SOCK_STREAM, 0);

	// //error check for socket open
	if (server_sock_id < 0)
	{
        printf("\nSocket could not be opened \n\n");
        exit(1);
	}
    else
	    printf("\nSocket open success with id : %d\n\n", server_sock_id);
	


 	//Iniptialise env for sockaddr structure
	struct sockaddr_in server;
	server.sin_family = AF_INET; // IPv4 family
	server.sin_port = htons(PORT); // host to network , converts short integer to network byte order
	server.sin_addr.s_addr =  INADDR_ANY; // my machine adress will be taken 
	                                    //  if specified, socket is bind to any available local IP address//inet_addr( "127.0.0.1") ;  
	memset ( &(server.sin_zero), 0, 8 );


	//bind IP address and port number to create a socket
    // bind function binds the socket to the address and port number specified in addr
	int server_bind_id;
	server_bind_id = bind( server_sock_id , (struct sockaddr*)&server, sizeof(struct sockaddr) );

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
	int listen_id = listen(server_sock_id, 1);

  	 if (listen_id < 0)
       {
		printf("Failed to listen to the \n\n");
        exit(3);
	 }
     else
	 	printf("Listening to the local port.... \n\n");
	 

	//wait for new request and proceed
	struct sockaddr_in client;
	socklen_t client_size;
	client_size = sizeof(client);
	
	
	//int client_id;
	
	client_id = accept(server_sock_id, (struct sockaddr*)&client, &client_size );

	while(1)
	{
        read(client_id, message_recd, sizeof(message_recd) );

        printf( "Message from client is %s \n\n" , message_recd);

        parse_count = sscanf(message_recd,"%s %d %[^\n]",command,&line_index,movie_name);
        // parsing recd message in respective variable 
        //parse count to decide the no of inputs sent and the validity of the same

        /*
        printf("Parse count is : %d\n", parse_count);
        printf("command : %s \n",command);
        printf("command LEN: %lu \n",strlen(command));
        printf("index : %d \n", line_index);
        printf("movie : %s \n", movie_name); 
        */

        if(!strcmp(command,"NLINEX") && parse_count == 1) // valid command with only no other argument
        {  
            f_nlinex();
        }
        else if(!strcmp(command,"READX") && (parse_count>=1 && parse_count<=2)) // VALID COMMAND with only upto one argumemt as index
        {
            f_readx();
        }
        else if(!strcmp(command,"INSERTX")) // valid command for INSERTX
        {	
            f_insertx();
        }
        else
        {
            printf("\nINVALID COMMAND : PLEASE TRY AGAIN \n\n");
            strcpy(message_sent, "INVALID COMMAND : PLEASE TRY AGAIN");
            write(client_id, message_sent, sizeof(message_sent));
            printf("\nback to the client\n\n");
            strcpy(message_recd,"");
            strcpy(message_sent,"");
        }

        printf(" \n\n ====================================\n\n");

	}

    //close(server_sock_id);
	return 0;
}



/*  IMLEMENTATION OF NLINEX functionality 
    Tells the current no of lines in the document
    checks of invalid commands
*/

void f_nlinex()
{
	
		char line[256];
		
		int linecount = 0;
        char command1[20] = "";
        char movie_name1[200] = "";
        char line_index1[20] = "";
        int prob_index;

        sscanf(message_recd,"%s %[^\n]",command1,line_index1);
        flag = 0;
        prob_index = f_atoi(line_index1);

		filePointerO = fopen("server_file.txt", "r") ;

		if ( filePointerO == NULL ) // check for sucessfull file opening 
		{
			printf( "server_file.txt file failed to open." ) ;
			sprintf(message_sent, "server_file.txt file failed to open. ");
			write(client_id, message_sent, sizeof(message_sent));

		}
		else
		{
			printf( "server_file.txt  file opened.\n" ) ;
            //printf("line index len %lu \n",strlen(line_index1));

            if(strlen(line_index1) > 0) // if multiple words are sent other than the actual command after first whitespace
            {
                printf("\nInvalid Command in : NLINEX.");
                sprintf(message_sent, "Invalid Command in : NLINEX.");	
                write(client_id, message_sent, sizeof(message_sent));
                
                fclose(filePointerO);
                strcpy(message_recd,"");
                strcpy(message_sent,"");
               
            
            }
            else if(parse_count ==1) // when only NLINEX is read alone, sends no of lines in the file
            {

                while(fgets(line,256,filePointerO)!= NULL)
                    linecount++;
                
                printf("\nno of lines in the file are : %d\n", linecount);		
                sprintf(message_sent, "No of lines in the file are : %d", linecount);	
                write(client_id, message_sent, sizeof(message_sent));
                fclose(filePointerO);
                strcpy(message_recd,"");
                strcpy(message_sent,"");
            }

		}

		


}


 
/*  IMLEMENTATION OF READX functionality 
    Tells the lines content in the document
    varify the correctsness of the command and the indices passed
    checks of invalid commands
*/
void f_readx()
{	  
	char line[256];
	int line_count = 0;
    int linecount = 0;
    char command1[20] = "";
    char movie_name1[200] = "";
    char line_index1[20] = "";
    int prob_index;

    sscanf(message_recd,"%s %[^\n]",command1,line_index1);
    flag = 0;
    prob_index = f_atoi(line_index1);  // asertain that the second argument after the command is a valid number or
                                       // it is a alphanumeric string , flag sets to one for alphanumeric/blank
                                       // remains zero for valid numeric arguments
	
	filePointerO = fopen("server_file.txt", "r") ;
	if ( filePointerO == NULL ) // check for successful file opening
    {
        printf( "server_file.txt file failed to open in READX module.\n\n" ) ;
		sprintf(message_sent, "server_file.txt file failed to open in READX module. ");	
		write(client_id, message_sent, sizeof(message_sent));
		strcpy(message_recd,"");
		strcpy(message_sent,"");
        

    }
    else 
    {
        printf("server_file.txt file opened.\n\n" ) ;
       

        while(fgets(line,256,filePointerO)!= NULL) // counting the no of lines 
            line_count++;  

        printf("total line count is : %d \n\n", line_count); 
		rewind(filePointerO);
        strcpy(line, "");
 

		if(parse_count==1 && strlen(line_index1)==0) // if only READX is given as command
		{
			rewind(filePointerO);
            fgets(line,256,filePointerO);

            printf("line at first index is  : %s\n",line);
			sprintf(message_sent, "line at first index is : %s ",line);	
			write(client_id, message_sent, sizeof(message_sent));
			fclose(filePointerO);
			strcpy(line, ""); 
			strcpy(message_recd,"");
			strcpy(message_sent,"");

		}
        else if (parse_count == 1 && strlen(line_index1)>0) // if READX is supplied with a invalid alphanumeric/string as index
        {
                printf(" Invalid Command in READX");
                sprintf(message_sent, "Invalid Command in READX");	
                write(client_id, message_sent, sizeof(message_sent));
                fclose(filePointerO);
                strcpy(message_recd,"");
                strcpy(message_sent,"");
        }  
        else if( parse_count == 2 )  // READX with valid arguments 
        {  

            if( (line_index >= (-1 * line_count ) ) && (line_index < line_count)) // line index valid range --> [-N,N)
            {    
                printf(" correct index \n");
                int temp ;
                if(line_index < 0) // if indexing is reversed
                    temp = line_index + line_count; 
                else
                    temp= line_index; // if indexing is forward

                for(int i = 0; i <= temp ; i++)
                fgets(line,256,filePointerO);

                printf("Line at index %d is : %s ",line_index,line);
                sprintf(message_sent, "Line at index %d is : %s ",line_index,line);	
                write(client_id, message_sent, sizeof(message_sent));
                fclose(filePointerO);
                strcpy(line, ""); 
                strcpy(message_recd,"");
                strcpy(message_sent,"");



            }
            else // // line index  not in valid range --> [-N,N)
            {
                printf(" Line index is Invalid  in READX");
                sprintf(message_sent, "Line index is Invalid in READX");
                write(client_id, message_sent, sizeof(message_sent));
                fclose(filePointerO);
                strcpy(message_recd,"");
                strcpy(message_sent,"");

            }   


        }
              

    }


 fclose(filePointerO);

}

/*  IMLEMENTATION OF INSERTX functionality 
    INSERT the lines in the document
    varify the correctsness of the command and the indices passed
    checks of invalid commands
*/

void f_insertx()
{	 
	char line[256];
	int line_count = 0;
	char command1[20] = "";
	char movie_name1[200] = "";
	char line_index1[20] = "";
	int prob_index;

	sscanf(message_recd,"%s %s %[^\n]",command1,line_index1,movie_name1);
	flag = 0;
	prob_index = f_atoi(line_index1); // CHECKING for if numeric of alphanumeric to validate the command

    /*
    printf("msg : %s*\n", message_recd);
    printf("msg recd len: %lu \n",strlen(message_recd));
	printf("Parse count is : %d\n", parse_count);
	printf("command : %s \n",command1);
	printf("command LEN: %lu \n",strlen(command1));

	printf("index : %s \n", line_index1);
    printf("index LEN: %lu \n",strlen(line_index1));
	printf("movie : %s \n", movie_name1);
    printf("movie LEN: %lu \n",strlen(movie_name1));

	printf("line index 1 %s-\n\n",line_index1);
	printf("flag %d-\n\n",flag);

    printf("flag val : %d\n",flag);
    if (flag==1)
    printf("wrong value\n");
    else
    printf("\natoi value %d flag %d\n", prob_index, flag); */

	if(parse_count == 1 && flag == 1 && !strlen(line_index1)) // when no index and movie name is given
	{   
        
		printf("Invalid command in insertx : provide index and movie name\n\n");

        strcpy(message_sent, "Invalid command in insertx : provide index and movie name");
		write(client_id, message_sent, sizeof(message_sent));

		printf("\n  back to the client  \n\n");
		strcpy(message_recd,"");
		strcpy(message_sent,"");
        
    }
    else if(parse_count == 2) // when no movie name is given
    {   
        printf("INVALID COMMAND : no movie name given \n\n");

        strcpy(message_sent, "INVALID COMMAND : no movie name given");
		write(client_id, message_sent, sizeof(message_sent));
		//printf("\n  back to the client  \n\n");
		strcpy(message_recd,"");
		strcpy(message_sent,"");
    }
    else 
    {   
        filePointerO = fopen("server_file.txt", "r+") ;
		filePointerT = fopen("temp_file.txt", "w+") ;


		if ( filePointerO == NULL )
		{
            printf( "server_file.txt file failed to open in INSERTX module\n\n" ) ;
            sprintf(message_sent, "server_file.txt file failed to open in INSERTX module. ");	
            write(client_id, message_sent, sizeof(message_sent));
            strcpy(message_recd,"");
		    strcpy(message_sent,"");
			
		}
    	else
        {   
            printf( "Server_file.txt file opened.\n" ) ;
            while(fgets(line,256,filePointerO)!= NULL) // counting the no of lines ,for further operations 
            {
            line_count++;
                    
            }
            printf("line count is : %d \n\n", line_count); 
            rewind(filePointerO);

            if( filePointerT == NULL)
            {
                printf( "temporary file failed to open." ) ;
                sprintf(message_sent, "temporary file failed to open in INSERTX module. ");	
                write(client_id, message_sent, sizeof(message_sent));
                strcpy(message_recd,"");
		        strcpy(message_sent,"");

            }
            else
            {
          
                printf( "temp_file.txt file opened.\n" ) ;

                for ( int j = 0 ; j < line_count; j++) // creating the temp file for insertion operations
                {
					fgets(line, sizeof(line), filePointerO);
					fprintf(filePointerT, "%s", line);
				}

            
                //printf("\ntotal no of lines : %d\n",line_count);

                rewind(filePointerT);
                rewind(filePointerO); 

                // when no index is provided , insrting new line at the end of the file
                if(parse_count == 1 && flag == 1 && strlen(line_index1)) 
                {   printf("line without  index \n\n");

                    int i = 0;
                    for(i=1;i<=(line_count+1);i++)
                    {
                            
                    if( i == line_count+1)
                        {   
                            sprintf(line, "%s %s",line_index1,movie_name1);
                            fputs ( "\n", filePointerO ) ;
                            fputs(line,filePointerO);                            

                        }
                        else
                        {
                            fgets(line,sizeof(line),filePointerT);  
                            fprintf(filePointerO,"%s",line);                                  

                        }

                    }           
                        
                        printf("line with out any index  has been added to end ");
                        strcpy(message_sent, "line with out any index added to end ");
		                write(client_id, message_sent, sizeof(message_sent));
                        //printf("\n  back to the client  \n\n");
                        fclose(filePointerO);
                        fclose(filePointerT);
                        strcpy(message_recd,"");
                        strcpy(message_sent,"");


                }

                else if(parse_count == 3 && flag == 0) // all three arguments are correctly sent from the client
                { 
                    
                    if( (line_index >= (-1 * line_count ) ) && (line_index < line_count)) // line index valid range --> [-N,N)
                    {   printf("Correct index \n");
                         int temp ;
                        if(line_index < 0)
                            temp = line_index + line_count;
                        else
                            temp = line_index;

                            
                        for(int i=1;i<=(line_count+1);i++)
                        {
                            if( i == temp+1)
                            {
                                            
                                fputs(movie_name,filePointerO);
                                fputs ( "\n", filePointerO ) ;

                            }
                            else
                            {
                                    fgets(line,sizeof(line),filePointerT);  
                                    fprintf(filePointerO,"%s",line);
                                                

                            }

                        }                      

                            
                            printf("line with index : %d has been added to file.",line_index);
                            sprintf(message_sent, "line with index %d has been added to file.",line_index);	
                            write(client_id, message_sent, sizeof(message_sent));
                            fclose(filePointerT);
                            fclose(filePointerO);
                            strcpy(message_recd,"");
                            strcpy(message_sent,"");
                    
                    } 
                    else            // line index in invalid range
                    {
                        printf(" Line index is invalid  in INSERTX");
                        sprintf(message_sent, "Line index is invalid in INSERTX");
                        write(client_id, message_sent, sizeof(message_sent));
                        fclose(filePointerO);
                        fclose(filePointerT);
                        strcpy(message_recd,"");
                        strcpy(message_sent,"");

                    }
  
                }else if(parse_count == 3 && flag == 1 ) // invalid command with out spaces beteen index and movie name like INSERTX 3Daredavil
                {   
                    printf("INVALID COMMAND : use spacesn\n");
                    strcpy(message_sent, "INVALID COMMAND : use spaces");
                    write(client_id, message_sent, sizeof(message_sent));
                    //printf("\n  back to the client  \n\n");
                    fclose(filePointerO);
                    fclose(filePointerT);
                    strcpy(message_recd,"");
                    strcpy(message_sent,"");

                }              

        }

    }

 }
}


/*
    converts the character / string to the respective Numeric value
    if alphanumeric value is given an flag is set to 1 
    to indicated a incorrect argument

*/
long f_atoi(const char S[])
{
    long num = 0;
    int i = 0, sign = 1;
 
    // note sign of the number
    if (S[i] == '+' || S[i] == '-')
    {
        if (S[i] == '-') 
        {
            sign = -1;
        }
        i++;
    }
 
    // run till the end of the string is reached, or the
    // current character is non-numeric
    if(strlen(S)==0)
    {
    //printf("blank value\n");
    flag = 1;

    }

    while (S[i] )
    {   //printf("inside while\n");

        if(S[i] >= '0' && S[i] <= '9')
        {
        num = num * 10 + (S[i] - '0');
        i++;
        }
        else
        {
            flag = 1;
            break;
        }
    }
 
    return sign * num;
}