/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "evkbimxrt1050_flexspi_nor_config.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(__CC_ARM) || defined(__GNUC__)
    __attribute__((section(".boot_hdr.conf")))
#elif defined(__ICCARM__)
#pragma location=".boot_hdr.conf"
#endif


const flexspi_nor_config_t qspiflash_config = {
    .memConfig =
        {
          .tag = FLEXSPI_CFG_BLK_TAG,
          .version = FLEXSPI_CFG_BLK_VERSION,
          .readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackInternally,//kFlexSPIReadSampleClk_LoopbackFromDqsPad,
          .csHoldTime = 3u,
          .csSetupTime = 3u,
          .columnAddressWidth = 0u,
          .configCmdEnable = 0u,
          .controllerMiscOption = 0u,
          .deviceType = kFlexSpiDeviceType_SerialNOR,
          .sflashPadType = kSerialFlash_4Pads,
          .serialClkFreq = kFlexSpiSerialClk_50MHz,
          .lutCustomSeqEnable = 0u,
          .sflashA1Size = 0x01000000, /* 8MB/64Mbit */
          .lookupTable =
              {
                // Fast read sequence
                [0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xEB, RADDR_SDR, FLEXSPI_4PAD, 0x18),
                [1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x06, READ_SDR, FLEXSPI_4PAD, 0x02),
				[2] = FLEXSPI_LUT_SEQ(STOP, 0, 0, STOP, 0, 0),
				[3] = FLEXSPI_LUT_SEQ(STOP, 0, 0, STOP, 0, 0),
		        [4] = 0x0818040B,
		        [5] = 0x24043008,
		        [6] = 0x0,
		        [7] = 0x0,
		        [8] = 0x0818046B,
		        [9] = 0x26043208,
		       [10] = 0x0,
		       [11] = 0x0,
		       [12] = 0x24040405,
		       [13] = 0x0,
		       [14] = 0x0,
		       [15] = 0x0,
		       [16] = 0x0406,
		       [17] = 0x0,
		       [18] = 0x0,
		       [19] = 0x0,
		       [20] = 0x081804D7,
		       [21] = 0x0,
		       [22] = 0x0,
		       [23] = 0x0,
		       [24] = 0x08180402,
		       [25] = 0x2004,
		       [26] = 0x0,
		       [27] = 0x0,
		       [28] = 0x08180432,
		       [29] = 0x2204,
		       [30] = 0x0,
		       [31] = 0x0,
		       [32] = 0x301804AB,
		       [33] = 0x2404,
		       [34] = 0x0,
		       [35] = 0x0,
		       [36] = 0x20040401,
		       [37] = 0x0,
		       [38] = 0x0,
		       [39] = 0x0,
		       [40] = 0x0435,
		       [41] = 0x0,
		       [42] = 0x0,
		       [43] = 0x0,
		       [44] = 0x06F5,
		       [45] = 0x0,
		       [46] = 0x0,
		       [47] = 0x0,
		       [48] = 0x24040405,
		       [49] = 0x0,
		       [50] = 0x0,
		       [51] = 0x0,
		       [52] = 0x0,
		       [53] = 0x0,
		       [54] = 0x0,
		       [55] = 0x0,
		       [56] = 0x0,
		       [57] = 0x0,
		       [58] = 0x0,
		       [59] = 0x0,
				/*
            		  [0] = 0x08180403,
					  [1] = 0x00002404,
					  [2] = 0x00000000,
					  [3] = 0x00000000,
					  */
				/* Table filled with contents from the flexspi_nor_polling_transfer example. */
              },

			  /*Inicialización en 		  .deviceModeCfgEnable = true,
			  		  .deviceModeArg = 0x40, // QE bit in status register
			  		  .deviceModeSeq = {1, 6, 0}, // LUT sequence entry index 6, 1 LUT sequence
			  		  .lookupTable[4*6] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x01, WRITE_SDR, FLEXSPI_1PAD, 0x01),*/ /* write status register */

        },
        .pageSize = 256u,
        .sectorSize = 4u * 1024u,
        .blockSize = 32u * 1024u,
        .isUniformBlockSize = true,
};
