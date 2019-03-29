/*****************************************************************************
 *
 * Copyright (c) 2016 mCube, Inc.  All rights reserved.
 *
 * This source is subject to the mCube Software License.
 * This software is protected by Copyright and the information and source code
 * contained herein is confidential. The software including the source code
 * may not be copied and the information contained herein may not be used or
 * disclosed except with the written permission of mCube Inc.
 *
 * All other rights reserved.
 *****************************************************************************/
 
/*******************************************************************************
 *** INCLUDE FILES
 *******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "m_drv_mc34x6.h"
#include "m_drv_mc_utility.h"
#include "m_drv_interface.h"
#include "m_drv_console.h"

#include "m_drv_mc36xx.h"
#include "scenario.h"

/*******************************************************************************
 *** CONFIGURATION
 *******************************************************************************/
#define SCEN_SNIFF_TEST_ROUNDS         (8)
#define SCEN_SNIFF_REF_SAMPLE_COUNT    (64)

/*******************************************************************************
 *** CONSTANT / DEFINE
 *******************************************************************************/

/*******************************************************************************
 *** EXTERNAL FUNCTION
 *******************************************************************************/
extern void    main_get_int_event(s_m_drv_mc36xx_int_t **ptINTeventW, s_m_drv_mc36xx_int_t **ptINTeventR);

/*******************************************************************************
 *** FUNCTION
 *******************************************************************************/

/*********************************************************************
 *** main_scen_sniff
 *********************************************************************/
void    main_scen_sniff(void)
{
    unsigned char    _bTestRound = 0;

    s_m_drv_mc36xx_int_t   *_ptINTeventW = NULL;
    s_m_drv_mc36xx_int_t   *_ptINTeventR = NULL;

    M_PRINTF("[%s] ====================================================", __func__);

    main_get_int_event(&_ptINTeventW, &_ptINTeventR);

    for (_bTestRound = 0; _bTestRound < SCEN_SNIFF_TEST_ROUNDS; _bTestRound++)
    {
        M_PRINTF("<ROUND %d>", (_bTestRound + 1));

        // In WAKE mode =============================================
        M_PRINTF("Enter WAKE ...");

        M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
        M_DRV_MC36XX_SetSampleRate(E_M_DRV_MC36XX_CWAKE_SR_LP_54Hz, E_M_DRV_MC36XX_SNIFF_SR_DEFAULT_6Hz);
        M_DRV_MC36XX_ConfigINT(0, 0, 0, 0, 0);
        M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_CWAKE);

        M_DRV_MC36XX_ReadRegMap(NULL);

        main_read_data(SCEN_SNIFF_REF_SAMPLE_COUNT);

        // In SNIFF mode ============================================
        M_PRINTF("Enter SNIFF ...");

        M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
        M_DRV_MC36XX_SetSampleRate(E_M_DRV_MC36XX_CWAKE_SR_LP_54Hz, E_M_DRV_MC36XX_SNIFF_SR_DEFAULT_6Hz);
        M_DRV_MC36XX_ConfigINT(0, 0, 0, 0, 1);
        M_DRV_MC36XX_SetSniffThreshold(M_DRV_MC36XX_AXIS_X, 4);
        M_DRV_MC36XX_SetSniffThreshold(M_DRV_MC36XX_AXIS_Y, 4);
        M_DRV_MC36XX_SetSniffThreshold(M_DRV_MC36XX_AXIS_Z, 4);
        M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_SNIFF);

        _ptINTeventW->bWAKE = 0;
        _ptINTeventR->bWAKE = 0;

        M_DRV_MC36XX_ReadRegMap(NULL);

        while (1)
        {
            if (_ptINTeventR->bWAKE != _ptINTeventW->bWAKE)
            {
                _ptINTeventR->bWAKE++;

                break;
            }
        }

    }
}

