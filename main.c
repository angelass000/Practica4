#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "dyn/dyn_app_common.h"
#include "dyn_test/dyn_emu.h"
#include "dyn_test/b_queue.h"
#include "joystick_emu/joystick.h"
#include "dyn/dyn_app_motors.h"
#include "dyn/dyn_app_sensor.h"

#define MOTOR_2_AX_12 0x02 //Roda esquerra
#define MOTOR_3_AX_12 0x01 //Roda dreta
#define SENSOR_AX_S1 0x00

uint8_t estado = Ninguno, estado_anterior = Ninguno, finalizar = 0;
/**
 * main.c
 */
int main(void)
{
	pthread_t tid, jid;
	uint8_t tmp;

	//Init semaphores for TX data handshake
	//Inicialitzen el objecte sem�for amb un valor 0(en "vermell") per poder-lo fer servir despr�s.
	sem_init(&sem_tx_msp, 0, 0);
	sem_init(&sem_tx_cli, 0, 0);

	//Init queue for RX data
	init_queue();

	//Start thread for dynamixel module emulation
	//Crea un thread separat que executar� la funcio dyn_emu.
	//Els altres arguments no s�n rellevant en el nostre cas
	pthread_create(&tid, NULL, dyn_emu, (void*) &tid);
	pthread_create(&jid, NULL, joystick_emu, (void*) &jid);

	//Testing some high level function
	printf("MAIN: Setting LED to 0 \n");
    dyn_led_control(1, 0);
    printf("MAIN: Getting LED value \n");
    dyn_led_read(1, &tmp);
    //assert(tmp == 0);
    printf("MAIN: Setting LED to 1 \n");
	dyn_led_control(1, 1);
	printf("MAIN: Getting LED value \n");
    dyn_led_read(1, &tmp);
    assert(tmp == 1);


	printf("************************\n");
	printf("Test passed successfully\n");
	printf("Pulsar 'q' para terminar, qualquier tecla para seguir\r");
	fflush(stdout);//	return 0;

	while(estado != Quit)
	{
		uint16_t velocitat=15;
		byte direccio=1;
		Get_estado(&estado, &estado_anterior);
		if(estado != estado_anterior){
			Set_estado_anterior(estado);
			printf("estado = %d\n", estado);
			fflush(stdout);
			switch(estado){
			case Up:
				printf("Boton Up ('i') apretado\n");
				printf("Comanda moviment endavant a velocitat" , velocitat, " pels motors" , MOTOR_2_AX_12, MOTOR_3_AX_12);
				move_foward(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
				break;
			case Down:
				printf("Boton Down ('m') apretado\n");
				printf("Comanda moviment enrrera a velocitat" , velocitat, " pels motors" , MOTOR_2_AX_12,MOTOR_3_AX_12);
				move_backward(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
				break;
			case Left:
				printf("Boton Left ('j') apretado\n");
				printf("Comanda moviment esquerra a velocitat" , velocitat, " pels motors" , MOTOR_2_AX_12, MOTOR_3_AX_12);
				move_left(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
				break;
			case Right:
				printf("Boton Right ('l') apretado\n");
				printf("Comanda moviment dreta a velocitat" , velocitat, " pels motors" , MOTOR_2_AX_12, MOTOR_3_AX_12);
				move_right(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
				break;
			case Center:
				printf("Boton Center ('k') apretado\n");
				printf("Comanda moviment continuament pel motor" , MOTOR_2_AX_12);
				moure_continuament(MOTOR_2_AX_12, velocitat);
				break;
			case Sw1:
				printf("Boton Sw1 ('a') apretado\n");
				printf("Comanda moviment enrrera a velocitat" , velocitat, " pels motors" , MOTOR_2_AX_12, "amb direcci�", direccio);
				canviar_velocitat(MOTOR_2_AX_12, velocitat, direccio);
				break;
			case Sw2:
				printf("Boton Sw2 ('s') apretado\n");
				printf("Comanda moviment continuament pel motor" , SENSOR_AX_S1);
				distance_wall_front(SENSOR_AX_S1);
				break;
			case Quit:
				printf("Adios!\n");
				break;
			//etc, etc...
			}
			fflush(stdout);
		}
	}
	printf("Programa terminado\n");
	//Signal the emulation thread to stop
	pthread_kill(tid, SIGTERM);
	pthread_kill(jid, SIGTERM);

}