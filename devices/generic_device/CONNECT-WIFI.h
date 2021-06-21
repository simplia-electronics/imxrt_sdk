/*********************************************************************/

/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*************************************************************************/
#ifndef CONNECT_WIFI_H_
#define CONNECT_WIFI_H_


/************************************************************************
	INCLUDES
************************************************************************/

/* Board Configuration File */
#include "MIMXRT1052.h"
/* Standard includes */

/* Application includes */

/* Drivers includes. */
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
/* Library includes */

/************************************************************************
	CONFIGURATIONS
************************************************************************/

/************************************************************************
	DEFINES AND TYPES
************************************************************************/
/* Global Defines */

#define CONNECT_WIFI_1052_BOARD_NAME											"CONNECT-WIFI-1052"

#define CONNECT_WIFI_1052_USER_LED_GPIO											GPIO5
#define CONNECT_WIFI_1052_USER_LED_PIN											(1U)

#define CONNECT_WIFI_1052_DISP_GPIO												(GPIO2)
#define CONNECT_WIFI_1052_DISP_PIN												(31U)

#define CONNECT_WIFI_1052_UART_TYPE     										kSerialPort_Uart
#define CONNECT_WIFI_1052_UART_BASEADDR 										(uint32_t) LPUART1
#define CONNECT_WIFI_1052_UART_INSTANCE 										(1U)

#ifndef CONNECT_WIFI_1052_UART_BAUDRATE
#define CONNECT_WIFI_1052_UART_BAUDRATE 										(115200U)
#endif /* BOARD_DEBUG_UART_BAUDRATE */

#define CONNECT_WIFI_1052_USB_PHY_D_CAL 										(0x0CU)
#define CONNECT_WIFI_1052_USB_PHY_TXCAL45DP 									(0x06U)
#define CONNECT_WIFI_1052_USB_PHY_TXCAL45DM 									(0x06U)


#define CONNECT_WIFI_1052_FLASH_SIZE											(0x1000000U)

/*! @brief The ENET PHY address. */
#define CONNECT_WIFI_1052_ENET0_PHY_ADDRESS 									(0x01U) /* Phy address of enet port 0. */

/* Global Types */

/************************************************************************
	DECLARATIONS
************************************************************************/
/* Global Variables */

/* Public Functions */
/*********************************************************************//**
  See "COMPACT.c" for details of how to use this function.
 */

/************************************************************************
	FUNCTIONS
************************************************************************/

#endif /* CONNECT_WIFI_H_ */
