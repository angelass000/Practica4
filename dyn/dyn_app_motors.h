/*
 * hal_dyn.h
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 */

#ifndef DYN_APP_MOTORS_H_
#define DYN_APP_MOTORS_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t byte;

typedef enum _dyn_ins_type
{
    _DYN_INSTR_READ = 2,
    _DYN_INSTR__WRITE = 3,
} DYN_INS_t;

typedef enum _dyn_re_type
{
    _DYN_REG__LED = 0x19,
    _DYN_REG__MAX = 0x32,
    _DYN_CW_Angle_Limit__H=0x07,
    _DYN_CW_Angle_Limit__L=0x06,
    _DYN_REG__MOV_SPEED__L=0x20,
    _DYN_REG__MOV_SPEED__H=0x21,
    _DYN_REG__TURN_DIRECTION=0x28,
    _DYN_REG__LOAD_DIRECTION=0x29,
}DYN_RE_t;

void moure_roda(uint8_t module_id, bool sentit_horari, uint16_t speed);
void move_foward(uint8_t roda_1, uint8_t roda_2, uint16_t speed);
void move_backward(uint8_t roda_1, uint8_t roda_2, uint16_t speed);
void move_left(uint8_t roda_1, uint8_t roda_2, uint16_t speed);
void move_right(uint8_t roda_1, uint8_t roda_2, uint16_t speed);
void canviar_velocitat(uint8_t module_id, uint16_t speed, uint8_t direction);
void moure_continuament(uint8_t module_id);
#endif /* DYN_APP_MOTORS_H_ */
