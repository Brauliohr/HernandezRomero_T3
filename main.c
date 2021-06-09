/* "Diseño con Microcontroladores"
 * Tarea 3.
 * Autor: Braulio Hernández Romero
 * Fecha de entrega: 08/06/2021
 */

#include <stdio.h>
#include <stdint.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "rgb.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"

uint8_t switch2 = 0;
uint8_t switch3 = 0;

gpio_pin_config_t sw_config = {
	kGPIO_DigitalInput,
	0,
};
gpio_pin_config_t led_config = {
    kGPIO_DigitalOutput,
    1,
};

#define PIN06  6u						//Macro para el pin del Switch 2
#define PIN04  4u						//Macro para el pin del Switch 3
#define PIN22  22u                      //Macro para el pin del LED Rojo  R
#define PIN26  26u                      //Macro para el pin del led Verde G
#define PIN21  21u                      //Macro para el pin del led Azul  B

#define ONE   (0x01u)
#define TWO   (0x02u)

typedef enum{
	Inicial,
	EDO1,
	EDO2,
	EDO3,
}State_name_t;

void PORTC_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOC, 1u << PIN06);

	switch2 = true;
	SDK_ISR_EXIT_BARRIER;
}

void PORTA_IRQHandler(void)
{
	GPIO_PortClearInterruptFlags(GPIOA, 1u << PIN04);

	switch3 = true;
	SDK_ISR_EXIT_BARRIER;
}


int main(void){

	//CONFIGURACIÓN PARA LOS PINES DE SW2 Y SW3

	const port_pin_config_t porta_pin4_config = {
	  kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
	  kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
	  kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
	  kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
	  kPORT_HighDriveStrength,                                 /* High drive strength is configured */
	  kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
	  kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};

	/*Configuraciones del PIT*/
	/* Structure of initialize PIT */
	pit_config_t pitConfig;
	uint32_t freq = 0;

	/* Board pin, clock, debug console init */
	BOARD_InitPins();
	BOARD_InitDebugConsole();

	/*
	 * pitConfig.enableRunInDebug = false;
	 */
	PIT_GetDefaultConfig(&pitConfig);

	/* Init pit module */
	PIT_Init(PIT, &pitConfig);

	/* Set timer period for channel 0 */
	freq = CLOCK_GetFreq(kCLOCK_BusClk);

	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000000U, freq));

	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(PIT0_IRQn);


	/*Start channel 0*/
	PIT_StartTimer(PIT, kPIT_Chnl_0);

	/*Habilitar los relojes para los puertos a utilizar*/
	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortE);

	PORT_SetPinConfig(PORTC, PIN06, &porta_pin4_config);
	PORT_SetPinConfig(PORTA, PIN04, &porta_pin4_config);

	PORT_SetPinInterruptConfig(PORTC, PIN06, kPORT_InterruptFallingEdge);
	PORT_SetPinInterruptConfig(PORTA, PIN04, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOC, PIN06, &sw_config);
	GPIO_PinInit(GPIOA, PIN04, &sw_config);

	NVIC_EnableIRQ(PORTC_IRQn);
	/**It Sets the priority of the IRQ*/
	NVIC_SetPriority(PORTC_IRQn, 2);

	NVIC_EnableIRQ(PORTA_IRQn);
	/**It Sets the priority of the IRQ*/
	NVIC_SetPriority(PORTA_IRQn, 2);

	//CONFIGURACIÓN PARA LOS PINES DEL LED RGB

	PORT_SetPinMux(PORTB, PIN22, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTB, PIN21, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, PIN26, kPORT_MuxAsGpio);

	GPIO_PinInit(GPIOB, PIN22, &led_config);
	GPIO_PinInit(GPIOB, PIN21, &led_config);
	GPIO_PinInit(GPIOE, PIN26, &led_config);

	uint8_t input_port_a, input_port_c, total_input;

	State_name_t current_state = Inicial;

	while(1){

		input_port_a = switch3;
		input_port_c = switch2;

		input_port_a = input_port_a << 1;

		total_input = input_port_a | input_port_c;

		switch2 = false;
		switch3 = false;

		switch (current_state) {

		      case Inicial:

		    	amarillo();
		    	rojo();
		    	morado();

		    	if(ONE == total_input)
		    	{
		    	  current_state = EDO1 ;
		    	}
		    	else
		    	{
		    	  current_state = Inicial;
		    	}
		    	break;

		      case EDO1:

		      	verde();
		  		azul();
		  		rojo();

		  		if(TWO == total_input)
		  		{
		  			current_state = EDO2 ;
		  		}
		  		else
		  		{
		  			current_state = EDO1;
		  		}
		  		break;

		  	 case EDO2:
		  		azul();
		  		blanco();
		  		rojo();

		  		if(TWO == total_input)
		  		{
		  			current_state = EDO3 ;
		  		}
		  		else
		  		{
		  			current_state = EDO2;
		  		}
		  		break;

		      case EDO3:
		  		verde();
		  		morado();
		  		amarillo();

		  		if(ONE == total_input)
		  		{
		  			current_state = Inicial ;
		  		}
		  		else
		  		{
		  			current_state = EDO3;
		  		}
		  		break;

		  	default:
		  		current_state = Inicial;
		  		break;
		    }

	}

	return 0;
}
