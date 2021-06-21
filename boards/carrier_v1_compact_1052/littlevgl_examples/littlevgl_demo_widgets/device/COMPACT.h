/*********************************************************************//**

	@file COMPACT.h

	@brief 

	@warning 
			
	@author Simplia
	@date 8 feb. 2021 Base version

  @copyright
		This software is property of Simplia. Its reproduction,
		total or  partial, by any means, in any form, permanent or temporary,
		is forbidden unless explicitly authorized by Simplia.
  @copyright
		Any adaptation, amendment, translation or transformation, as well as
		the decompiling or to disassemble of this software  product, shall only
		be performed with the explicit authorization of Simplia.
		The user of the present software product shall be allowed  to make a
		backup copy  as long as it is necessary  for the utilization  of the
		same.
  @copyright
		The terms stated above shall be understood  affecting that stated in
		applicable Laws.

*************************************************************************/
#ifndef COMPACT_H_
#define COMPACT_H_


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

#define COMPACT_1052_BOARD_NAME											"COMPACT-1052"

#define COMPACT_1052_USER_LED_GPIO										GPIO5
#define COMPACT_1052_USER_LED_PIN										(1U)

#define COMPACT_1052_DISP_GPIO											(GPIO2)
#define COMPACT_1052_DISP_PIN											(31U)

#define COMPACT_1052_UART_TYPE     										kSerialPort_Uart
#define COMPACT_1052_UART_BASEADDR 										(uint32_t) LPUART1
#define COMPACT_1052_UART_INSTANCE 										(1U)

#ifndef COMPACT_1052_UART_BAUDRATE
#define COMPACT_1052_UART_BAUDRATE 										(115200U)
#endif /* BOARD_DEBUG_UART_BAUDRATE */

#define COMPACT_1052_USB_PHY_D_CAL 										(0x0CU)
#define COMPACT_1052_USB_PHY_TXCAL45DP 									(0x06U)
#define COMPACT_1052_USB_PHY_TXCAL45DM 									(0x06U)

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

#endif /* COMPACT_H_ */
