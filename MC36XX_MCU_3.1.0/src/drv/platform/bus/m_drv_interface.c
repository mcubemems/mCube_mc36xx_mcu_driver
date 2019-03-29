/******************************************************************************
 *
 * Copyright (c) 2018 mCube, Inc.  All rights reserved.
 *
 * This source is subject to the mCube Software License.
 * This software is protected by Copyright and the information and source code
 * contained herein is confidential. The software including the source code
 * may not be copied and the information contained herein may not be used or
 * disclosed except with the written permission of mCube Inc.
 *
 * All other rights reserved.
 *****************************************************************************/

/**
 * @file    m_drv_interface.c
 * @author  mCube
 * @date    22 June 2018
 * @brief   System I2C/SPI interface for mCube drivers.
 * @see     http://www.mcubemems.com
 */

/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
/** Please include interface driver from platform SDK */


/** mCube functions include */
#include "m_drv_interface.h"
#include "m_drv_console.h"

/******************************************************************************
 *** CONFIGURATION
 *****************************************************************************/

/******************************************************************************
 *** CONSTANT / DEFINE
 *****************************************************************************/

/******************************************************************************
 *** STATIC VARIABLE
 *****************************************************************************/

/******************************************************************************
 *** FUNCTION
 *****************************************************************************/
/** Delay required milliseconds */
void mcube_delay_ms(unsigned int ms)
{
    /** Function hook by customer. */
}

/** I2C init function */
int m_drv_i2c_init(void)
{
    /** Function hook by customer. */
    return 0;
}

/** SPI init function */
int m_drv_spi_init(e_m_drv_interface_spimode_t spi_hs_mode)
{
    /** Function hook by customer. */
    return 0;
}

/** I2C/SPI write function */
/** bSpi : I2C/SPI bus selection.        SPI: 0,       I2C: 1           */
/** chip_select : Chip selection.        SPI: CS pins, I2C: I2C address */
/** reg : Sensor registers. */
/** value : Write value.*/
/** size : data length */
uint8_t mcube_write_regs(bool bSpi, uint8_t chip_select, uint8_t reg,       \
                         uint8_t *value, uint8_t size)
{
    /** Please implement I2C/SPI write function from platform SDK */
    /** 0 = SPI, 1 = I2C */
    if(!bSpi) {
        /** SPI write function */
    } else {
        /** I2C write function */
    } 

    return 0;
}

/** I2C/SPI write function */
/** bSpi : I2C/SPI bus selection.        SPI: 0,       I2C: 1           */
/** chip_select : Chip selection.        SPI: CS pins, I2C: I2C address */
/** reg : Sensor registers. */
/** value : read value.*/
/** size : data length */
unsigned char mcube_read_regs(bool bSpi, uint8_t chip_select, uint8_t reg,  \
                              uint8_t * value, uint8_t size)

{
    /** Please implement I2C/SPI read function from platform SDK */
    /** 0 = SPI, 1 = I2C */
    if(!bSpi) {
        /** SPI read function */
    } else {
        /** I2C read function */
    } 

    return 0;
}

/*********************************************************************
 *** mcube_burst_read
 *********************************************************************/
uint8_t mcube_burst_read(uint8_t address, uint8_t *buf, uint8_t size)
{
    return mcube_read_regs(0, 0, address, buf, size);
}

