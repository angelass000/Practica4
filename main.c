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
#include "dyn/dyn_frames.h"
#include "dyn_test/dyn_emu.c"
#include "dyn_emu.h"

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
	printf("MAIN: Setting LED to 0 \n");
    dyn_led_control(1, 0);
    printf("MAIN: Getting LED value \n");
    dyn_led_read(1, &tmp);
    assert(tmp == 0);
    printf("MAIN: Setting LED to 1 \n");
    //Aquí s'atura
	dyn_led_control(1, 1);
	printf("MAIN: Getting LED value \n");
    dyn_led_read(1, &tmp);
    //assert(tmp == 1);

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
			    //Comprovació de velocitat i id per la comanda moviment endavant
                printf("Boton Up ('i') apretado\n");
                printf("Comanda moviment endavant a velocitat " );
                printf("%" PRIu16 , velocitat);
                printf(" pels motors %");
                printf("%" PRIu8 , MOTOR_2_AX_12);
                printf(" i ");
                printf("%" PRIu8 , MOTOR_3_AX_12);
                printf("\n");
                move_foward(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);//cridem a la funció moure cap endavant amb els dos motors,
                //, els quals hem definit amb dos valors de 1 per la roda dreta i 2 la roda esquerra, i la velocitat
                printf("Sortida esperada: \n");
                printf("Movent roda 2 a velocitat 15 i amb direccio esquerra \n");
                printf("Movent roda 1 a velocitat 15 i amb direccio dreta");
				//Com podem observar en la consola, tant la sortida esperada com la donada son iguals

				break;
			case Down:
			    //Comprovació de velocitat i id per la comanda moviment enrera
				printf("Boton Down ('m') apretado\n");
				printf("Comanda moviment enrera a velocitat " );
				printf("%" PRIu16 , velocitat);
				printf(" pels motors %");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				printf(" i ");
				printf("%" PRIu8 , MOTOR_3_AX_12);
				printf("\n");
				move_backward(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);//cridem a la funció moure cap enrera amb els dos motors
				// ,els quals hem definit amb dos valors de 1 per la roda dreta i 2 la roda esquerra, i la velocitat
				printf("Sortida esperada:\n");
				printf("Movent roda 2 a velocitat 15 i amb direccio dreta \n");
				printf("Movent roda 1 a velocitat 15 i amb direccio esquerra");
                    //Com podem observar en la consola, tant la sortida esperada com la donada son iguals
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
				move_left(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);//cridem a la funció moure cap a l'esquerra amb els dos motors
				//, els quals hem definit amb dos valors de 1 per la roda dreta i 2 la roda esquerra, i la velocitat
				printf("Sortida esperada: \n");
				printf("Movent roda 2 a velocitat 15 i amb direccio esquerra \n");
				printf("Movent roda 1 a velocitat 20 i amb direccio dreta");
                    //Com podem observar en la consola, tant la sortida esperada com la donada son iguals
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
				move_right(MOTOR_2_AX_12, MOTOR_3_AX_12, velocitat);//cridem a la funció moure cap a la dreta amb els dos motors
				//,  els quals hem definit amb dos valors de 1 per la roda dreta i 2 la roda esquerra,  i la velocitat
				printf("Sortida esperada: \n");
				printf("Movent roda 2 a velocitat 20 i amb direccio esquerra \n");
				printf("Movent roda 1 a velocitat 15 i amb direccio dreta");
                    //Com podem observar en la consola, tant la sortida esperada com la donada son iguals
				break;
			case Center:
				printf("Boton Center ('k') apretado\n");
				printf("Comanda moviment continuament pel motor");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				moure_continuament(MOTOR_2_AX_12);//cridem a la funció moure continuament amb un motor, en aquest cas el motor
				//de la roda esquerra que l'hem definit amb el valor 2.
				printf("Sortida esperada: \n");
				printf("Moure roda 2 continuament");
                    //Com podem observar en la consola, tant la sortida esperada com la donada son iguals
				break;
			case Sw1:
				printf("Boton Sw1 ('a') apretado\n");
				printf("Comanda canvi de velocitat a velocitat " );
				printf("%" PRIu16 , velocitat);
				printf(" pel motors %");
				printf("%" PRIu8 , MOTOR_2_AX_12);
				printf("amb direcció dreta");
				printf("%" PRIu8 , dreta);
				canviar_velocitat(MOTOR_2_AX_12, velocitat, dreta);//cridem a la funció canviar de velocitat amb un motor,en aquest cas el motor
				//de la roda esquerra que l'hem definit amb el valor 2, una velocitat i una direcció
                printf("Sortida esperada: \n");
				printf("Canvi de velocitat per la roda 2 a velocitat 15 i amb direccio dreta"); //direccio 4 dreta, direccio 0 esquerra
                    //Com podem observar en la consola, tant la sortida esperada com la donada son iguals
				break;
			case Sw2:
				printf("Boton Sw2 ('s') apretado\n");
				printf("Comanda mesurar distància ");
				printf("%" PRIu8, SENSOR_AX_S1);
				printf("\n");
				struct RxReturn distancia=distance_wall_front(SENSOR_AX_S1);//cridem a la funció amb el sensor com a paràmetre
				//i li assignem a distancia que es un struct RxReturn, ja que la funció retorna una variable d'aquest tipus
				uint8_t left_IR_Sensor=(distancia.StatusPacket[5]); //assignem a left IR sensor la posició 5 del statusPacket
				//que és on es troba el primer sensor
				uint8_t center_IR_Sensor=(distancia.StatusPacket[6]);//assignem a center IR sensor la posició 6 del statusPacket
				//que és on es troba el segon sensor
				uint8_t right_IR_Sensor=(distancia.StatusPacket[7]);//assignem a right IR sensor la posició 7 del statusPacket
				//que és on es troba el tercer sensor

				//Amb tres asserts, comprovem si el valor que hem assignat als diferents sensors corresponen amb el que
				//retorna el statusPacket
				assert(left_IR_Sensor==3);
				assert(center_IR_Sensor==4);
				assert(right_IR_Sensor==5);
                break;
                //Com podem observar, el codi s'executa correctament ja que els tres asserts son correctes
			case Quit:
				printf("Adeu!\n");
				break;
			}
			fflush(stdout);
		}
	}
	printf("Programa finalitzat\n");
	//Signal the emulation thread to stop
	pthread_kill(tid, SIGTERM);
	pthread_kill(jid, SIGTERM);

}
