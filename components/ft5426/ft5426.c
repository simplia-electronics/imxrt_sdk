/*********************************************************************//**

	@file ft5426.c

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

/************************************************************************
	INCLUDES
************************************************************************/
/* Project options include */

/* Scheduler includes. */

/* Drivers includes. */
#include "fsl_common.h"
#include "fsl_lpi2c.h"
#include "ft5426.h"
/* Library includes. */

/************************************************************************
	LOCAL DEFINES AND TYPES
************************************************************************/

/* Private Types */
typedef struct _ft5426_touch_point
{
    uint8_t XH;
    uint8_t XL;
    uint8_t YH;
    uint8_t YL;
    uint8_t RESERVED[2];
} ft5426_touch_point_t;

typedef struct _ft5426_touch_data
{
    uint8_t GEST_ID;
    uint8_t TD_STATUS;
    ft5426_touch_point_t TOUCH[FT5426_MAX_TOUCHES];
} ft5426_touch_data_t;

/* Private Defines */

#define TOUCH_POINT_GET_EVENT(T) ((touch_event_t)((T).XH >> 6))
#define TOUCH_POINT_GET_ID(T)    ((T).YH >> 4)
#define TOUCH_POINT_GET_X(T)     ((((T).XH & 0x0f) << 8) | (T).XL)
#define TOUCH_POINT_GET_Y(T)     ((((T).YH & 0x0f) << 8) | (T).YL)


/************************************************************************
	DECLARATIONS
************************************************************************/
/* Private Global Variables */

/* Private Functions */
/*********************************************************************//**
 * 	@brief
 * 	@param
 *  @return
 *  @note
 */


/************************************************************************
	FUNCTIONS
************************************************************************/

/*********************************************************************//**
 * 	@brief 
 * 	@param 
 *  @return 
 *  @note
 */
status_t FT5426_Init(ft5426_handle_t *handle, LPI2C_Type *base)
{
    lpi2c_master_transfer_t *xfer = &(handle->xfer);
    status_t status;
    uint8_t mode;

    assert(handle);
    assert(base);

    if (!handle || !base)
    {
        return kStatus_InvalidArgument;
    }

    handle->base = base;

    /* clear transfer structure and buffer */
    memset(xfer, 0, sizeof(*xfer));
    memset(handle->touch_buf, 0, FT5426_TOUCH_DATA_LEN);

    /* set device mode to normal operation */
    mode                 = 0;
    xfer->slaveAddress   = FT5426_I2C_ADDRESS;
    xfer->direction      = kLPI2C_Write;
    xfer->subaddress     = 0;
    xfer->subaddressSize = 1;
    xfer->data           = &mode;
    xfer->dataSize       = 1;
    xfer->flags          = kLPI2C_TransferDefaultFlag;

    status = LPI2C_MasterTransferBlocking(handle->base, &handle->xfer);

    /* prepare transfer structure for reading touch data */
    xfer->slaveAddress   = FT5426_I2C_ADDRESS;
    xfer->direction      = kLPI2C_Read;
    xfer->subaddress     = 1;
    xfer->subaddressSize = 1;
    xfer->data           = handle->touch_buf;
    xfer->dataSize       = 6;//FT5426_TOUCH_DATA_LEN;
    xfer->flags          = kLPI2C_TransferDefaultFlag;

    return status;
}

/*********************************************************************//**
 * 	@brief
 * 	@param
 *  @return
 *  @note
 */
status_t FT5426_Denit(ft5426_handle_t *handle)
{
    assert(handle);

    if (!handle)
    {
        return kStatus_InvalidArgument;
    }

    handle->base = NULL;
    return kStatus_Success;
}

/*********************************************************************//**
 * 	@brief
 * 	@param
 *  @return
 *  @note
 */
status_t FT5426_ReadTouchData(ft5426_handle_t *handle)
{
    assert(handle);

    if (!handle)
    {
        return kStatus_InvalidArgument;
    }

    return LPI2C_MasterTransferBlocking(handle->base, &handle->xfer);
}

/*********************************************************************//**
 * 	@brief 
 * 	@param 
 *  @return 
 *  @note
 */
status_t FT5426_GetSingleTouch(ft5426_handle_t *handle, touch_event_t *touch_event, int *touch_x, int *touch_y)
{
    status_t status;
    touch_event_t touch_event_local;

    status = FT5426_ReadTouchData(handle);

    if (status == kStatus_Success)
    {
        ft5426_touch_data_t *touch_data = (ft5426_touch_data_t *)(void *)(handle->touch_buf);

        if (touch_event == NULL)
        {
            touch_event = &touch_event_local;
        }
        *touch_event = TOUCH_POINT_GET_EVENT(touch_data->TOUCH[0]);

        /* Update coordinates only if there is touch detected */
        if ((*touch_event == kTouch_Down) || (*touch_event == kTouch_Contact))
        {
            if (touch_x)
            {
                *touch_x = TOUCH_POINT_GET_X(touch_data->TOUCH[0]);
            }
            if (touch_y)
            {
                *touch_y = TOUCH_POINT_GET_Y(touch_data->TOUCH[0]);
            }
        }
    }

    return status;
}

status_t FT5426_GetMultiTouch(ft5426_handle_t *handle,
                                 int *touch_count,
                                 touch_point_t touch_array[FT5426_MAX_TOUCHES])
{
    status_t status;

    status = FT5426_ReadTouchData(handle);

    if (status == kStatus_Success)
    {
        ft5426_touch_data_t *touch_data = (ft5426_touch_data_t *)(void *)(handle->touch_buf);
        int i;

        /* Check for valid number of touches - otherwise ignore touch information */
        if (touch_data->TD_STATUS > FT5426_MAX_TOUCHES)
        {
            touch_data->TD_STATUS = 0;
        }

        /* Decode number of touches */
        if (touch_count)
        {
            *touch_count = touch_data->TD_STATUS;
        }

        /* Decode valid touch points */
        for (i = 0; i < touch_data->TD_STATUS; i++)
        {
            touch_array[i].TOUCH_ID    = TOUCH_POINT_GET_ID(touch_data->TOUCH[i]);
            touch_array[i].TOUCH_EVENT = TOUCH_POINT_GET_EVENT(touch_data->TOUCH[i]);
            touch_array[i].TOUCH_X     = TOUCH_POINT_GET_X(touch_data->TOUCH[i]);
            touch_array[i].TOUCH_Y     = TOUCH_POINT_GET_Y(touch_data->TOUCH[i]);
        }

        /* Clear vacant elements of touch_array */
        for (; i < FT5426_MAX_TOUCHES; i++)
        {
            touch_array[i].TOUCH_ID    = 0;
            touch_array[i].TOUCH_EVENT = kTouch_Reserved;
            touch_array[i].TOUCH_X     = 0;
            touch_array[i].TOUCH_Y     = 0;
        }
    }

    return status;
}
