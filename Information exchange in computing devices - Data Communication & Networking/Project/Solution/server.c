/*
stdio.h contains declarations for unput and output

stdlib.h contains declarations of variable types, macros and functions
We will be using atoi function of stlib which converts a string pointed to, by the argument into an int

unistd.h contains read write close functions

sys/types.h contains definitions of data types used in sysytem calls

sys/socket.h contains structers needed for socket. sockaddr structure is in this file only

netinet/in.h contains constants and structures needed for internet domain address. sockaddr_in is in this file only

*/

/*Server Model should have following functions
1) Socket
2) Bind
3) listen
4) Accept
5) Read
6) Write
7) close
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
	//contains system error numbers
#include <errno.h>



// int *
// add_1_svc(numbers *argp, struct svc_req *rqstp)
// {
// 	static int  result;

// 	printf("add(%d, %d), is called\n", argp->num1, argp->num1);
// 	result = argp->num1 + argp->num1;

// 	return &result;
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
	/*The user need to provide 2 arguments, first one the file name,
		 and secondly the port no. */


	
	//checking if user provided 2 arguments or not
	if (argc < 2)
	{
		//Terminating the program as user didnt provide 2 arguments
		fprintf(stderr, "Please provide a port number, program is terminated\n");
		exit(1);
	}

	//socketfd and newsocketfd is file descriptor, newsocketfd is for client
	int socketfd, newsocketfd, portno, n;

	//file name variable
	char str[20];
	int client_len, read_len, file_read_len;
	int des_fd;

	//variables for operation - send file, chat etc.
	int received_int, sub_received_int;
	int operation, return_status, sub_return_status;
	int sub_operation;

	//buffer will be transferred in a data stream
	char buffer[1024];

	//sockaddr_in gives internet address
	struct sockaddr_in serv_address, client_address;
	socklen_t clilen;

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

	//bzero clears data or anything in whatever it is refrenced to
	//clearing serv_address so as to do operations on it 
	bzero((char*) &serv_address, sizeof(serv_address));

	//converting the string argument into an int
	portno = atoi(argv[1]);

	/*getting info such as port, using htons - host to network short,
		converts port no. in host byte order to port no. in network byte order */
	serv_address.sin_family = AF_INET;
	serv_address.sin_addr.s_addr = INADDR_ANY;
	serv_address.sin_port = htons(portno);

	/*bind assigns the address specified by addr */
	if (bind(socketfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
		error("Binding Failed");

	/*listen marks the socketfd as passive soocket i.e. A socket that will be used for accepting incoming connections using accept */
	listen(socketfd, 5);
	clilen = sizeof(client_address);

	/*accept function also returns a file descriptor and waits for connect and whenever connect is triggered by client, accept is called by host */
	newsocketfd = accept(socketfd, (struct sockaddr *) &client_address, &clilen);

	if (newsocketfd < 0)
		error("Error Accepting");

	/*getting the operation by the user */

	return_status = read(newsocketfd, &received_int, sizeof(received_int));
	if (return_status > 0)
	{
		//printing the opeartion code
		fprintf(stdout, "Operation code = %d\n", ntohl(received_int));
		operation = ntohl(received_int);
	}

	//performing the operation according to user
	switch (operation)
	{
		case 1:
			//user selected upload file
			{
			 	//asking user which type of file he/she wants to send
				sub_return_status = read(newsocketfd, &sub_received_int, sizeof(sub_received_int));
				if (return_status > 0)
				{
				 		//printing the sub operation code
					fprintf(stdout, "Sub Operation code = %d\n", ntohl(sub_received_int));
					sub_operation = ntohl(sub_received_int);
				}

				switch (sub_operation)
				{
				 		//performing the sub operation according to user
					case 1:

						{

						 	//User selected txt file
							//reading the filename given by user
							bzero(str, 20);
							n = read(newsocketfd, str, 20);
							printf("%s", str);
							int len = strlen(str);

							//manupilating the file name, the last 4 chars are the extension, so removing the extension from the name
							str[len - 4] = '\0';
							if (n < 0)
								error("Error Reading");

							fprintf(stdout, "Received File Name = %s\n", str);

							//Starting the operation to receive the txt file
							FILE * fp;
							int ch = 0;
							fp = fopen(strcat(str, "_received.txt"), "a");
							int words;
							read(newsocketfd, &words, sizeof(int));
							//For counting words in txt file
							printf("No of words in file is : %d\n", words);

							/*
							Receiving the file until there are no words left
							*/
							//So we will keep sending the file through buffer until EOF is encountered
							while (ch != words)
							{
								read(newsocketfd, buffer, 512);
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

							char file_name[1024];	// local val

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							/*file name */
							read_len = read(newsocketfd, buffer, 1024);
							if (read_len > 0)
							{
							/*getting the file name and manupilating the file name, the last 4 chars are the extension, so removing the extension from the name*/
								strcpy(file_name, buffer);
								int len = strlen(file_name);
								file_name[len - 4] = '\0';
							}
							else
							{
							 	//Error reading the file name, terminating the program
								close(newsocketfd);
								break;
							}

							/*create file */

							des_fd = open(strcat(file_name, "_received.png"), O_WRONLY | O_CREAT | O_EXCL, 0700);
							if (!des_fd)
							{
								perror("file open error : ");
								break;
							}
							/*file save */
							while (1)
							{
								memset(buffer, 0x00, 1024);
								file_read_len = read(newsocketfd, buffer, 1024);
								write(des_fd, buffer, file_read_len);
								if (file_read_len == EOF | file_read_len == 0)
								{
								 	//if block executed when EOF is encountered, meaning there is nothing left to receive
									//The operation executed successfully
									//The file was recived successfully
									printf("Uploaded Image file\n");
									printf("The file is saved as %s\n", file_name);
									break;
								}
							}
							break;
						}
					case 3:
						{
							char file_name[1024];	// local val

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							/*file name */
							read_len = read(newsocketfd, buffer, 1024);
							if (read_len > 0)
							{
							 	/* getting the file name and manupilating the file name, the last 4 chars are the extension, so removing the extension from the name */
								strcpy(file_name, buffer);
								int len = strlen(file_name);
								file_name[len - 4] = '\0';
							}
							else
							{
							 	//Error reading the file name, terminating the program
								close(newsocketfd);
								break;
							}

							/*create file */

							des_fd = open(strcat(file_name, "received.mp3"), O_WRONLY | O_CREAT | O_EXCL, 0700);
							if (!des_fd)
							{

								perror("file open error : ");
								break;
							}
							/*file save */
							while (1)
							{
								memset(buffer, 0x00, 1024);
								file_read_len = read(newsocketfd, buffer, 1024);
								write(des_fd, buffer, file_read_len);
								if (file_read_len == EOF | file_read_len == 0)
								{
								 	//if block executed when EOF is encountered, meaning there is nothing left to receive
									//The operation executed successfully
									//The file was recived successfully
									printf("Uploaded audio file\n");
									printf("The file is saved as %s\n", file_name);
									break;
								}
							}
							break;
						}

					case 4:
						{
							char file_name[1024];	// local val

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							/*file name */
							read_len = read(newsocketfd, buffer, 1024);
							if (read_len > 0)
							{
							 	/* getting the file name and manupilating the file name, the last 4 chars are the extension, so removing the extension from the name */
								strcpy(file_name, buffer);
								int len = strlen(file_name);
								file_name[len - 4] = '\0';
							}
							else
							{
							 	//Error reading the file name, terminating the program
								close(newsocketfd);
								break;
							}

							/*create file */

							des_fd = open(strcat(file_name, "received.mp4"), O_WRONLY | O_CREAT | O_EXCL, 0700);
							if (!des_fd)
							{

								perror("file open error : ");
								break;
							}
							/*file save */
							while (1)
							{
								memset(buffer, 0x00, 1024);
								file_read_len = read(newsocketfd, buffer, 1024);
								write(des_fd, buffer, file_read_len);
								if (file_read_len == EOF | file_read_len == 0)
								{
								 	//if block executed when EOF is encountered, meaning there is nothing left to receive
									//The operation executed successfully
									//The file was recived successfully
									printf("Uploaded Video file\n");
									printf("The file is saved as %s\n", file_name);
									break;
								}
							}
							break;
						}
					default:
						exit(0);
				}
				break;
			}
		
		case 2: {
			//asking user which type of file he/she wants to download
				sub_return_status = read(newsocketfd, &sub_received_int, sizeof(sub_received_int));
				if (return_status > 0)
				{
				 		//printing the sub operation code
					fprintf(stdout, "Sub Operation code = %d\n", ntohl(sub_received_int));
					sub_operation = ntohl(sub_received_int);
				}

				switch (sub_operation)
				{
				 		//performing the sub operation according to user
					case 1:

						{

						 	//User selected txt file
							//reading the filename given by user
							bzero(str, 20);
							n = read(newsocketfd, str, 20);
							int len = strlen(str);

							//manupilating the file name, the last 4 chars are the extension, so removing the extension from the name
							str[len - 4] = '\0';
							if (n < 0)
								error("Error Reading");

							fprintf(stdout, "Received File Name for download = %s\n", str);

							FILE * f;

							int words = 0;
							char c;
							strcat(str, "_received.png");
							
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
							printf("The file was sent successfully\n");
							break;


							
						}
					case 2:
						{

							char file_name[1024];	// local val

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							/*file name */
							read_len = read(newsocketfd, buffer, 1024);
							if (read_len > 0)
							{
							/*getting the file name and manupilating the file name, the last 4 chars are the extension, so removing the extension from the name*/
								strcpy(file_name, buffer);
								int len = strlen(file_name);
								file_name[len - 4] = '\0';
							}
							else
							{
							 	//Error reading the file name, terminating the program
								close(newsocketfd);
								break;
							}
								strcat(buffer, "_received.png");

							//opening the file in the buffer for reading
							des_fd = open(buffer, O_RDONLY);
							if (!des_fd)
							{
							 					//error opening the file
								perror("Error reading file");
								return 1;
							}
							bzero(buffer, 1024);
							while (1)
							{
								
								//reading the file and then sending it through buffer, if read_len is 0 then there is nothing to send and the loop gets terminated
								memset(buffer, 0x00, 1024);
								read_len = read(des_fd, buffer, 1024);
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
							char file_name[1024];	// local val

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							/*file name */
							read_len = read(newsocketfd, buffer, 1024);
							if (read_len > 0)
							{
							 	/* getting the file name and manupilating the file name, the last 4 chars are the extension, so removing the extension from the name */
								strcpy(file_name, buffer);
								int len = strlen(file_name);
								file_name[len - 4] = '\0';
							}
							else
							{
							 	//Error reading the file name, terminating the program
								close(newsocketfd);
								break;
							}

							/*create file */

							des_fd = open(strcat(file_name, "received.mp3"), O_WRONLY | O_CREAT | O_EXCL, 0700);
							if (!des_fd)
							{

								perror("file open error : ");
								break;
							}
							/*file save */
							while (1)
							{
								memset(buffer, 0x00, 1024);
								file_read_len = read(newsocketfd, buffer, 1024);
								write(des_fd, buffer, file_read_len);
								if (file_read_len == EOF | file_read_len == 0)
								{
								 	//if block executed when EOF is encountered, meaning there is nothing left to receive
									//The operation executed successfully
									//The file was recived successfully
									printf("Uploaded audio file\n");
									printf("The file is saved as %s\n", file_name);
									break;
								}
							}
							break;
						}

					case 4:
						{
							char file_name[1024];	// local val

							/*memset copies the character 0 (an unsigned char) to the first 1024 characters of the string pointed to, by the argument buffer */
							memset(buffer, 0x00, 1024);

							/*file name */
							read_len = read(newsocketfd, buffer, 1024);
							if (read_len > 0)
							{
							 	/* getting the file name and manupilating the file name, the last 4 chars are the extension, so removing the extension from the name */
								strcpy(file_name, buffer);
								int len = strlen(file_name);
								file_name[len - 4] = '\0';
							}
							else
							{
							 	//Error reading the file name, terminating the program
								close(newsocketfd);
								break;
							}

							/*create file */

							des_fd = open(strcat(file_name, "received.mp4"), O_WRONLY | O_CREAT | O_EXCL, 0700);
							if (!des_fd)
							{

								perror("file open error : ");
								break;
							}
							/*file save */
							while (1)
							{
								memset(buffer, 0x00, 1024);
								file_read_len = read(newsocketfd, buffer, 1024);
								write(des_fd, buffer, file_read_len);
								if (file_read_len == EOF | file_read_len == 0)
								{
								 	//if block executed when EOF is encountered, meaning there is nothing left to receive
									//The operation executed successfully
									//The file was recived successfully
									printf("Uploaded Video file\n");
									printf("The file is saved as %s\n", file_name);
									break;
								}
							}
							break;
						}
					default:
						exit(0);
				}
				break;
		}
		case 3: {
			int data[10];
			printf("Please input 4 bits of data\n");
			scanf("%d", &data[0]);
			scanf("%d", &data[1]);
			scanf("%d", &data[2]);
			scanf("%d", &data[4]);

			//calculations of even parity for encoding
			data[6] = data[4] ^ data[2] ^ data[0];
			data[5] = data[4] ^ data[1] ^ data[0];
			data[3] = data[0] ^ data[2] ^ data[1];

			//Sending the encoded data to client
			for(int i =0; i < 7; i++) {
				n = write(newsocketfd, &data[i], sizeof(int));
				if(n < 0)
					printf("Error while transmiting the data");
			}
			printf("The data sent is ");
			for(int i = 0; i< 7; i++) {
				printf("%d", data[i]);
			}
			break;
		}
		case 4: {
			int num1 , num2 , ans , choice;
	   n = write(newsocketfd,"Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n",strlen("Enter your choice : \n1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n"));         //Ask for choice    
     if (n < 0) error("ERROR writing to socket");
     read(newsocketfd, &choice, sizeof(int));					//Read choice
     printf("Client - Choice is : %d\n" , choice);


      n = write(newsocketfd,"Enter Number 1 : ",strlen("Enter Number 1"));         //Ask for Number 1     
     if (n < 0) error("ERROR writing to socket");
     read(newsocketfd, &num1, sizeof(int));					//Read No 1
     printf("Client - Number 1 is : %d\n" , num1);
          
          
     n = write(newsocketfd,"Enter Number 2 : ",strlen("Enter Number 2"));         //Ask for Number 2    
     if (n < 0) error("ERROR writing to socket");
     read(newsocketfd, &num2, sizeof(int));					//Read Number 2
     printf("Client - Number 2 is : %d\n" , num2);
     
     

     
      switch(choice)
     {
     	case 1:
     		ans = num1 + num2;
     		break;
     	case 2:
     		ans = num1 -num2;
     		break;
     	case 3:
     		ans = num1*num2;
     		break;
     	case 4:
     		ans = num1/num2;
     		break;
     	default:
     		break;
     }
     
     write(newsocketfd , &ans , sizeof(int));
	break;
		}


		default:
			//user entered other keyword, so we have to terminate the program 
			printf("Other key entered, terminating\n");
			exit(0);
	}

	//closing the socket
	close(newsocketfd);
	close(socketfd);
	return 0;
}
