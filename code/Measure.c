#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>

#define PORT 5060                     //port 5060 has been assigned to a client-server protocol
#define IP "127.0.0.1"                //the standard address for IPv4 loopback traffic
#define SIZE ((1024*1024)-1)          //the size of the file we created
int i=1, k=1;

void write_file(int socket){
    char buffer[SIZE] = {0};         //array for the buffer
    int sum=0;                       //variable to sum the number of bytes
    int check_recv=0;                //variable for check the receive

    while(1){
        check_recv = recv(socket, buffer, sizeof(buffer), 0); //The recv function shall return the length of the message written to the buffer
        sum=sum+check_recv;
        if(check_recv <= 0){
            if(check_recv == -1){
                printf("**ERROR** type_error: not receive the file\n");
                close(socket);//close the socket
            }
            else{
                if(i>=1&&i<=5) {
                    printf("num: %d successfully received -cubic-\n", i);
                }
                i++;
                if(i>=7&&i<=11) {
                    printf("num: %d successfully received -reno-\n", k);
                    k++;
                }
            }
            break;
        }
        bzero(buffer, SIZE); //function erases the data in the n bytes of the memory starting at the location pointed to by s

    }
    printf("The number of bytes that sent: %d \n", sum);
    return;
}



int main(){
    int server_socket=-1,client_socket=-1;
    struct sockaddr_in server_address, client_address; //structures for handling internet addresses
    server_socket = socket(AF_INET, SOCK_STREAM, 0);//create socket

        if (server_socket < 0) { // if the creating of the socket failed
            printf("**ERROR** type_error: cannot create socket\n");
            return -1; //there is an error, exit from the program
        }

    memset(&server_address, 0, sizeof(server_address)); // we used memset to fill a block with memory with 0
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) { //bind function binds the socket to the address and port number specified in address
            printf("**ERROR** type_error: cannot binds the socket to the address and port number specified in address\n");
            close(server_socket); //close the socket
            return -1;                //there is an error, exit from the program
        }

    int check_listen = listen(server_socket , 10); //The listen() function shall mark a connection-mode socket, specified by the socket argument, as accepting connections.
        if(check_listen < 0 ){ // if the listen was filed
            printf("**ERROR** type_error: the listen was filed \n");
            close(server_socket); //close the socket
            return -1;                //there is an error, exit from the program
        }

    socklen_t client_address_size = sizeof(client_address); // socklen_t which is an unsigned opaque integral type of length of at least 32 bits

    double cnt_time;
    int j=1,cubic=0, reno=0;
    struct timeval first,second; //struct timeval represents an elapsed time.

    while (1){
        memset(&client_address, 0, sizeof(client_address)); // deleting first n chars in the server_address that we're receiving and swapping them with .c
        client_address_size = sizeof(client_address);               //update the length
        client_socket = accept(server_socket, (struct  sockaddr*)&client_address, &client_address_size); //The accept function call is used by a server to accept a connection request from a client.
            if(client_socket == -1){ // if accept was filed
                printf("**ERROR** type_error:server not accept the connection request from a client\n");
                close(server_socket); //close the socket
                return -1;                //there is an error, exit from the program
            }

        gettimeofday(&first, NULL); //The gettimeofday function gets the system's clock time
        write_file(client_socket);
        gettimeofday(&second, NULL);

        //millisecond calculation:
        cnt_time=(second.tv_sec-first.tv_sec)*1000.0;
        cnt_time+=(second.tv_usec-first.tv_usec)/1000.0;

        //checking what is the algorithm
        if(j <= 5){
            cubic+=cnt_time;
        }
        else {
            reno+=cnt_time;
        }
        j++;
        if((j>=2) && (j<=11))
            printf("Time: %lf millisecond\n", cnt_time);
        //done the cubic algorithm
        if(j==6){
            printf("\n#####################\n");
            printf("The average of cubic is: %lf millisecond\n",(cubic/5.0));
            printf("#####################\n");
        }
        //done the reno algorithm
        if(j==11){
            printf("\n#####################\n");
            printf("The average of reno is: %lf millisecond\n",(reno/5.0));
            printf("#####################\n");
            break;
        }
        printf("*********************\n\n");
    }
    close(server_socket);  //close the file
    return 0;
}

