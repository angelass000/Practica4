/*
 * dyn_sensor.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * TODO: High-level functions like "distance_wall_front", etc
 * TODO: Generate another file for motors, with functions like "move_forward", etc
 */

#include "dyn/dyn_frames.h"
#include "dyn/dyn_app_sensor.h"


int distance_wall_front(uint8_t module_id){
	uint8_t parameters[2];
	struct RxReturn distance;
	parameters[0]=0x14; //Dirección donde se encuentra la instucción para poder leer la distancia detectada cuando hay un obstaculo
	parameters[1]=1; //El parameters[0] ocupa 1 byte
	TxPacket(module_id, 2, DYN_INSTR_READ, parameters); //Envia los datos al módulo indicado
	distance=RxPacket(); //Recibe una respuesta de dicho módulo
	return distance.StatusPacket[5]; //En la posición 5 se encuentra el parámetro donde indica la distacia.

}

