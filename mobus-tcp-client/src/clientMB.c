#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "modbusAP.h" //lembrar q "" pq é local
#include "modbusTCP.h"

//compilar com -c é para nao gerar o programa, é apenas para compilar o programa assim vemos os erros

void main(void){
	
	char*  serv_ip = "127.0.0.1"; // local host; aloca exatamente o espaço necessário para guardar esta string
	int serv_port = 502; //modbus standard port
	
	uint16_t start_reg = 1;
	uint16_t num_reg = 10;
	
	uint16_t vals[5] = {6,7,8,9,10}; // 5 registos, é onde guardamos as respostas recebidas aqui. uint16_t pq cada valor tem 16 bits 
	uint16_t vals2[10] = {0};
	//Write
	//if(Write_multiple_regs(serv_ip, serv_port, start_reg, num_reg, vals)<0){	
	//	printf("error writing registers \n");
	//	exit(-1);
	//}
	//else printf("Write multiple registers OK\n");
	
	//Read
	if(Read_h_regs(serv_ip, serv_port, start_reg, num_reg, vals2)<0){	
		printf("error reading registers \n");
		exit(-1);
	}
	else
    {
        printf("Read holding registers OK\n");
        for (int i = 0; i < num_reg; i++)
            printf(" %d ", vals2[i]);
    }
		
	return;
}