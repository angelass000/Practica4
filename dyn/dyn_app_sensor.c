/*
 * dyn_sensor.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * TODO: High-level functions like "distance_wall_front", etc
 * TODO: Generate another file for motors, with functions like "move_forward", etc
 */
#include <stdio.h>
#include <inttypes.h>

#include <stdint-gcc.h>
#include "dyn/dyn_frames.h"
#include "dyn/dyn_app_sensor.h"
#include "dyn_app_sensor.h"
#include "dyn_frames.h"


int distance_wall_front(uint8_t module_id){
	uint8_t parameters[2];
	parameters[0]=0x14; //Direcci�n donde se encuentra la instucci�n para poder leer la distancia detectada cuando hay un obstaculo
	parameters[1]=1; //El parameters[0] ocupa 1 byte
	 //Envia los datos al m�dulo indicado
    struct RxReturn distance= RxTxPacket(module_id, 2, DYN_INSTR_READ, parameters);;
	return distance.StatusPacket[4]; //En la posici�n 4 se encuentra el par�metro donde indica la distacia.
}

