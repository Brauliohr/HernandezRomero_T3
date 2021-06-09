/*
 * pit.c
 *
 *  Created on: 8 jun. 2021
 *      Author: Braulio
 */
#include "pit.h"
#include <stdint.h>
#include <stdio.h>
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_pit.h"
#include "board.h"
#include "clock_config.h"


volatile bool pitIsrFlag = false;

void PIT0_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	pitIsrFlag = true;

	__DSB();
}

uint8_t PITflag_get_pit(void)
{
	return (pitIsrFlag);
}

void PITflag_set_pit(void)
{
	pitIsrFlag = false;
}



