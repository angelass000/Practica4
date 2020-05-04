/*
 * dyn_app_motors.c
 *
 * TODO: High-level functions like "distance_wall_front", etc
 * TODO: Generate another file for motors, with functions like "move_forward", etc
 */
#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#include <stdlib.h>
#include <string.h>



#include "dyn/dyn_frames.h"
#include "dyn/dyn_app_motors.h"
#include "dyn_app_motors.h"
#include "dyn_frames.h"


void moure_roda(uint8_t module_id, bool sentit_horari, uint16_t speed){
	byte mov_speed_l, mov_speed_h;
	if(sentit_horari){
		mov_speed_h =0x04; //cap a la dreta
	}else{
		mov_speed_h=0x00; //cap a l'esquerra
	}

    mov_speed_l= speed & 0xFF;
    mov_speed_h|= (speed >> 8);
	uint8_t parameters[3];
	parameters[0]= _DYN_REG__MOV_SPEED__L;
	parameters[1]=mov_speed_l; //Posem a mov_speed_l el Moving Speed(L)
	parameters[2]=mov_speed_h; //Posem a mov_speed_h el Moving Speed (H)

	struct RxReturn resposta=RxTxPacket(module_id, 3, _DYN_INSTR__WRITE, parameters);

	//Per comprovar si funciona:
    uint16_t vel = resposta.StatusPacket[6] | (resposta.StatusPacket[7] & 0x03); // Hem de juntarlos amb els 2 primers bits del paràmetre 7
    uint8_t dire = resposta.StatusPacket[7] & 0x04; // Hem d'agafar el tercer bit del reguistre low retornat a l'status packet
    printf("Sortida donada: ");
    printf("Moven roda ");
    printf("%" PRIu16, resposta.StatusPacket[2]);
    printf(" a velocitat ");
	printf("%" PRIu16 ,vel);
	printf(" i amb direccio ");
    if(dire==4){
        printf("dreta");
    }else{
        printf("esquerra");
    }
	printf("\n");

}

void move_foward(uint8_t roda_1, uint8_t roda_2, uint16_t speed){
	bool sentit_horari=true;
	moure_roda(roda_1, !sentit_horari, speed); //Roda esquerra
	moure_roda(roda_2, sentit_horari, speed); //Roda dreta


}

void move_backward(uint8_t roda_1, uint8_t roda_2,  uint16_t speed){
	bool sentit_horari=true;
	moure_roda(roda_1, sentit_horari, speed); //Roda esquerra
	moure_roda(roda_2, !sentit_horari, speed); //Roda dreta

}

void move_left(uint8_t roda_1, uint8_t roda_2, uint16_t speed){
	bool sentit_horari=true;
	moure_roda(roda_1, !sentit_horari, speed); // Roda esquerra
	moure_roda(roda_2, sentit_horari, (speed+5)); // Roda dreta. Augmenta la velocitat
}

void move_right(uint8_t roda_1, uint8_t roda_2, uint16_t speed){
	bool sentit_horari=true;
	moure_roda(roda_1, !sentit_horari, (speed+5)); // Roda esquerra. Augmenta la velocitat
	moure_roda(roda_2, sentit_horari, speed); //Roda dreta.
}



void canviar_velocitat(uint8_t module_id, uint16_t speed, uint8_t direction){

	byte mov_speed_l, mov_speed_h;
    //Associem a l'mov_speed_l els primers 8 bytes de speed value
	mov_speed_l= speed & 0xFF;
    //Associem a l'mov_speed_h els 2 bytes que falten de speed value i la direcci�
	mov_speed_h=( (direction << 2) & 0x04) | ((speed >> 8) & 0x03);
	uint8_t parameters[3];
	parameters[0]= _DYN_REG__MOV_SPEED__L;
	parameters[1]=mov_speed_l; //Posem a mov_speed_l el Moving Speed(L)
	parameters[2]=mov_speed_h; //Posem a mov_speed_h el Moving Speed(H)

    struct RxReturn resposta=RxTxPacket(module_id, 3, _DYN_INSTR__WRITE, parameters);
    //Per comprovar si funciona:
    uint16_t vel = resposta.StatusPacket[6] | (resposta.StatusPacket[7] & 0x03); // Hem de juntarlos amb els 2 primers bits del paràmetre 7
    uint8_t dire = resposta.StatusPacket[7] & 0x04; // Hem d'agafar el tercer bit del reguistre low retornat a l'status packet


    printf("Canvi de velocitat per la roda ");
    printf("%" PRIu16, resposta.StatusPacket[2]);
    printf(" a velocitat ");
    printf("%" PRIu16 ,vel);
    printf(" i amb direccio ");
    if(dire==4){
        printf("dreta");
    }else{
        printf("esquerra");
    }
    printf("\n");
}

void moure_continuament(uint8_t module_id){
	struct RxReturn resposta;
	uint8_t parameters[5];
	parameters[0]=_DYN_CW_Angle_Limit__L; //Comencem per la adre�a 0x06, on hem de ecriure la informaci�
	parameters[1]=0; //Posem a 0 el CW Angle Limit L
	parameters[2]=0; //Posem a 0 el CW Angle Limit H
	parameters[3]=0; //Posem a 0 el CCW Angle Limit L
	parameters[4]=0; //Posem a 0 el CCW Angle Limit H

	resposta=RxTxPacket(module_id, 5, _DYN_INSTR__WRITE, parameters);
	uint8_t r = resposta.StatusPacket[2];
	printf("Moure roda ");
	printf("%" PRIu16, resposta.StatusPacket[2]);
	printf(" continuament \n");
}

