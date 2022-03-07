#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdlib.h>

#define PORT 5060                     //port 5060 has been assigned to a client-server protocol
#define IP "127.0.0.1"                //the standard address for IPv4 loopback traffic
#define SIZE ((1024*1024)-1)          //the size of the file we created

void send_file(FILE *file, int socket_number){
    char buffer[SIZE] = {0};          //array for the buffer
    int sum=0;                        //variable to sum the number of bytes
    int check_send=-1;                //if the sending failed
    int byte_send=0;                  //the number of the bytes that was send


    while(fgets(buffer, sizeof(buffer), file) != NULL){
        check_send = send(socket_number, buffer, sizeof(buffer), 0);     //check the sending
            if(check_send == -1){                                                        //if the sending of the message failed
                perror("**ERROR** type_error: send the message\n");                   //print tha it was failed
                exit(1);    //there is an error, exit from the program
            }
            else if(sizeof(buffer)>check_send){                //if the sending of the message not failed
                printf("Send: %d bytes until now\n ", byte_send);
            }
        sum=sum+check_send;
        bzero(buffer, SIZE); //function erases the data in the n bytes of the memory starting at the location pointed to by s
    }
    printf("The number of bytes that sent: %d\n",sum);
    return;
}

int main(){
    char buffer[SIZE];          //array for the buffer
    struct sockaddr_in server_address;      //structures for handling internet addresses
    int socket_number=0;
    for(int i=1; i<=10; i++) {              //loop for open 10 sockets
        printf("Socket num: %d \n",i);
        socket_number= socket(AF_INET, SOCK_STREAM, 0);   //create socket

            if (socket_number == -1) {          // if we receive wrong values
                printf("**ERROR** type_error: create socket\n");
                return -1;  //there is an error, exit from the program
            }
            if(i>=6){
                strcpy(buffer,"reno");
                socklen_t len = strlen(buffer);
                if(setsockopt(socket_number, IPPROTO_TCP, TCP_CONGESTION, buffer, len) != 0){
                    perror("**ERROR** type_error: setsockopt\n");
                    return -1;   //there is an error, exit from the program
                }
            }

        memset(&server_address, 0, sizeof(server_address)); // we used memset to fill a block with memory with 0
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT);

        int inet = inet_pton(AF_INET, (const char *) IP, &server_address.sin_addr); //inet_pton - convert IPv4 and IPv6 addresses from text to binary form
            if (inet == -1) { //If af does not contain a valid address family
                printf("**ERROR** type_error: not contain a valid address family \n");
                return -1;   //there is an error, exit from the program
            }

        printf("Trying to connect to the server...\n");
        int check_connect = connect(socket_number, (struct sockaddr *) &server_address, sizeof(server_address));
            if (check_connect < 0) {
                printf("**ERROR** type_error: connect\n");
                return -1;   //there is an error, exit from the program
            }

        printf("Connect to server successfully!\n");
        FILE *file; //file pointer
        file = fopen("1gb.txt","r"); //open the file we created

            if(file == NULL){
                perror("**ERROR** type_error: there is no file\n");
                exit(1);     //there is an error, exit from the program
            }

        send_file(file, socket_number); //send the file to server
        printf("The file was sent\nClosing the connection to server!\n");
        printf("*********************\n\n");
        fclose(file);        //close the file
        close(socket_number);   //close the socket

    }
    return 0;
}
