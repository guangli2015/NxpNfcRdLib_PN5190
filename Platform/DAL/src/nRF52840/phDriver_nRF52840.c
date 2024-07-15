/*----------------------------------------------------------------------------*/
/* Copyright 2017-2022 NXP                                                    */
/*                                                                            */
/* NXP Confidential. This software is owned or controlled by NXP and may only */
/* be used strictly in accordance with the applicable license terms.          */
/* By expressly accepting such terms or by downloading, installing,           */
/* activating and/or otherwise using the software, you are agreeing that you  */
/* have read, and that you agree to comply with and are bound by, such        */
/* license terms. If you do not agree to be bound by the applicable license   */
/* terms, then you may not retain, install, activate or otherwise use the     */
/* software.                                                                  */
/*----------------------------------------------------------------------------*/

/** \file
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
*/

/* *****************************************************************************************************************
 * Includes
 * ***************************************************************************************************************** */

#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "phDriver.h"
#include "BoardSelection.h"

LOG_MODULE_DECLARE(pn5190);


/* *****************************************************************************************************************
 * Internal Definitions
 * ***************************************************************************************************************** */


/* *****************************************************************************************************************
 * Type Definitions
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Global and Static Variables
 * Total Size: NNNbytes
 * ***************************************************************************************************************** */


static pphDriver_TimerCallBck_t pPitTimerCallBack;
static volatile uint8_t dwTimerExp;

/* *****************************************************************************************************************
 * Private Functions Prototypes
 * ***************************************************************************************************************** */


void my_timer_handler(struct k_timer *dummy)
{
      if(NULL != pPitTimerCallBack)
    	{
    		pPitTimerCallBack();
    	}
}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

/* *****************************************************************************************************************
 * Public Functions
 * ***************************************************************************************************************** */
phStatus_t phDriver_TimerStart(phDriver_Timer_Unit_t eTimerUnit, uint32_t dwTimePeriod, pphDriver_TimerCallBck_t pTimerCallBack)
{

  

	pPitTimerCallBack = pTimerCallBack;
	
	if(PH_DRIVER_TIMER_SECS == eTimerUnit)
   	{
   	     k_timer_start(&my_timer, K_SEC(dwTimePeriod), K_NO_WAIT);
   	}
   else if(PH_DRIVER_TIMER_MILLI_SECS == eTimerUnit)
   	{
   		
		 k_timer_start(&my_timer, K_MSEC(dwTimePeriod), K_NO_WAIT);
   	}
   else if(PH_DRIVER_TIMER_MICRO_SECS == eTimerUnit)
   	{
   		k_timer_start(&my_timer, K_USEC(dwTimePeriod), K_NO_WAIT);
   	}

   	if(NULL == pTimerCallBack)
    	{
    		k_timer_status_sync(&my_timer);
    	}
    return PH_DRIVER_SUCCESS;
}

phStatus_t phDriver_TimerStop(void)
{
	k_timer_stop(&my_timer);


    return PH_DRIVER_SUCCESS;
}

phStatus_t phDriver_PinConfig(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc, phDriver_Pin_Config_t *pPinConfig)
{
    //gpio_pin_config_t sGpioConfig;
    uint8_t bPinNum;
    uint8_t bPortGpio;
	struct device *dev_gpioX;

    //port_interrupt_t eInterruptType;
    /*port_pin_config_t sPinConfig =
    {
        .pullSelect = kPORT_PullDisable,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainDisable,
        .driveStrength = kPORT_HighDriveStrength,
        .mux = kPORT_MuxAsGpio,
        .lockRegister = kPORT_UnlockRegister
    };*/

    if((ePinFunc == PH_DRIVER_PINFUNC_BIDIR) || (pPinConfig == NULL))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    /* Extract the Pin, Gpio, Port details from dwPinNumber */
	if(dwPinNumber<32)
		{
    		bPinNum = (uint8_t)(dwPinNumber);
    		bPortGpio = (uint8_t)(0);
		}
	else if(dwPinNumber>=32)
		{
			bPinNum = (uint8_t)(dwPinNumber-32);
    		bPortGpio = (uint8_t)(1);
		}

   /* sGpioConfig.pinDirection = (ePinFunc == PH_DRIVER_PINFUNC_OUTPUT)?GPIO_OUTPUT:GPIO_INPUT;
    sGpioConfig.outputLogic  =  pPinConfig->bOutputLogic;
    sPinConfig.pullSelect = (pPinConfig->bPullSelect == PH_DRIVER_PULL_DOWN)? kPORT_PullDown : kPORT_PullUp;

    CLOCK_EnableClock(pPortsClock[bPortGpio]);
    PORT_SetPinConfig((PORT_Type *)pPortsBaseAddr[bPortGpio], bPinNum, &sPinConfig);*/
	if(bPortGpio == 0)
		{
			//获取GPIO Port的句柄
			dev_gpioX  = DEVICE_DT_GET(DT_NODELABEL(gpio0));
		}
	else if(bPortGpio == 1)
		{
			//获取GPIO Port的句柄
			dev_gpioX = DEVICE_DT_GET(DT_NODELABEL(gpio1));
		}

	switch( ePinFunc )
		{
		    case PH_DRIVER_PINFUNC_OUTPUT:
				gpio_pin_configure(dev_gpioX, bPinNum, GPIO_OUTPUT);
				gpio_pin_set(dev_gpioX, bPinNum, pPinConfig->bOutputLogic);
				break;
			case PH_DRIVER_PINFUNC_INPUT:
			gpio_pin_configure(dev_gpioX, bPinNum, GPIO_INPUT);
				break;
		}


    if(ePinFunc == PH_DRIVER_PINFUNC_INTERRUPT)
    {
        /*eInterruptType = aInterruptTypes[(uint8_t)pPinConfig->eInterruptConfig];
        GPIO_PortClearInterruptFlags((GPIO_Type *)pGpiosBaseAddr[bPortGpio], bPinNum);
        PORT_SetPinInterruptConfig((PORT_Type *)pPortsBaseAddr[bPortGpio], bPinNum, eInterruptType);*/
        gpio_pin_configure(dev_gpioX, bPinNum ,GPIO_INPUT);
		 gpio_pin_interrupt_configure(dev_gpioX,bPinNum,GPIO_INT_EDGE_TO_ACTIVE);
    }

   // GPIO_PinInit((GPIO_Type *)pGpiosBaseAddr[bPortGpio], bPinNum, &sGpioConfig);

    return PH_DRIVER_SUCCESS;
}

