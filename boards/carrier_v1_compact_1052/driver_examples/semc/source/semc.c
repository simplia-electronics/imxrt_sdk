/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    COMPACT-1052_Project_semc.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "COMPACT.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_semc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SEMC               												SEMC
#define EXAMPLE_SEMC_START_ADDRESS 												(0x80000000U)
#define EXAMPLE_SEMC_CLK_FREQ      												CLOCK_GetFreq(kCLOCK_SemcClk)

#define SEMC_EXAMPLE_DATALEN    												(0x100000U)
#define SEMC_EXAMPLE_WRITETIMES 												(1000U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t BOARD_InitSEMC(void);
static void SEMC_SDRAM_ReadWriteTest( void );
/*******************************************************************************
 * Variables
 ******************************************************************************/

uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t BOARD_InitSEMC(void)
{
    semc_config_t config;
    semc_sdram_config_t sdramconfig;
    uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;

    /* Initializes the MAC configure structure to zero. */
    memset(&config, 0, sizeof(semc_config_t));
    memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

    /* Initialize SEMC. */
    SEMC_GetDefaultConfig(&config);
    config.dqsMode = kSEMC_Loopbackdqspad; /* For more accurate timing. */
    SEMC_Init(SEMC, &config);

    /* Configure SDRAM. */
    sdramconfig.csxPinMux           = kSEMC_MUXCSX0;
    sdramconfig.address             = 0x80000000;
    sdramconfig.memsize_kbytes      = 16 * 1024; /* 32MB = 32*1024*1KBytes*/
    sdramconfig.portSize            = kSEMC_PortSize16Bit;
    sdramconfig.burstLen            = kSEMC_Sdram_BurstLen8;
    sdramconfig.columnAddrBitNum    = kSEMC_SdramColunm_9bit;
    sdramconfig.casLatency          = kSEMC_LatencyThree;
    sdramconfig.tPrecharge2Act_Ns   = 18; /* Trp 18ns */
    sdramconfig.tAct2ReadWrite_Ns   = 18; /* Trcd 18ns */
    sdramconfig.tRefreshRecovery_Ns = 67; /* Use the maximum of the (Trfc , Txsr). */
    sdramconfig.tWriteRecovery_Ns   = 12; /* 12ns */
    sdramconfig.tCkeOff_Ns =
        42; /* The minimum cycle of SDRAM CLK off state. CKE is off in self refresh at a minimum period tRAS.*/
    sdramconfig.tAct2Prechage_Ns       = 42; /* Tras 42ns */
    sdramconfig.tSelfRefRecovery_Ns    = 67;
    sdramconfig.tRefresh2Refresh_Ns    = 60;
    sdramconfig.tAct2Act_Ns            = 60;
    sdramconfig.tPrescalePeriod_Ns     = 160 * (1000000000 / clockFrq);
    sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; /* 64ms/8192 */
    sdramconfig.refreshUrgThreshold    = sdramconfig.refreshPeriod_nsPerRow;
    sdramconfig.refreshBurstLen        = 1;
    return SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
}


/*!
 * @brief Main function
 */
int main(void)
{
    /* Hardware initialize. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    CLOCK_InitSysPfd(kCLOCK_Pfd2, 29);
    /* Set semc clock to 163.86 MHz */
    CLOCK_SetMux(kCLOCK_SemcMux, 1);
    CLOCK_SetDiv(kCLOCK_SemcDiv, 1);
    BOARD_InitDebugConsole();

    PRINTF("\r\n SEMC SDRAM Example Start!\r\n");
    if (BOARD_InitSEMC() != kStatus_Success)
    {
        PRINTF("\r\n SEMC SDRAM Init Failed\r\n");
    }

    SEMC_SDRAM_ReadWriteTest();
    PRINTF("\r\n SEMC SDRAM Example End.\r\n");
    while (1)
    {
    }
}

static void SEMC_SDRAM_ReadWriteTest( void )
{
    uint32_t *sdram  = (uint32_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    uint32_t errIncrementally = 0;
    uint32_t errDecrementally = 0;
    uint32_t errAAA = 0;

    /* Check we can write from the beginning to the end of the RAM incrementally*/
    uint32_t i = 0;
    for( i = 0; i < SEMC_EXAMPLE_DATALEN; i++ )
    {
    	sdram[i] = i;
    }

    for( i = 0; i < SEMC_EXAMPLE_DATALEN; i++ )
    {
    	if( sdram[i] != i )
    	{
    		errIncrementally++;
    	}
    }

    /* Check we can write the RAM decrementally */
    for( i = 0; i < SEMC_EXAMPLE_DATALEN; i++ )
    {
    	sdram[i] = SEMC_EXAMPLE_DATALEN - 1 -i ;
    }

    for( i = 0; i < SEMC_EXAMPLE_DATALEN; i++ )
    {
    	if( sdram[i] != (SEMC_EXAMPLE_DATALEN - 1 -i ) )
    	{
    		errDecrementally++;
    	}
    }

    /* Check we can write the RAM starting */
    for( i = 0; i < SEMC_EXAMPLE_DATALEN; i++ )
    {
    	sdram[i] = 0xAAAAAAAA + i;
    }

    for( i = 0; i < SEMC_EXAMPLE_DATALEN; i++ )
    {
    	if( sdram[i] != (0xAAAAAAAA + i) )
    	{
    		errAAA++;
    	}
    }

    PRINTF("\r\n Incrementally test has %d errors\r\n", errIncrementally );
    PRINTF("\r\n Decrementally test has %d errors\r\n", errDecrementally );
    PRINTF("\r\n 0xAAAAAAAA test has %d errors\r\n", errAAA );


}
