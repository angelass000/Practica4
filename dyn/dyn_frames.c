/*
 * hal_uart.c
 *
 *  Created on: 18 mar. 2020
 *      Author: droma
 *
 * Dynamixel framing layer
 */

#include "dyn_frames.h"

#include <stdio.h>

#ifndef __MSP432P401R__
#include "hal_dyn_uart/hal_dyn_uart_emu.h"
#include "dyn_test/fake_msp.h"
#include "../dyn_test/fake_msp.h"

#define f_TxUAC2 TxUAC2_emu
#define f_Sentit_Dades_Tx Sentit_Dades_Tx_emu
#define f_Sentit_Dades_Rx Sentit_Dades_Rx_emu
#define f_Activa_Timer_TimeOut Activa_Timer_TimeOut_emu
#define f_rx_uart_byte rx_uart_byte_emu

#else
#include "hal_dyn_uart/hal_dyn_uart.h"
#include "msp.h"

#define f_TxUAC2 TxUAC2u
#define f_Sentit_Dades_Tx Sentit_Dades_Tx
#define f_Sentit_Dades_Rx Sentit_Dades_Rx
#define f_Activa_Timer_TimeOut Activa_Timer_TimeOut
#define f_rx_uart_byte rx_uart_byte

#endif

//TxPacket()  3 par�metres: ID del Dynamixel, Mida dels par�metres, Instruction byte. torna la mida del "Return packet"
byte TxPacket(byte bID, byte bParameterLength, byte bInstruction,
		const byte * Parametros) {
	byte bCount, bCheckSum, bPacketLength;
	byte TxBuffer[32];
	f_Sentit_Dades_Tx();  //El pin P3.0 (DIRECTION_PORT) el posem a 1 (Transmetre)
	TxBuffer[0] = 0xff;    //Primers 2 bytes que indiquen inici de trama FF, FF.
	TxBuffer[1] = 0xff;
	TxBuffer[2] = bID;         //ID del m�dul al que volem enviar el missatge
	TxBuffer[3] = bParameterLength + 2; //Length(Parameter,Instruction,Checksum)
	TxBuffer[4] = bInstruction;    //Instrucci� que enviem al M�dul

	//TODO: La instrucci� no ha de poder modificar les primeres 5 instruccions
	static int cinco=5; //Variable estatica para no poder modificar las primeras 5 instrucciones al ejecutar
	//la siguiente instrucci�n.
	for (bCount = 0; bCount < bParameterLength; bCount++) //Comencem a generar la trama que hem d�enviar
			{
		TxBuffer[bCount + cinco] = Parametros[bCount];
	}
	bCheckSum = 0;
	bPacketLength = bParameterLength + 4 + 2;
	for (bCount = 2; bCount < bPacketLength - 1; bCount++) //C�lcul del checksum
			{
		bCheckSum += TxBuffer[bCount];
	}
	TxBuffer[bCount] = ~bCheckSum;         //Escriu el Checksum (complement a 1)
	for (bCount = 0; bCount < bPacketLength; bCount++) //Aquest bucle �s el que envia la trama al M�dul Robot
			{
		f_TxUAC2(TxBuffer[bCount]);
	}
	while ((UCA2STATW & UCBUSY)) {
	};   //Espera fins que s�ha transm�s el �ltim byte
	f_Sentit_Dades_Rx(); //Posem la l�nia de dades en Rx perqu� el m�dul Dynamixel envia resposta
	return (bPacketLength);
}

struct RxReturn RxPacket(void) {
	struct RxReturn respuesta;
	byte bCount;
	byte bCheckSum;
	byte bLength;
	respuesta.time_out = false;
	respuesta.idx = 0;
	f_Sentit_Dades_Rx();   //Ponemos la linea half duplex en Rx
	f_Activa_Timer_TimeOut();

	for (bCount = 0; bCount < 4; bCount++) {
		f_rx_uart_byte(&respuesta);
    } //fin del for
	if (!respuesta.time_out) {
		for (bCount = 0; bCount < respuesta.StatusPacket[3]; bCount++) {
			f_rx_uart_byte(&respuesta);
		} //fin del for
	}
	bLength=respuesta.StatusPacket[3]+4 ; //Longitud del StatusPacket

	//TODO: Decode packet and verify checksum
	if(!respuesta.time_out){
		for(bCount=2; bCount<bLength-1; bCount++){
		bCheckSum += respuesta.StatusPacket[bCount];
		}


		bCheckSum= ~bCheckSum; ///Escriu el Checksum (complement a 1)
		if(respuesta.StatusPacket[bLength-1] != bCheckSum){
			respuesta.tx_err=true;

		}
	}

	return respuesta;
}



/**
 * Perform a full read/write transaction
 *
 * This function will send an instruction frame to the dynamixel module
 * and the following status frame
 *
 * @param[in] bID Id of the dynamixel module
 * @param[in] bParameterLength Number of parameters to send
 * @param[in] bInstruction Instruction type of the frame
 * @param[in] Parametros Parameters of the TX frame
 * @return Returns a RxReturn struct with the information of the reply
 */
struct RxReturn RxTxPacket(byte bID, byte bParameterLength, byte bInstruction,
		const byte * Parametros) {
	struct RxReturn respuesta;
	TxPacket(bID, bParameterLength, bInstruction, Parametros);
	respuesta = RxPacket();
	return respuesta;
}
