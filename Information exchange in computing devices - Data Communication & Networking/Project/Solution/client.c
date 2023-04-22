/*
provide three arguments
filename server_ip_address port_no

argv[0] filename
argv[1] server_ip_address
argv[2] port_no

*/

/*Client Model should have following functions
1) Socket
2) Connect
3) Write
4) Read
5) close
*/

	//Contains declarartions for input and output
#include <stdio.h>
	//Defines 4 variable types, several macros and various functions
	//We will use atoi function to convert the string pointed to, by the argument to an integer
#include <stdlib.h>
	//defines one variable type, one macro, and various functions for manipulating arrays of characters
#include <string.h>
	//for read, write and close
#include <unistd.h>
	//conatins definitions of a no. of data types used in system calls
#include <sys/types.h>
	//contains constants and structures needed for internet domain addresses e.g. sockaddr_in
#include <netinet/in.h>
	//definitions of structures needed for sockets e.g. defines sockaddr structure
#include <sys/socket.h>
	//contains definition of Hostent structure
#include <netdb.h>
	//declares several functions that are useful for testing and mapping characters.
#include <ctype.h>
	// defines the structure of the data returned by the function fstat() 
#include <sys/stat.h>
	//defines the following requests and arguments for use by the functions fcntl() and open()
#include <fcntl.h>
	//definitions for internet operations 
#include <arpa/inet.h>
#include <rpc/rpc.h>


// void
// add_prog_1(char *host, int x, int y)
// {
// 	CLIENT *clnt;
// 	int  *result_1;
// 	numbers  add_1_arg;

// #ifndef	DEBUG
// 	clnt = clnt_create (host, ADD_PROG, ADD_VERS, "udp");
// 	if (clnt == NULL) {
// 		clnt_pcreateerror (host);
// 		exit (1);
// 	}
// #endif	/* DEBUG */
// 	add_1_arg.num1 = x;
// 	add_1_arg.num2 = y;
// 	result_1 = add_1(&add_1_arg, clnt);
// 	if (result_1 == (int *) NULL) {
// 		clnt_perror (clnt, "call failed");
// 	} else {
// 		printf("Result : %d\n", *result_1);
// 	}
// #ifndef	DEBUG
// 	clnt_destroy (clnt);
// #endif	 /* DEBUG */
// }



