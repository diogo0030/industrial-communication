#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include "modbusTCP.h"

#define unit_id 51

#define SERVER_ADDR "127.0.0.1" // endereço servidor
#define SERVER_PORT 502 //porto do servidor

uint16_t TI = 0;//transaction identifier - global var

int Send_modbus_request(char* server_add, int port, uint8_t* APDU, int APDUlen, uint8_t* APDU_R){
	TI++;
	//construimos o MBAP
	//E ENVIAMOS POR SOCKET
	//DEPOIS ENVIAMOS O APDU POR SOCKETS E VAI TUDO JUNTO COMO SE FOSSE TUDO UMA STRING
	uint16_t PI = 0;   // Protocol Identifier
	uint8_t MBAP[7];
	uint8_t MBAPDU_R[260];
	uint8_t send_buff[7 + APDUlen];
	int fd, out, in;
	struct sockaddr_in server; // struct do addr socket
	
	// generates TI transID sequence number
	MBAP[0] = (uint8_t) (TI >> 8);
	MBAP[1] = (uint8_t) TI;
	
	// append PI=0
	MBAP[2] = (uint8_t) (PI >> 8);
	MBAP[3] = (uint8_t) PI;
	
	//length = APDUlen + 1 (+1 por ser number of FOLLOWING bytes)
	MBAP[4] = (uint8_t) ((APDUlen+1) >> 8); //high
	MBAP[5] = (uint8_t) (APDUlen+1); // low
	
	//unit identifier
	MBAP[6] = unit_id; // 
	
	//Create socket
	fd = socket(PF_INET , SOCK_STREAM , IPPROTO_TCP); //Familia:PF_INET Tipo stream:SOCK_STREAM ---- Retorno o descritor do socket se >0 criou se <0 correu mal
	
	if (fd == -1)
	{
		printf("Socket creation failed...\n"); //controlo de erros
		return -1;
	}
	else
		printf("\nSocket successfully created...\n");
		
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET; //constante
	//server.sin_addr.s_addr = inet_addr(SERVER_ADDR);	// endereço string -> endereço IP 32 bits no formato de rede
	//server.sin_port = htons(SERVER_PORT);	// porto local(so) -> porto de rede

	server.sin_addr.s_addr = inet_addr(server_add);server.sin_addr.s_addr = inet_addr(server_add);
	server.sin_port = htons(port);

	//Connect to remote server
	
	if (connect(fd , (struct sockaddr *)&server , sizeof(server)) < 0) // é feito um cast no segundo parametro. Se esta funçao crrer bem -> >0 se nao <0
	{
		printf("Connection with the server failed...\n"); 
		return -1;
	}
	else
		printf("Connected to the server at address (%s) port (%d)...\n", server_add, port); 	
	
	
	//Send data to the server
	for(int k = 0; k < (7+APDUlen) ;k++){
		
		if(k < 7)send_buff[k] = MBAP[k];
		else send_buff[k] = APDU[k-7];
	}

	out = send(fd , send_buff, APDUlen + 7 , 0); //Envia MBAP(7 bytes) + APDU
	if(out < 0)
	{
		printf("Send failed..\n");
		close(fd);
		return -1;
	}
	else
		printf("Sent data (%d bytes)\n", out);
	
	
	in = recv(fd, MBAPDU_R , sizeof(MBAPDU_R), 0); //Lê a resposta inteira (MBAP + APDU) para MBAPDU_R
	if(in < 0)
	{
		printf("Recv failed..\n");
		close(fd);
		return -1;
	}
	else
		printf("Received data (%d bytes)\n", in); 
	
	
	    //  Transaction Identifier
		//Confere se o TI bate com o do pedido. Se não descarta.
    if ((((uint16_t)(MBAPDU_R[0]) << 8) | (uint16_t)(MBAPDU_R[1])) != TI)
    {
        printf("Incorrect TI \n");
        close(fd);
        return -1;
    }
    
    // APDU lenght 
    int APDU_Rlen = (((uint16_t)(MBAPDU_R[4]) << 8) | ((uint16_t)(MBAPDU_R[5]))) - 1;

    // Copy data
    for (int i = 0; i < APDU_Rlen; i++)
        APDU_R[i] = MBAPDU_R[7 + i];

    // Close connection and return
    close(fd);
    return 0;

}