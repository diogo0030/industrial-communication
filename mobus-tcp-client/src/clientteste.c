#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "modbusAP.h"
#include "modbusTCP.h"

void main(void){
    
    char*  serv_ip = "127.0.0.1";
    int port = 502;

    uint16_t start_reg = 122;
    uint16_t num_reg = 1;

    uint16_t val[1] = {0x41};
    uint16_t A[4]= {0,0,0,0};
    uint16_t B[1]= {0};
    uint16_t C[1]= {77};

    if(Write_multiple_regs(serv_ip, port, start_reg, num_reg, val)<0){
        exit(-1);
    }
    else printf("Write multiple registers OK\n");

    start_reg = 123;
    num_reg = 4;

    if(Read_h_regs(serv_ip, port, start_reg, num_reg, A)<0){
        exit(-1);
    }
    else
    {
        printf("Read holding registers OK\n");
        for (int i = 0; i < num_reg; i++)
            printf(" %d ", A[i]);
    }

    start_reg = 127;
    num_reg = 1;

    if(Read_h_regs(serv_ip, port, start_reg, num_reg, B)<0){
        exit(-1);
    }
    else
    {
        printf("\nRead holding register 127 OK\n");
        printf(" %d \n", B[0]);
    }

    start_reg = 126;
    num_reg = 1; 

    if(Write_multiple_regs(serv_ip, port, start_reg, num_reg, C)<0){
        exit(-1);
    }
    else printf("Write holding register 127 OK\n");

    serv_ip = "10.227.113.1";

    if(Write_multiple_regs(serv_ip, port, start_reg, num_reg, C)<0){
        exit(-1);
    }
    else printf("Write holding register 127 on another server OK\n");






}





