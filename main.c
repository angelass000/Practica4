#include <pthread.h>
#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

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

uint8_t dreta=0x01; //Sentit dret de la roda
uint8_t esquerra=0x00; //Sentit esquerra de la roda

uint8_t estado = Ninguno, estado_anterior = Ninguno, finalizar = 0;
/**
 * main.c
 */
int main(void)
{
	pthread_t tid, jid;
	uint8_t tmp;

	//Init semaphores for TX data handshake
	//Inicialitzen el objecte semàfor amb un valor 0(en "vermell") per poder-lo fer servir després.
	sem_init(&sem_tx_msp, 0, 0);
	sem_init(&sem_tx_cli, 0, 0);

	//Init queue for RX data
	init_queue();

	//Start thread for dynamixel module emulation
	//Crea un thread separat que executarà la funcio dyn_emu.
	//Els altres arguments no són rellevant en el nostre cas
	pthread_create(&tid, NULL, dyn_emu, (void*) &tid);
	pthread_create(&jid, NULL, joystick_emu, (void*) &jid);

	//Testing some high level function
	/*printf("MAIN: Setting LED to 0 \n");
    dyn_led_control(1, 0);
    printf("MAIN: Getting LED value \n");
    dyn_led_read(1, &tmp);
    assert(tmp == 0);
    printf("MAIN: Setting LED to 1 \n");
    //Aquí s'atura
	dyn_led_control(1, 1);
	printf("MAIN: Getting LED value \n");
    dyn_led_read(1, &tmp);
    assert(tmp == 1);*/

	printf("************************\n");
	printf("Test passed successfully\n");
	printf("Pulsar 'q' para terminar, qualquier tecla para seguir\r");
	fflush(stdout);//	return 0;

	while(estado != Quit)
	{
		uint16_t velocitat=0x0F;
		int sensor = 0;
		uint8_t rodaDreta = 0x01;
		uint8_t rodaEsquerra = 0x02;
		Get_estado(&estado, &estado_anterior);
		if(estado != estado_anterior){
			Set_estado_anterior(estado);
			printf("estado = %d\n", estado);
			fflush(stdout);
			switch(estado){
			case Up:
                printf("Boton Up ('i') apretado\n");
                printf("Comanda moviment endavant a velocitat " );
                printf("%" PRIu16 , velocitat);
                printf(" pels motors %");
                printf("%" PRIu8 , MOTOR_2_AX_12);
                printf(" i ");
                printf("%" PRIu8 , MOTOR_3_AX_12);
                printf("\n");
                move_foward(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
                printf("Sortida esperada: \n");
                printf("Moven roda 2 a velocitat 15 i amb direccio 0 \n");
                printf("Moven roda 1 a velocitat 15 i amb direcci0 4");
				//Imprimirem la direcció i el primer print serà la 0, ja que es tracta de la roda esquerra
				// Al segon la direcció serà 1. La velocitat serà la mateixa en tots dos de la mateixa manera que l'ID dels motors.
				break;
			case Down:
				printf("Boton Down ('m') apretado\n");
				printf("Comanda moviment enrrera a velocitat " );
				printf("%" PRIu16 , velocitat);
				printf(" pels motors %");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				printf(" i ");
				printf("%" PRIu8 , MOTOR_3_AX_12);
				printf("\n");
				move_backward(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
				printf("Sortida esperada:\n");
				printf("Moven roda 2 a velocitat 15 i amb direccio 4 \n");
				printf("Moven roda 1 a velocitat 15 i amb direccio 0");
				break;
			case Left:
				printf("Boton Left ('j') apretado\n");
				printf("Comanda moviment esquerra a velocitat");
				printf("Comanda moviment esqierra a velocitat " );
				printf("%" PRIu16 , velocitat);
				printf(" pels motors %");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				printf(" i ");
				printf("%" PRIu8 , MOTOR_3_AX_12);
				printf("\n");
				move_left(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
				printf("Sortida esperada: \n");
				printf("Moven roda 2 a velocitat 15 i amb direccio 0 \n");
				printf("Moven roda 1 a velocitat 20 i amb direccio 4");
				break;
			case Right:
				printf("Boton Right ('l') apretado\n");
				printf("Comanda moviment dreta a velocitat " );
				printf("%" PRIu16 , velocitat);
				printf(" pels motors %");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				printf(" i ");
				printf("%" PRIu8 , MOTOR_3_AX_12);
				printf("\n");
				move_right(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);
				printf("Sortida esperada: \n");
				printf("Moven roda 2 a velocitat 20 i amb direccio 0 \n");
				printf("Moven roda 1 a velocitat 15 i amb direccio 4");
				break;
			case Center:
				printf("Boton Center ('k') apretado\n");
				printf("Comanda moviment continuament pel motor");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				moure_continuament(MOTOR_2_AX_12);
				printf("Sortida esperada: \n");
				printf("Moure roda 2 continuament");
				break;
			case Sw1:
				printf("Boton Sw1 ('a') apretado\n");
				printf("Comanda moviment enrrera a velocitat " );
				printf("%" PRIu16 , velocitat);
				printf(" pel motors %");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				printf("amb direcció dreta");
				printf("%" PRIu8 , dreta);
				canviar_velocitat(MOTOR_2_AX_12, velocitat, dreta);
				printf("Sortida esperada: \n");
				printf("Canvi de velocitat per la roda 2 a velocitat 15 i amb direccio 4"); //direccio 4 dreta, direccio 0 esquerra
				break;
			case Sw2:
				printf("Boton Sw2 ('s') apretado\n");
				printf("Comanda moviment continuament pel motor ");
				printf("%" PRIu8, SENSOR_AX_S1);
				printf("\n");
				distance_wall_front(SENSOR_AX_S1);

				break;
			case Quit:
				printf("Adios!\n");
				break;
			}
			fflush(stdout);
		}
	}
	printf("Programa terminado\n");
	//Signal the emulation thread to stop
	pthread_kill(tid, SIGTERM);
	pthread_kill(jid, SIGTERM);

}
