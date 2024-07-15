/*----------------------------------------------------------------------------*/
/* Copyright 2018-2021 NXP                                                    */
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

#ifndef BOARD_K82F_PNEV5190B_H_
#define BOARD_K82F_PNEV5190B_H_

#include "MK82F25615.h"

#define GPIO_PORT_A         0    /* Same macro for GPIOA/PORTA */
#define GPIO_PORT_B         1    /* Same macro for GPIOB/PORTB */
#define GPIO_PORT_C         2    /* Same macro for GPIOC/PORTC */
#define GPIO_PORT_D         3    /* Same macro for GPIOD/PORTD */
#define GPIO_PORT_E         4    /* Same macro for GPIOE/PORTE */

/******************************************************************
 * Board Pin/Gpio configurations
 ******************************************************************/
/* Pin configuration format : Its a 32 bit format where every byte represents a field as shown below.
 * | Byte3 | Byte2 | Byte1      | Byte0 |
 * |  --   |  --   | GPIO/PORT  | PIN   |
 * */
#define PHDRIVER_PIN_RESET  ((GPIO_PORT_B << 8) | 9)   /**< Reset pin, Pin9, GPIOB, PORTB */
#define PHDRIVER_PIN_IRQ    ((GPIO_PORT_B << 8) | 22)  /**< IRQ pin, Pin22, GPIOB, PORTB */
/* For 5190 busy is same as IRQ */
#define PHDRIVER_PIN_BUSY    ((GPIO_PORT_B << 8) | 22)  /**< IRQ pin, Pin22, GPIOB, PORTB */
//#define PHDRIVER_PIN_WKUP   ((GPIO_PORT_C << 8) | 7)   /**< WakeUp Request pin, Pin7, GPIOC, PORTC */
//#define PHDRIVER_PIN_DWL    ((GPIO_PORT_C << 8) | 13)  /**< Download pin, Pin13, GPIOC, PORTC */
#define PHDRIVER_PIN_DWL    ((GPIO_PORT_B << 8) | 0)  /**< Download pin, Pin0, GPIOB, PORTB */
#define PHDRIVER_PIN_CLK    ((GPIO_PORT_B << 8) | 21)  /**< CLK Request pin, Pin21, GPIOB, PORTB */
#define PHDRIVER_PIN_SSEL     ((GPIO_PORT_B << 8) | 10)  /**< SSEL pin, Pin10, GPIOB, PORTB */
#define PHDRIVER_PIN_WKUP   ((GPIO_PORT_B << 8) | 1)   /**< WakeUp Request pin, Pin1, GPIOB, PORTB */

#define PHDRIVER_PIN_RLED   ((GPIO_PORT_C << 8) | 7)   /**< RED LED, Pin7, GPIOC, PORTC */
#define PHDRIVER_PIN_GLED   ((GPIO_PORT_C << 8) | 8)   /**< GREEN LED, Pin8, GPIOC, PORTC */
#define PHDRIVER_PIN_BLED   ((GPIO_PORT_C << 8) | 13)  /**< BLUE LED, Pin13, GPIOC, PORTC */
#define PHDRIVER_PIN_OLED   ((GPIO_PORT_D << 8) | 10)  /**< ORANGE LED, Pin10, GPIOD, PORTD */

#define PHDRIVER_PIN_GPIO1  ((GPIO_PORT_D << 8) | 12)  /**< GPIO1, Port D, Pin12 (TP8)*/
#define PHDRIVER_PIN_GPIO2  ((GPIO_PORT_D << 8) | 13)  /**< GPIO2, Port D, Pin13 (TP6)*/
#define PHDRIVER_PIN_GPIO3  ((GPIO_PORT_D << 8) | 14)  /**< GPIO3, Port D, Pin14 (TP7)*/
#define PHDRIVER_PIN_GPIO4  ((GPIO_PORT_D << 8) | 15)  /**< GPIO4, Port D, Pin15 (TP5)*/

/* These pins are used for EMVCo Interoperability test status indication,
 * not for the generic Reader Library implementation.
 */
