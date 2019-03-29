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
#define SCEN_CONFIG_REF_SAMPLE_COUNT    (1024)

/*******************************************************************************
 *** CONSTANT / DEFINE
 *******************************************************************************/

/*******************************************************************************
 *** MACRO
 *******************************************************************************/

/*******************************************************************************
 *** TYPE DEFINITION / DATA STRUCTURE
 *******************************************************************************/

/*******************************************************************************
 *** STATIC VARIABLE
 *******************************************************************************/

/*******************************************************************************
 *** EXTERNAL FUNCTION
 *******************************************************************************/

/*******************************************************************************
 *** _config_range_resolution
 *******************************************************************************/
typedef unsigned char uint8_t;

static void    _config_range_resolution(void)
{
    uint8_t    _bCfgIdxRng = 0;
    uint8_t    _bCfgIdxRes = 0;

    uint8_t    _baRng[E_M_DRV_MC36XX_RANGE_END]      = { 2, 4, 8, 16, 12, 24 };
    uint8_t    _baRes[E_M_DRV_MC36XX_RES_END] = { 6, 7, 8, 10, 12, 14 };

    for (_bCfgIdxRng = E_M_DRV_MC36XX_RANGE_2G; _bCfgIdxRng < E_M_DRV_MC36XX_RANGE_END; _bCfgIdxRng++)
    {
        for (_bCfgIdxRes = E_M_DRV_MC36XX_RES_6BIT; _bCfgIdxRes < E_M_DRV_MC36XX_RES_END; _bCfgIdxRes++)
        {
            M_PRINTF("@@@@@ Range: %dG, Resolution: %d-bit", _baRng[_bCfgIdxRng], _baRes[_bCfgIdxRes]);

            M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);

            M_DRV_MC36XX_ConfigRegRngResCtrl(_bCfgIdxRng, _bCfgIdxRes);

            M_DRV_MC36XX_ReadRegMap(NULL);

            main_read_data(SCEN_CONFIG_REF_SAMPLE_COUNT);
        }
    }

    M_DRV_MC36XX_ConfigRegRngResCtrl(E_M_DRV_MC36XX_RANGE_8G, E_M_DRV_MC36XX_RES_14BIT);
}

/*******************************************************************************
 *** _config_cwake_sample_rate
 *******************************************************************************/
static void    _config_cwake_sample_rate(void)
{
    uint8_t    _bCfgIdx = 0;

    uint8_t    _bCfgToBeCheckedCount = E_M_DRV_MC36XX_CWAKE_SR_PR_DUMMY_END;
    uint8_t    _baIsCfgChecked[E_M_DRV_MC36XX_CWAKE_SR_PR_DUMMY_END] = { 0 };

    float    _baSR_UL[(E_M_DRV_MC36XX_CWAKE_SR_UL_DUMMY_END - E_M_DRV_MC36XX_CWAKE_SR_UL_DUMMY_BASE)] = { 0.0f, 0.4f, 0.8f, 1.4f, 5.5f, 11.0f, 23.0f, 46.0f, 90.0f, 190.0f, 370.0f };
    float    _baSR_LP[(E_M_DRV_MC36XX_CWAKE_SR_LP_DUMMY_END - E_M_DRV_MC36XX_CWAKE_SR_LP_DUMMY_BASE)] = { 0.0f, 0.4f, 0.8f, 1.5f, 6.0f, 13.0f, 25.0f, 50.0f, 100.0f, 200.0f };
    float    _baSR_PR[(E_M_DRV_MC36XX_CWAKE_SR_PR_DUMMY_END - E_M_DRV_MC36XX_CWAKE_SR_PR_DUMMY_BASE)] = { 0.0f, 0.2f, 0.4f, 1.6f, 3.0f, 14.0f, 26.0f, 53.0f };

    //unsigned    _bSeed;
    //srand(_bSeed);

    memset(_baIsCfgChecked, 0, sizeof(_baIsCfgChecked));

    for ( ; ; )
    {
        _bCfgIdx = (rand() % E_M_DRV_MC36XX_CWAKE_SR_PR_DUMMY_END);

        if (_baIsCfgChecked[_bCfgIdx])
            continue;

        if (E_M_DRV_MC36XX_CWAKE_SR_LP_DUMMY_BASE > _bCfgIdx)
            M_PRINTF("@@@@@ CWAKE [ULTRA LOW POWER] - %.1fHz", _baSR_UL[(_bCfgIdx - E_M_DRV_MC36XX_CWAKE_SR_UL_DUMMY_BASE)]);
        else if (E_M_DRV_MC36XX_CWAKE_SR_PR_DUMMY_BASE > _bCfgIdx)
            M_PRINTF("@@@@@ CWAKE [LOW POWER] - %.1fHz", _baSR_LP[(_bCfgIdx - E_M_DRV_MC36XX_CWAKE_SR_LP_DUMMY_BASE)]);
        else
            M_PRINTF("@@@@@ CWAKE [PRECISION] - %.1fHz", _baSR_PR[(_bCfgIdx - E_M_DRV_MC36XX_CWAKE_SR_PR_DUMMY_BASE)]);

        M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);

        if (M_DRV_MC36XX_RETCODE_SUCCESS != M_DRV_MC36XX_SetSampleRate(_bCfgIdx, E_M_DRV_MC36XX_SNIFF_SR_6Hz))
        {
            M_PRINTF("!!! un-supported rate config...");

            goto _CONFIG_CWAKE_SR_MOVE_NEXT;
        }

        M_DRV_MC36XX_ReadRegMap(NULL);

        main_read_data(SCEN_CONFIG_REF_SAMPLE_COUNT);

_CONFIG_CWAKE_SR_MOVE_NEXT:
        _baIsCfgChecked[_bCfgIdx] = 1;

        _bCfgToBeCheckedCount--;

        if (0 == _bCfgToBeCheckedCount)
            break;
    }
}

/*******************************************************************************
 *** FUNCTION
 *******************************************************************************/

/*********************************************************************
 *** main_scen_config
 *********************************************************************/
void    main_scen_config(void)
{
    M_PRINTF("[%s] ====================================================", __func__);

    M_PRINTF("     /////////////// CONFIG:: RANGE/RESOLUTION ///////////////\n");
    _config_range_resolution();

    M_PRINTF("     /////////////// CONFIG:: SAMPLE RATE ///////////////\n");
    _config_cwake_sample_rate();
}

