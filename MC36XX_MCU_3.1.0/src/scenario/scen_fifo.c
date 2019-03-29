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
#define SCEN_FIFO_THRESHOLD      (8)
#define SCEN_FIFO_TEST_ROUNDS    (8)

/*******************************************************************************
 *** CONSTANT / DEFINE
 *******************************************************************************/
#define SCEN_FIFO_FULL    (M_DRV_MC36XX_FIFO_DEPTH)

#define SCEN_FIFO_EPISODE_THRESHOLD                (0)
#define SCEN_FIFO_EPISODE_FULL                     (1)
#define SCEN_FIFO_EPISODE_RESOLUTION_12BIT_TEST    (2)

/*******************************************************************************
 *** EXTERNAL FUNCTION
 *******************************************************************************/
extern void    main_get_int_event(s_m_drv_mc36xx_int_t **ptINTeventW, s_m_drv_mc36xx_int_t **ptINTeventR);

/*********************************************************************
 *** _IntHandler
 *********************************************************************/
static void    _IntHandler(int nEpisode, s_m_drv_mc36xx_int_t *ptINTeventW, s_m_drv_mc36xx_int_t *ptINTeventR)
{
    switch (nEpisode)
    {
    case SCEN_FIFO_EPISODE_THRESHOLD:
         if (ptINTeventR->bFIFO_THRESHOLD != ptINTeventW->bFIFO_THRESHOLD)
         {
             ptINTeventR->bFIFO_THRESHOLD++;

             M_PRINTF("<ROUND %d>", ptINTeventR->bFIFO_THRESHOLD);

             main_read_data(SCEN_FIFO_THRESHOLD);
         }
         break;    

    case SCEN_FIFO_EPISODE_FULL:
         if (ptINTeventR->bFIFO_FULL != ptINTeventW->bFIFO_FULL)
         {
             ptINTeventR->bFIFO_FULL++;

             M_PRINTF("<ROUND %d>", ptINTeventR->bFIFO_FULL);

             main_read_data(SCEN_FIFO_FULL);
         }
         break;    
    }

    #if 0
    if (ptINTeventR->bFIFO_EMPTY != ptINTeventW->bFIFO_EMPTY)
    {
        ptINTeventR->bFIFO_EMPTY++;

        M_PRINTF("[%s] bFIFO_EMPTY", __func__);
    }
    #endif
}

/*******************************************************************************
 *** FUNCTION
 *******************************************************************************/

/*********************************************************************
 *** main_scen_fifo
 *********************************************************************/
void    main_scen_fifo(void)
{
    int    _nPreEpisode = -1;
    int    _nEpisode    = SCEN_FIFO_EPISODE_THRESHOLD;

    s_m_drv_mc36xx_int_t   *_ptINTeventW = NULL;
    s_m_drv_mc36xx_int_t   *_ptINTeventR = NULL;

    M_PRINTF("[%s] ====================================================", __func__);

    main_get_int_event(&_ptINTeventW, &_ptINTeventR);

    M_DRV_INT_Init();

    while(1)
    {
        switch (_nEpisode)
        {
        case SCEN_FIFO_EPISODE_THRESHOLD:
             if (_nPreEpisode != _nEpisode)
             {
                 M_PRINTF("@@@@@ FIFO-THRESHOLD (8 Samples)");
                 M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
                 M_DRV_MC36XX_SetSampleRate(E_M_DRV_MC36XX_CWAKE_SR_LP_54Hz, E_M_DRV_MC36XX_SNIFF_SR_6Hz);
                 M_DRV_MC36XX_EnableFIFO(E_M_DRV_MC36XX_FIFO_CTL_ENABLE, E_M_DRV_MC36XX_FIFO_MODE_WATERMARK, SCEN_FIFO_THRESHOLD);
                 M_DRV_MC36XX_ConfigINT(1, 0, 0, 0, 0);
                 M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_CWAKE);
                 M_DRV_MC36XX_ReadRegMap(NULL);

                 _nPreEpisode = _nEpisode;
             }
             else
             {
                if (SCEN_FIFO_TEST_ROUNDS < _ptINTeventR->bFIFO_THRESHOLD)
                    _nEpisode++;
             }
             break;    
    
        case SCEN_FIFO_EPISODE_FULL:
             if (_nPreEpisode != _nEpisode)
             {
                 M_PRINTF("@@@@@ FIFO-FULL (32 Samples)");
                 M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
                 M_DRV_MC36XX_SetSampleRate(E_M_DRV_MC36XX_CWAKE_SR_LP_7Hz, E_M_DRV_MC36XX_SNIFF_SR_0p4Hz);
                 M_DRV_MC36XX_EnableFIFO(E_M_DRV_MC36XX_FIFO_CTL_ENABLE, E_M_DRV_MC36XX_FIFO_MODE_WATERMARK, 0);
                 M_DRV_MC36XX_ConfigINT(0, 1, 0, 0, 0);
                 M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_CWAKE);
                 M_DRV_MC36XX_ReadRegMap(NULL);

                 _nPreEpisode = _nEpisode;
             }
             else
             {
                if (SCEN_FIFO_TEST_ROUNDS < _ptINTeventR->bFIFO_FULL)
                    _nEpisode++;
             }
             break;    
    
        case SCEN_FIFO_EPISODE_RESOLUTION_12BIT_TEST:
             M_PRINTF("@@@@@ Request FIFO, Resolution = 12-bit");
             M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
             M_DRV_MC36XX_EnableFIFO(E_M_DRV_MC36XX_FIFO_CTL_DISABLE, E_M_DRV_MC36XX_FIFO_MODE_NORMAL, 0);
             M_DRV_MC36XX_ConfigRegRngResCtrl(E_M_DRV_MC36XX_RANGE_8G, E_M_DRV_MC36XX_RES_12BIT);
             M_DRV_MC36XX_EnableFIFO(E_M_DRV_MC36XX_FIFO_CTL_ENABLE, E_M_DRV_MC36XX_FIFO_MODE_NORMAL, 0);
             M_DRV_MC36XX_ReadRegMap(NULL);

             M_PRINTF("@@@@@ Request > 12-bit Resolution, when FIFO is not in service");
             M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
             M_DRV_MC36XX_EnableFIFO(E_M_DRV_MC36XX_FIFO_CTL_DISABLE, E_M_DRV_MC36XX_FIFO_MODE_NORMAL, 0);
             M_DRV_MC36XX_ConfigRegRngResCtrl(E_M_DRV_MC36XX_RANGE_8G, E_M_DRV_MC36XX_RES_14BIT);
             M_DRV_MC36XX_ReadRegMap(NULL);

             _nEpisode++;
             break;    

        default:
             M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
             M_DRV_MC36XX_EnableFIFO(E_M_DRV_MC36XX_FIFO_CTL_DISABLE, E_M_DRV_MC36XX_FIFO_MODE_NORMAL, 0);
             return;
        }

        _IntHandler(_nEpisode, _ptINTeventW, _ptINTeventR);
    }
}