#define PHDRIVER_PIN_SUCCESS   ((GPIO_PORT_D << 8) | 12)   /**< GPIO, Port D, Pin12 */
#define PHDRIVER_PIN_FAIL      ((GPIO_PORT_D << 8) | 13)   /**< GPIO, Port D, Pin13 */

/* GPIO and LED for applications use */
#define PHDRIVER_PIN_GPIO      PHDRIVER_PIN_GPIO1   /**< GPIO1, Port D, Pin12 (TP8)*/
#define PHDRIVER_PIN_LED       PHDRIVER_PIN_RLED    /**< RED LED */

/******************************************************************
 * PIN Pull-Up/Pull-Down configurations.
 ******************************************************************/
#define PHDRIVER_PIN_RESET_PULL_CFG    PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_IRQ_PULL_CFG      PH_DRIVER_PULL_DOWN
#define PHDRIVER_PIN_WKUP_PULL_CFG     PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_CLK_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_DWL_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_NSS_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_BUSY_PULL_CFG     PH_DRIVER_PULL_UP

/******************************************************************
 * IRQ PIN NVIC settings
 ******************************************************************/
#define EINT_IRQn                 PORTB_IRQn
#define EINT_PRIORITY             8
#define PIN_IRQ_TRIGGER_TYPE      PH_DRIVER_INTERRUPT_RISINGEDGE
#ifdef PHFL_WITH_NXP_NFC_COCKPIT
/* Nfc cockpit needs Irq Handler. */
#   define CLIF_IRQHandler_NxpNfcCockpit PORTB_IRQHandler
#else
#   define CLIF_IRQHandler               PORTB_IRQHandler
#endif

/*****************************************************************
 * Front End Reset logic level settings
 ****************************************************************/
#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */
#define RESET_POWERDOWN_LEVEL PH_DRIVER_SET_LOW
#define RESET_POWERUP_LEVEL   PH_DRIVER_SET_HIGH

/*****************************************************************
 * SPI Configuration
 ****************************************************************/

#define PHDRIVER_KSDK_SPI_POLLING          /* Enabling polling method helps while performing SPI transfer from ISR. */
#define PHDRIVER_KSDK_SPI_MASTER           DSPI1
#define PHDRIVER_KSDK_SPI_DATA_RATE        5000000U
#define PHDRIVER_KSDK_SPI_CLK_SRC          DSPI1_CLK_SRC
#define PHDRIVER_KSDK_SPI_IRQ              SPI1_IRQn

/* SPI interrupt PRIO is greater than RF/Timer prio, as SPI transaction is performed from RF ISR.
 * The lower the value, the greater the priority of the corresponding interrupt.
 * */
#define DSPI_IRQ_PRIORITY     7

#define ENABLE_PORT_SSP_1     kCLOCK_PortB     /* SPI_SCK */
#define PORT_SSP_1            PORTB
#define FIRST_PINNUM_SSP      11               /* PTB11 */

#define ENABLE_PORT_SSP_2     kCLOCK_PortB     /* SPI_SOUT, SPI_MOSI */
#define PORT_SSP_2            PORTB
#define SECOND_PINNUM_SSP     16               /* PTB16 */

#define ENABLE_PORT_SSP_3     kCLOCK_PortB     /* SPI_SIN, SPI_MISO */
#define PORT_SSP_3            PORTB
#define THIRD_PINNUM_SSP      17               /* PTB17 */

/*****************************************************************
 * Timer Configuration
 ****************************************************************/
#define PH_DRIVER_KSDK_PIT_TIMER          PIT
#define PH_DRIVER_KSDK_PIT_CLK            kCLOCK_BusClk
#define PH_DRIVER_KSDK_TIMER_CHANNEL      kPIT_Chnl_0    /**< PIT channel number 0 */
#define PH_DRIVER_KSDK_TIMER_NVIC         PIT0CH0_IRQn
#define PH_DRIVER_KSDK_TIMER_PRIORITY     8              /**< Same as RF prio. */

#define PIT0_IRQHandler                   PIT0CH0_IRQHandler

#endif /* BOARD_K82F_PNEV5190B_H_ */