uint8_t phDriver_PinRead(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc)
{
    uint8_t bValue;
    uint8_t bPinNum;
    uint8_t bPortGpio;

	int value = 0;
	struct device *dev_gpioX;

    /* Extract the Pin, Gpio details from dwPinNumber */
   // bPinNum = (uint8_t)(dwPinNumber & 0xFF);
   // bGpioNum = (uint8_t)((dwPinNumber & 0xFF00)>>8);
   
	/* Extract the Pin, Gpio, Port details from dwPinNumber */
		if(dwPinNumber<32)
			{
				bPinNum = (uint8_t)(dwPinNumber);
				bPortGpio = (uint8_t)(0);
			}
		else if(dwPinNumber>=32)
			{
				bPinNum = (uint8_t)(dwPinNumber-32);
				bPortGpio = (uint8_t)(1);
			}

		if(bPortGpio == 0)
		{
			//获取GPIO Port的句柄
			dev_gpioX  = DEVICE_DT_GET(DT_NODELABEL(gpio0));
		}
	else if(bPortGpio == 1)
		{
			//获取GPIO Port的句柄
			dev_gpioX = DEVICE_DT_GET(DT_NODELABEL(gpio1));
		}

		value = gpio_pin_get(dev_gpioX, bPinNum);
		value = (value < 0) ? 0 : value;
	
        bValue = value;  

    return bValue;
}

phStatus_t phDriver_IRQPinPoll(uint32_t dwPinNumber, phDriver_Pin_Func_t ePinFunc, phDriver_Interrupt_Config_t eInterruptType)
{
    uint8_t    bGpioState = 0;

    if ((eInterruptType != PH_DRIVER_INTERRUPT_RISINGEDGE) && (eInterruptType != PH_DRIVER_INTERRUPT_FALLINGEDGE))
    {
        return PH_DRIVER_ERROR | PH_COMP_DRIVER;
    }

    if (eInterruptType == PH_DRIVER_INTERRUPT_FALLINGEDGE)
    {
        bGpioState = 1;
    }

	while(phDriver_PinRead(dwPinNumber, ePinFunc) == bGpioState);

    return PH_DRIVER_SUCCESS;
}

void phDriver_PinWrite(uint32_t dwPinNumber, uint8_t bValue)
{
    uint8_t bPinNum;
    uint8_t bPortGpio;


	struct device *dev_gpioX;
	

	   
		/* Extract the Pin, Gpio, Port details from dwPinNumber */
			if(dwPinNumber<32)
				{
					bPinNum = (uint8_t)(dwPinNumber);
					bPortGpio = (uint8_t)(0);
				}
			else if(dwPinNumber>=32)
				{
					bPinNum = (uint8_t)(dwPinNumber-32);
					bPortGpio = (uint8_t)(1);
				}
	
			if(bPortGpio == 0)
			{
				//获取GPIO Port的句柄
				dev_gpioX  = DEVICE_DT_GET(DT_NODELABEL(gpio0));
			}
		else if(bPortGpio == 1)
			{
				//获取GPIO Port的句柄
				dev_gpioX = DEVICE_DT_GET(DT_NODELABEL(gpio1));
			}
	
			gpio_pin_set(dev_gpioX, bPinNum, bValue);

}

void phDriver_PinClearIntStatus(uint32_t dwPinNumber)
{

}

phStatus_t phDriver_IRQPinRead(uint32_t dwPinNumber)
{
	phStatus_t bGpioVal = false;

	bGpioVal = phDriver_PinRead(dwPinNumber, PH_DRIVER_PINFUNC_INPUT);

	return bGpioVal;
}




void phDriver_EnterCriticalSection(void)
{
    __disable_irq();
}

void phDriver_ExitCriticalSection(void)
{
    __enable_irq();
}
