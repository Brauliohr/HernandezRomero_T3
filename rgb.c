/*
 * rgb.c
 *
 *  Created on: 6 jun. 2021
 *      Author: Braulio
 */
#include "rgb.h"
#include <stdint.h>
#include <stdio.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_pit.h"
#include "pit.h"

#define PIN22       22u                      //Macro para el pin del LED Rojo  R
#define PIN26       26u                      //Macro para el pin del led Verde G
#define PIN21       21u                      //Macro para el pin del led Azul  B


void amarillo()
{
	if(PITflag_get_pit() == true)
	{
		GPIO_PortToggle(GPIOB, 1u << PIN22);
		GPIO_PortToggle(GPIOE, 1u << PIN26);
		PITflag_set_pit();
	}
}
void rojo()
{
	if(PITflag_get_pit() == true)
	{
		GPIO_PortToggle(GPIOB, 1u << PIN22);
		PITflag_set_pit();
	}
}
void morado()
{
	if(PITflag_get_pit() == true)
	{
		GPIO_PortToggle(GPIOB, 1u << PIN22);
		GPIO_PortToggle(GPIOB, 1u << PIN21);
		PITflag_set_pit();
	}
}
void verde()
{
	if(PITflag_get_pit() == true)
	{
		GPIO_PortToggle(GPIOE, 1u << PIN22);
		PITflag_set_pit();
	}

}
void azul()
{
	if(PITflag_get_pit() == true)
	{
		GPIO_PortToggle(GPIOB, 1u << PIN21);
		PITflag_set_pit();
	}
}
void blanco()
{
	if(PITflag_get_pit() == true)
	{
		GPIO_PortToggle(GPIOB, 1u << PIN22);
		GPIO_PortToggle(GPIOE, 1u << PIN26);
		GPIO_PortToggle(GPIOB, 1u << PIN21);
		PITflag_set_pit();
	}
}
