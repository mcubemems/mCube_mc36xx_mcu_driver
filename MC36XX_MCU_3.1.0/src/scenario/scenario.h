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

#ifndef _SCENARIO_H_
    #define _SCENARIO_H_

/*******************************************************************************
 *** INCLUDE FILES
 *******************************************************************************/

/*******************************************************************************
 *** CONSTANT / DEFINE
 *******************************************************************************/

/*****************************************************************************
 *** DATA TYPE / STRUCTURE DEFINITION / ENUM
 *****************************************************************************/

/*******************************************************************************
 *** GLOBAL VARIABLE
 *******************************************************************************/
extern void    main_read_data(int nSampleCount);
extern void    main_scen_config(void);
extern void    main_scen_fifo(void);
extern void    main_scen_sniff(void);

#endif    // END of _SCENARIO_H_

