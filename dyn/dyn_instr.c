/*
 * hal_dyn.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * This file implements the different dynamixel instructions
 *
 */

#include <stdint-gcc.h>
#include "dyn/dyn_instr.h"
#include "dyn/dyn_frames.h"
#include "dyn_frames.h"
#include "dyn_app_motors.h"

/**
 * Single byte write instruction
 *
 * This function sends a write instruction for a single address position
 * to a given dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the write is performed
 * @param[in] reg_write_val Value written to the previous address
 * @return Error code to be treated at higher levels.
 */
int dyn_write_byte(uint8_t module_id, DYN_REG_t reg_addr, uint8_t reg_write_val) {
	uint8_t parameters[2] ;
	struct RxReturn reply;

	parameters[0] = reg_addr;
	parameters[1] = reg_write_val;
	reply = RxTxPacket(module_id, 2, DYN_INSTR__WRITE, parameters);
    //
    //	//tx_err = Transmission error (error de transmissi�)
	//time_out= La resposta ha trigat m�s del esperat
	//Com els dos s�n boleans, estem retornant en bits diferent un codi d'error que no pot ser
	//tractat a un nivell m�s alt
	return (reply.tx_err < 1) | reply.time_out;
}

/**
 * Single byte read instruction
 *
 * This function sends a read instruction for a single address position
 * to a given dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the read is performed
 * @param[out] reg_read_val Pointer where the read value is stored
 * @return Error code to be treated at higher levels.
 */
int dyn_read_byte(uint8_t module_id, DYN_REG_t reg_addr) {
    uint8_t reg_read_val;
	uint8_t parameters[2];
	struct RxReturn reply;

	parameters[0] = reg_addr;
	parameters[1] = 1;
	reply = RxTxPacket(module_id, 2, DYN_INSTR__READ, parameters);
	reg_read_val = reply.StatusPacket[5];

	return (reply.tx_err < 1) | reply.time_out;
}

/**
 * Multi-byte write instruction
 *
 * This function sends a write instruction starting at a given address position
 * with a given length for a dynamixel module.
 *
 * @param[in] module_id Id of the dynamixel module
 * @param[in] reg_addr Address where the write is performed
 * @param[in] val Pointer to the byte array to be written
 * @param[in] len Number of position to be written
 * @return Error code to be treated at higher levels.
 */
int dyn_write(uint8_t module_id, DYN_REG_t reg_addr, uint8_t *val, uint8_t len) {
	//TODO: Implement multiposition write
	uint8_t parameters[len]; //Inicialitzem la llista amb la len
	struct RxReturn reply;
	parameters[0]=reg_addr;
	for(int i=1; i<len-1; i++){
		parameters[i]=val[i-1];//S'afageix l'array passat per par�metre a la llista parameters[]
	}

	reply = RxTxPacket(module_id, len, DYN_INSTR__WRITE, parameters);

	//tx_err = Transmission error (error de transmissi�)
	//time_out= La resposta ha trigat m�s del esperat
	//Com els dos s�n boleans, estem retornant en bits diferent un codi d'error que no pot ser
	//tractat a un nivell m�s alt
	return (reply.tx_err < 1) | reply.time_out;
}

