#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "modbusAP.h" //lembrar q "" pq é local
#include "modbusTCP.h"

//compilar com -c é para nao gerar o programa, é apenas para compilar o programa assim vemos os erros

void main(void){
	
    //Ex1
    int write1;
	char*  ip1 = "127.0.0.1"; // local host; aloca exatamente o espaço necessário para guardar esta string
	char*  ip2 = "10.227.113.1";
	int serv_port = 502; //modbus standard port
	int r,result = 0;
	
	//ex1
	uint16_t start_reg = 122;
	uint16_t num_reg = 1;
	uint16_t val[1] = {0x41};
	r=Write_multiple_regs(ip1,serv_port,start_reg,num_reg,val);
	if(r<0) {result = -r; printf("%d",result); exit(-1);} // errro Modbus
	if(r != 1) {result = -1; printf("%d ex1",result); exit(-1);} // errro nao Modbus
	//ex2
	uint16_t A[4]={0};
	start_reg = 123;
	num_reg = 4;
	r=Read_h_regs(ip1,serv_port,start_reg,num_reg,A);
	if(r<0) {result = -r; printf("%d",result); exit(-1);} // errro Modbus
	if(r != 4) {result = -1; printf("%d ex2",result); exit(-1);} // errro nao Modbus
	//ex3
	start_reg = 127;
	num_reg = 1;
	uint16_t B[1] = {0};
	r=Read_h_regs(ip1,serv_port,start_reg,num_reg,B);
	if(r<0) {result = -r; printf("%d",result); exit(-1);} // errro Mod
	if(r != 1) {result = -1; printf("%d ex3",result); exit(-1);} // errro nao Modbus
	//ex7

	uint16_t C[1]={0};
	if((B[0] == 0)) C[0] = 9999;
	else C[0] = A[0] + A[3];
	//ex4
	start_reg = 128;
	num_reg = 1;
	r=Write_multiple_regs(ip1,serv_port,start_reg,num_reg,C);
	if(r<0) {result = -r; printf("%d",result); exit(-1);} // errro Modbus
	if(r != 1) {result = -1; printf("%d ex4",result);  exit(-1);} // errro
	//ex5

	start_reg = 129;
	num_reg = 1;
	r=Read_h_regs(ip2,serv_port,start_reg,num_reg,C);
	if(r<0) {result = -r; printf("%d",result); exit(-1);} // errro Modbus
	if(r != 1) {result = -1; printf("%d",result); exit(-1);} // errro

	printf("%d",result);









}