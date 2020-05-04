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


uint16_t distance_wall_front(uint8_t module_id){
	uint8_t parameters[4];
	parameters[0]=DYN_REG_Left_IR_Sensor_Data;
    parameters[1]=0x1A;
	parameters[2]=0x1B;
	parameters[3]=0x1C;
	 //Envia los datos al módulo indicado
    struct RxReturn distance= RxTxPacket(module_id, 4, DYN_INSTR_READ, parameters);
    printf("%" PRIu8, distance.StatusPacket[4]);
	return distance.StatusPacket[4]; //En la posición 4 se encuentra el parámetro donde indica la distacia.
}

