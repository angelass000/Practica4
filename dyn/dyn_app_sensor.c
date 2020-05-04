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



struct RxReturn distance_wall_front(uint8_t module_id){
	uint8_t parameters[2];
	parameters[0]=DYN_REG_Left_IR_Sensor_Data;
    parameters[1]=3;
	//Envia los datos al módulo indicado
    struct RxReturn distance= RxTxPacket(module_id, 2, DYN_INSTR_READ, parameters);
    return distance; //En la posición 4 se encuentra el parámetro donde indica la distacia.

}

