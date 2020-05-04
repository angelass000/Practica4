/*
 * dyn_sensor.h
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 */

#ifndef DYN_SENSOR_H_
#define DYN_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>
typedef uint8_t byte;

typedef enum _dyn_inst_type
{
    DYN_INSTR_READ = 2,
    DYN_INSTR_WRITE = 3,
} DYN_INST_t;

typedef enum _dyn_reg_typ
{
    DYN_REG_LED = 0x19,
    DYN_REG_MAX = 0x32,
	DYN_CW_Angle_Limit_H=0x07,
	DYN_CW_Angle_Limit_L=0x06,
	DYN_REG_MOV_SPEED__L=0x20,
	DYN_REG_MOV_SPEED__H=0x21,
	DYN_REG_TURN_DIRECTION=0x28,
	DYN_REG_LOAD_DIRECTION=0x29,
	DYN_REG_Left_IR_Sensor_Data=0X1A,
    DYN_REG_Center_IR_Sensor_Data=0X1B,
    DYN_REG_Right_IR_Sensor_Data=0X1C,

}DYN_REG__t;

struct RxReturn distance_wall_front(uint8_t module_id);



#endif /* DYN_SENSOR_H_ */
