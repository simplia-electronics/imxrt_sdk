/*********************************************************************//**

	@file ft5426.h

	@brief 

	@warning 

	@author mperez			
	@author RBZ Embedded Logics
	@date 12 nov. 2020 Base version

  @copyright
		This software is property of RBZ Embedded Logics. Its reproduction,
		total or  partial, by any means, in any form, permanent or temporary,
		is forbidden unless explicitly authorized by RBZ Embedded Logics.
  @copyright
		Any adaptation, amendment, translation or transformation, as well as
		the decompiling or to disassemble of this software  product, shall only
		be performed with the explicit authorization of RBZ Embedded Logics.
		The user of the present software product shall be allowed  to make a
		backup copy  as long as it is necessary  for the utilization  of the
		same.
  @copyright
		The terms stated above shall be understood  affecting that stated in
		applicable Laws.

*************************************************************************/
#ifndef FT5426_H_
#define FT5426_H_


/************************************************************************
	INCLUDES
************************************************************************/

/* Board Configuration File */

/* Standard includes */

/* Application includes */

/* Drivers includes. */
#include "fsl_common.h"
/* Library includes */

/************************************************************************
	CONFIGURATIONS
************************************************************************/

/************************************************************************
	DEFINES AND TYPES
************************************************************************/
/* Global Defines */
/*! @brief FT5426 I2C address. */
#define FT5426_I2C_ADDRESS (0x38)

/*! @brief FT5426 maximum number of simultaneously detected touches. */
#define FT5426_MAX_TOUCHES (5U)

/*! @brief FT5426 register address where touch data begin. */
#define FT5426_TOUCH_DATA_SUBADDR (1)

/*! @brief FT5426 raw touch data length. */
#define FT5426_TOUCH_DATA_LEN (0x20)

/* Global Types */
typedef enum _touch_event
{
    kTouch_Down     = 0, /*!< The state changed to touched. */
    kTouch_Up       = 1, /*!< The state changed to not touched. */
    kTouch_Contact  = 2, /*!< There is a continuous touch being detected. */
    kTouch_Reserved = 3  /*!< No touch information available. */
} touch_event_t;

typedef struct _touch_point
{
    touch_event_t TOUCH_EVENT; /*!< Indicates the state or event of the touch point. */
    uint8_t TOUCH_ID; /*!< Id of the touch point. This numeric value stays constant between down and up event. */
    uint16_t TOUCH_X; /*!< X coordinate of the touch point */
    uint16_t TOUCH_Y; /*!< Y coordinate of the touch point */
} touch_point_t;

typedef struct _ft5426_handle
{
    LPI2C_Type *base;
    lpi2c_master_transfer_t xfer;
    uint8_t touch_buf[FT5426_TOUCH_DATA_LEN];
} ft5426_handle_t;



/************************************************************************
	DECLARATIONS
************************************************************************/
/* Global Variables */

/* Public Functions */

/*********************************************************************//**
  See "ft5426.c" for details of how to use this function.
 */
status_t FT5426_Init(ft5426_handle_t *handle, LPI2C_Type *base);

/*********************************************************************//**
  See "ft5426.c" for details of how to use this function.
 */
status_t FT5426_Denit(ft5426_handle_t *handle);

/*********************************************************************//**
  See "ft5426.c" for details of how to use this function.
 */
status_t FT5426_GetSingleTouch(ft5426_handle_t *handle, touch_event_t *touch_event, int *touch_x, int *touch_y);

/*********************************************************************//**
  See "ft5426.c" for details of how to use this function.
 */
status_t FT5426_GetMultiTouch(ft5426_handle_t *handle,
                                 int *touch_count,
                                 touch_point_t touch_array[FT5426_MAX_TOUCHES]);
/************************************************************************
	FUNCTIONS
************************************************************************/

#endif /* FT5426_H_ */