//Error function to exit the program
void error(const char *msg)
{
	/*perror is an inbuilt function that interprets the error no. and outputs the
		output error description by using STDERR */

	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{







	
	/*The user need to provide 3 arguments, first one the file name,
	second one -the ip address of server and lastly the port no. */

	//socketfd is file descriptor
	int socketfd, portno, n;

	//sockaddr_in gives internet address
	struct sockaddr_in serv_address;

	//hostent structure is used to store info about the host such as host name, an IPv4 address etc.
	struct hostent * server;
	int sourse_fd;

	//buffer will be transferred in a data stream
	char buffer[1024];

	//variables for operation - send file, chat etc.
	int operation, sub_operation;

	//file name variable
	char str[20];
	int file_name_len, read_len;

	//checking if user provided 3 arguments or not	
	if (argc < 3)
	{
		//Terminating the program as user didnt provide 3 arguments
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);

	}

	// if(argc == 4) {
	// char *host;
	// host = argv[1];
	// add_prog_1 (host, atoi(argv[2]), atoi(argv[3]));
	// }

	//converting the string argument into an int
	portno = atoi(argv[2]);

	//socket function returns a file descriptor, if its less than 0 then there is an error opening socket
	/*socket function has 3 arguments, first one is domain, here we will use AF_INET which is an IPv4 protocol,
	second argument is type, we will use SOCK_STREAM as it for TCP protocol, If we were to use UDP protocl, we would have provide SOCK_DGRAM as second argument,
	third argument is protocol, which is 0 for TCP */
	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketfd < 0)
	{
		//File descrpitor is less than 0, error opening socket
		error("Error opening socket");
	}

	//getting info about host
	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		//server is null, no host exists
		fprintf(stderr, "No such host exists");
	}

	//bzero clears data or anything in whatever it is refrenced to
	//clearing serv_address so as to do operations on it 
	bzero((char*) &serv_address, sizeof(serv_address));

	/*getting info such as port, using htons - host to network short,
	converts port no. in host byte order to port no. in network byte order */
	serv_address.sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char*) &serv_address.sin_addr.s_addr, server->h_length);
	serv_address.sin_port = htons(portno);

	//conneting to host, if file descriptor is less than 0, then the operation failed
	if (connect(socketfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
		error("connection Failed");

	//clearing everything in the output buffer of a stream
	fflush(stdout);

 	int num1 ;
    int num2 ;
    int ans;
    int choice;
    

	//Asking user what he wants to do
	printf("What do you want me to do ?\n 1) Upload File\n 2) Download File\n 3) Test Data (Error Detection and correction using Hamming code) \n 4) Caluclator \n Press other key to terminate\n ");
	//getting user input
	scanf("%d", &operation);

	//we will send this operation no. to server, so that server can execute operations accordingly
	int number_to_send;
	fflush(stdout);
	number_to_send = operation;
	int converted_number = htonl(number_to_send);

	// Telling the server about the operation using write, there is a corresponding read in the server
	write(socketfd, &converted_number, sizeof(converted_number));

	//performing the operation according to user
	switch (operation)
	{
		case 1:
			//user selected upload file
			{
			 	//asking user which type of file he/she wants to send
				bzero(str, 20);
				printf("Which type of file you want to upload? \n1) Text file \n2) Image \n3) Audio \n4) Video \nPress any other key to terminate the program\n");

				//we will send this sub - operation no. to server, so that server can execute sub-operations accordingly
				scanf("%d", &sub_operation);
				int sub_number_to_send;

				//Telling the server about the sub - operation
				fflush(stdout);
				sub_number_to_send = sub_operation;
				int sub_converted_number = htonl(sub_number_to_send);

				// Telling the server about the sub-operation
				write(socketfd, &sub_converted_number, sizeof(sub_converted_number));

				switch (sub_operation)
				{
				 		//performing the sub operation according to user
					case 1:
						{
						 	//User selected txt file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your file that you want to send with the extension. Make sure that the file is in same directory as this file\n");

							//clearing everything in str, so as to save the file name in str
							//we will send this file name to server
							bzero(str, 20);
							scanf("%s", str);

							n = write(socketfd, str, strlen(str));

							//Starting the operation to send the txt file
							FILE * f;

							int words = 0;
							char c;
							
							f = fopen(str, "r");
							while ((c = getc(f)) != EOF)
							//Counting No of words in the file
							{
								fscanf(f, "%s", buffer);
								if (isspace(c) || c == '\t')
									words++;
							}
							//For printing the no. of words
							//printf("Words = %d \n", words);	

							write(socketfd, &words, sizeof(int));
							rewind(f);

							// tells size of the file. Not required for the functionality in code.      
							/*
							fseek(f, 0L, SEEK_END);    	
							int sz = ftell(f);				
							printf("Size is %d \n", sz);
         						rewind(f);  
							*/

							char ch;
							/*
							On getting EOF (or error) from standard input, the client Stdin process, before it exits, needs to tell
							the server that the client is done and there is nothing more coming down
							the socket.  The client has to signal "EOF" to the server.
							*/
							//So we will keep sending the file through buffer until EOF is encountered
							while (ch != EOF)
							{

								fscanf(f, "%s", buffer);
								//For printing every word in the file
								//printf("%s\n", buffer);	
								write(socketfd, buffer, 512);
								ch = fgetc(f);
							}
							//The operation executed successfully
							//The file was sent successfully
							printf("The file was uploaded successfully\n");
							break;
						}
					case 2:

						{

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer*/
							memset(buffer, 0x00, 1024);

							//User selected img file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your image that you want to send with the extension. Make sure that the file is in same directory as this file\n");
							scanf("%s", buffer);
							printf("uploading your Image\n");
							file_name_len = strlen(buffer);

							//sending file name to server
							send(socketfd, buffer, file_name_len, 0);

							//opening the file in the buffer for reading
							sourse_fd = open(buffer, O_RDONLY);
							if (!sourse_fd)
							{
							 					//error opening the file
								perror("Error reading file");
								return 1;
							}

							while (1)
							{

								//reading the file and then sending it through buffer, if read_len is 0 then there is nothing to send and the loop gets terminated
								memset(buffer, 0x00, 1024);
								read_len = read(sourse_fd, buffer, 1024);
								send(socketfd, buffer, read_len, 0);
								if (read_len == 0)
								{
									break;
								}
							}
							//Image was sent successfully
							printf("Image uploaded\n");
							break;
						}
					case 3:
						{
						 	/* memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer*/
							memset(buffer, 0x00, 1024);

							//User selected audio file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your file that you want to send with the extension. Make sure that the file is in same directory as this file\n");
							scanf("%s", buffer);
							printf("uploading your audio file\n");
							file_name_len = strlen(buffer);

							//sending file name to server
							send(socketfd, buffer, file_name_len, 0);

							//opening the file in the buffer for reading
							sourse_fd = open(buffer, O_RDONLY);
							if (!sourse_fd)
							{
							 	//error opening the file
								perror("Error : ");
								return 1;
							}

							while (1)
							{
							 	//reading the file and then sending it through buffer, if read_len is 0 then there is nothing to send and the loop gets terminated
								memset(buffer, 0x00, 1024);
								read_len = read(sourse_fd, buffer, 1024);
								send(socketfd, buffer, read_len, 0);
								if (read_len == 0)
								{
									break;
								}
							}
							//Audio file was sent successfully
							printf("Audio uploaded\n");
							break;
						}
					case 4:
						{
						 	/* memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							//User selected video file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your file that you want to send with the extension. Make sure that the file is in same directory as this file\n");
							scanf("%s", buffer);

							printf("uploading your video file\n");
							file_name_len = strlen(buffer);

							//sending file name to server
							send(socketfd, buffer, file_name_len, 0);

							//opening the file in the buffer for reading
							sourse_fd = open(buffer, O_RDONLY);
							if (!sourse_fd)
							{
							 	//error opening the file
								perror("Error : ");
								return 1;
							}

							while (1)
							{
							 	//reading the file and then sending it through buffer, if read_len is 0 then there is nothing to send and the loop gets terminated
								memset(buffer, 0x00, 1024);
								read_len = read(sourse_fd, buffer, 1024);
								send(socketfd, buffer, read_len, 0);
								if (read_len == 0)
								{
									break;
								}
							}
							//Video file was sent successfully
							printf("Video uploaded\n");
							break;
						}
					default:
						exit(0);
				}
				break;
			}
		case 2:
		{

			 	//asking user which type of file he/she wants to send
				bzero(str, 20);
				printf("Which type of file you want to download? \n1) Text file \n2) Image \n3) Audio \n4) Video \nPress any other key to terminate the program\n");

				//we will send this sub - operation no. to server, so that server can execute sub-operations accordingly
				scanf("%d", &sub_operation);
				int sub_number_to_send;

				//Telling the server about the sub - operation
				fflush(stdout);
				sub_number_to_send = sub_operation;
				int sub_converted_number = htonl(sub_number_to_send);

				// Telling the server about the sub-operation
				write(socketfd, &sub_converted_number, sizeof(sub_converted_number));

				switch (sub_operation)
				{
				 		//performing the sub operation according to user
					case 1:
						{
						 	//User selected txt file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your file that you want to download with the extension. Make sure that the file is in same directory as this file\n");

							//clearing everything in str, so as to save the file name in str
							//we will send this file name to server
							bzero(str, 20);
							scanf("%s", str);

							n = write(socketfd, str, strlen(str));

							//Starting the operation to send the txt file
							//Starting the operation to receive the txt file
							FILE * fp;
							int ch = 0;
							int len = strlen(str);
							str[len - 4] = '\0';
							fp = fopen(strcat(str, "_downloaded.txt"), "a");
							int words;
							read(socketfd, &words, sizeof(int));
							//For counting words in txt file
							printf("No of words in file is : %d\n", words);

							/*
							Receiving the file until there are no words left
							*/
							//So we will keep sending the file through buffer until EOF is encountered
							while (ch != words)
							{
								read(socketfd, buffer, 512);
								fprintf(fp, " %s", buffer);
								//For printing every word of file
								//printf(" %s %d ", buffer, ch); 
								ch++;
							}
							//The operation executed successfully
							//The file was recived successfully
							printf("The file was uploaded successfully\n");
							printf("The file is saved as %s\n", str);


								break;
						}
					case 2:

						{

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer*/
							memset(buffer, 0x00, 1024);

							//User selected img file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your image that you want to send with the extension. Make sure that the file is in same directory as this file\n");
							scanf("%s", buffer);
							file_name_len = strlen(buffer);

							//sending file name to server
							send(socketfd, buffer, file_name_len, 0);

							/*create file */
							buffer[file_name_len  - 4] = '\0';
							sourse_fd = open(strcat(buffer, "_received.png"), O_WRONLY | O_CREAT | O_EXCL, 0700);
							if (!sourse_fd)
							{
								perror("file open error : ");
								break;
							}
							/*file save */
							while (1)
							{
								memset(buffer, 0x00, 1024);
								file_name_len = read(socketfd, buffer, 1024);
								write(sourse_fd, buffer, file_name_len);
								if (file_name_len == EOF | file_name_len == 0)
								{
								 	//if block executed when EOF is encountered, meaning there is nothing left to receive
									//The operation executed successfully
									//The file was recived successfully
									printf("Uploaded Image file\n");
									printf("The file is saved as %s\n", buffer);
									break;
								}
							}
							break;
						}
					case 3:
						{
						 	/* memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer*/
							memset(buffer, 0x00, 1024);

							//User selected audio file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your file that you want to send with the extension. Make sure that the file is in same directory as this file\n");
							scanf("%s", buffer);
							printf("uploading your audio file\n");
							file_name_len = strlen(buffer);

							//sending file name to server
							send(socketfd, buffer, file_name_len, 0);

							//opening the file in the buffer for reading
							sourse_fd = open(buffer, O_RDONLY);
							if (!sourse_fd)
							{
							 	//error opening the file
								perror("Error : ");
								return 1;
							}

							while (1)
							{
							 	//reading the file and then sending it through buffer, if read_len is 0 then there is nothing to send and the loop gets terminated
								memset(buffer, 0x00, 1024);
								read_len = read(sourse_fd, buffer, 1024);
								send(socketfd, buffer, read_len, 0);
								if (read_len == 0)
								{
									break;
								}
							}
							//Audio file was sent successfully
							printf("Audio uploaded\n");
							break;
						}
					case 4:
						{
						 	/* memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							//User selected video file
							//asking the user, the name of the file he wants to send
							//Note : The file should be in the same directory as this client.c file
							printf("Type the name of your file that you want to send with the extension. Make sure that the file is in same directory as this file\n");
							scanf("%s", buffer);

							printf("uploading your video file\n");
							file_name_len = strlen(buffer);

							//sending file name to server
							send(socketfd, buffer, file_name_len, 0);

							//opening the file in the buffer for reading
							sourse_fd = open(buffer, O_RDONLY);
							if (!sourse_fd)
							{
							 	//error opening the file
								perror("Error : ");
								return 1;
							}

							while (1)
							{
							 	//reading the file and then sending it through buffer, if read_len is 0 then there is nothing to send and the loop gets terminated
								memset(buffer, 0x00, 1024);
								read_len = read(sourse_fd, buffer, 1024);
								send(socketfd, buffer, read_len, 0);
								if (read_len == 0)
								{
									break;
								}
							}
							//Video file was sent successfully
							printf("Video uploaded\n");
							break;
						}
					default:
						exit(0);
				}
				break;
		}
		case 3: {
			int datarec[10];
			int test[10];
			int p, p1, p2, p3, p4;

			printf("The data recieved is ");
			for(int i = 0; i < 7; i++) {
				n = read(socketfd, &datarec[i], sizeof(int));
				if(n < 0)
					error("Error on receiving data");
				printf("%d", datarec[i]);
			}

			printf("\nPlease enter the data to be tested \n");
			for(int i = 0; i < 7; i++) {
				scanf("%d", &test[i]);
			}
			
			p1 = test[6] ^ test[4] ^ test[2] ^ test[0];
			p2 = test[5] ^ test[4] ^ test[1] ^ test[0];
			p4 = test[3] ^ test[0] ^ test[2] ^ test[1];

			p = (4 * p4) + (2 * p2) + p1;

			printf("The data for testing is : ");
			for(int i =0; i < 7; i++) {
				printf("%d", test[i]);
			}
			if (p == 0) {
				printf("No error");
			} else
			{
				printf("\n The error is at position %d\n", p);

				printf("The correct data is :");
				if(test[7-p] == 0) {
					test[7-p] = 1;
				} else {
test[7-p] = 0;
				}
				for(int i =0; i < 7; i++) {
				printf("%d", test[i]);
			}
			}
			break;
			
		}
		case 4: {
			 int num1 ;
   	 int num2 ;
    int ans;
    int choice;
    
    
    bzero(buffer,256);						
   n = read(socketfd,buffer,255);				//Read Server String
    if (n < 0) 
         error("ERROR reading from socket");
    printf("Server - %s",buffer);
    scanf("%d" , &choice);						//Enter choice
    write(socketfd, &choice, sizeof(int));  

    bzero(buffer,256);						
    n = read(socketfd,buffer,255);				//Read Server String
    if (n < 0) 
         error("ERROR reading from socket");
    printf("Server - %s\n",buffer);
    scanf("%d" , &num1);						//Enter No 1
    write(socketfd, &num1, sizeof(int));     			//Send No 1 to Server
	
	
	
    bzero(buffer,256);						
    n = read(socketfd,buffer,255);				//Read Server String
    if (n < 0) 
         error("ERROR reading from socket");
    printf("Server - %s\n",buffer);
    scanf("%d" , &num2);						//Enter No 2
    write(socketfd, &num2, sizeof(int));     			//Send No 2 to Server
	
	
    	
   			//Send choice to Server

	   read(socketfd , &ans , sizeof(int));				//Read Answer from Server
    	printf("Server - The answer is : %d\n" , ans);		//Get Answer from server
   	 printf("\n\n");
		break;
		}
	}

 	//closing the socket

	close(socketfd);
	return 0;
}
