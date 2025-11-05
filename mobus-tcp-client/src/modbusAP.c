//temos de compilar com -c para dizer que é apenas uma library
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "modbusTCP.h" //é onde esta o Send_Modbus_request
#include "modbusAP.h"

#define MAX_APDU 253

// functions:
//	Write_multiple_regs()
//	Read_h_res()


int Write_multiple_regs(char* server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t* val){ //o start adress tem de ter 16 bits porque há 2^16 registers
		
	if(server_add== NULL || val == NULL){
		return -1;
	}

	if(port<0 || port > 65535){
		return -1;

	}
	//consistencia dos parametros: n_r tem um numero maximo (numero maximo de registers: 1-123, ver no doc. standard)
	if(n_r < 1 || n_r > 123){
		printf("Write_multiple_regs: too many registers");
		return -1; // termina o processo
	}
	if(n_r + st_r > 65536){
		printf("Write_multiple_regs: too many registers");
		return -1; // termina o processo
	}
	//verificar se st_r + n_r é maior que 2^16...
	uint8_t APDU[MAX_APDU]; // APDU da request
	// max APDU 253 bytes -  ver no standard da funçao que nesta camada so pode ter 253 depois ao enviar para o socket na proxima camada adicionamos um cabeçalho de 7 bytes
	
	uint8_t APDU_R[5]; //APDU da Response
	
	// 5 bytes - tamanho da resposta
	int APDUlen;
	st_r=st_r-1;
	
	//assembles APDU (MODBUS PDU)
	APDU[0] = 0x10; // write_multiple regs - function code 0x10(em hexadecimal) = 16(decimal), este é o cabeçalho
	//start adress
	APDU[1] = (uint8_t)(st_r >> 8); // estamos a shiftar à direita de modo a apenas guardar os 8 bits nos HIGH bits do str_r
	APDU[2] = (uint8_t) (st_r); // cast para ficar com os LOW bits
	
	//number of registers
	APDU[3] = (uint8_t)(n_r >> 8);
	APDU[4] = (uint8_t) (n_r);
	
	//byte count 
	APDU[5] = n_r * 2; //registers(2 bytes) hi e lo para cada registo
	
	//write registers values
	for(int i=0; i < n_r; i++){
		APDU[6+2*i] = (uint8_t)(val[i] >> 8); // 6 -> 8 -> 10 (...) byte alto
		APDU[6+2*i+1] = (uint8_t) val[i]; // 7 -> 9 -> 11 (...) byte baixo
	}
	
	APDUlen = 1 + 2 + 2 + 1 + n_r * 2; // size of the APDU - (function code)+(starting adress)+(quantity fo registers)+(byte count)+(registers value)
	
	if(Send_modbus_request(server_add, port, APDU, APDUlen, APDU_R) < 0){
		return (-1);
	}
	
	//checks the response
	
	if(APDU_R[0] & 0x80) // máscara: so deixamos passar o bit mais significativo ver como é 0x80 em binario é algo como 10000000
						 // o if nao deixa passar se for 0 mas deixa se for diferente de 0 (nao precisa de ser 1)
		return -(APDU_R[1]); // retorna o exception code negado
	//se virmos o standard vamos ver que error code - 2 bytes e response - 5 bytes (correu bem)
	
	//returns: number of written registers -ok, <0 - error
	return (n_r);
}




int Read_h_regs(char* server_add, int port, uint16_t st_r, uint16_t n_r, uint16_t* val){ // 32 bits por causa do offset do adress, ficava a um bit de diferença do 16 entao temos de usar 32
	int APDURlen = 1 + 1 + 2 * n_r; // 2*n_r equivale a data efetiva pq vamos ler n_r registers cada um com 2 bytes.
	
	uint8_t APDU[MAX_APDU]; 
	uint8_t APDU_R[APDURlen]; 
	int APDUlen;
	st_r=st_r-1;

	if(server_add== NULL || val == NULL){
		return -1;
	}

	if(port<0 || port > 65535){
		return -1;

	}
	if(n_r + st_r > 65536){
		//printf("Write_multiple_regs: too many registers");
		return -1; // termina o processo
	}

	if(n_r < 1 || n_r > 125){ // Podemos ler mais 2 valores dos que o podemos escrever porque o escreve precisa de saber e dar a inf. sobre qual o bloco a escrever previamente. Como i ler nao pode utilizar esses dois blocos para mais leitura.
		//printf("Read_h_regs: too many registers");
		return -1; 
	}
	
	// faziamos isto se n tuivessemos o offset st_r = st_r - 1;
	
	//assembles APDU (MODBUS PDU)
	APDU[0] = 0x03; 
	APDU[1] = (uint8_t)(st_r >> 8);
	APDU[2] = (uint8_t)(st_r);
	
	//number of registers
	APDU[3] = (uint8_t)(n_r >> 8);
	APDU[4] = (uint8_t) (n_r);
	
	APDUlen = 1 + 2 + 2; // size of the APDU - (function code)+(starting adress)+(quantity fo registers)
	
	if(Send_modbus_request(server_add, port, APDU, APDUlen, APDU_R) < 0){
		return (-1);
	}
	
	if(APDU_R[0] == 0x03 + 0x80) {
		return -(APDU_R[1]); 
	}
	for(int i = 0; i < APDU_R[1]/2; i++){
		val[i] = ((uint16_t)APDU_R[2 + 2 * i] << 8) | ((uint16_t)APDU_R[2 * i + 1 + 2]); // os HIGH bits estao nos endereços pares e os LOW nos impares
	}   // os bytes high sao pares e os low impares
	int read_reg=APDU_R[1]/2;
	//returns: number of readed registers -ok, <0 - error
	return (n_r);					
}

