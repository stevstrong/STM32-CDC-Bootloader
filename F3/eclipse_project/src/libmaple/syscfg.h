/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2013 OpenMusicKontrollers.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/stm32f3/include/series/syscfg.h
 * @author F3-port by Hanspeter Portner <dev@open-music-kontrollers.ch>
 * @brief System configuration controller (SYSCFG)
 */

#ifndef _LIBMAPLE_STM32F3_SYSCFG_H_
#define _LIBMAPLE_STM32F3_SYSCFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "libmaple_types.h"
#include "stm32f3xx.h"


/*
 * Register bit definitions
 */

#include "rcc.h"


/*
 * Routines
 */

/**
 * @brief Initialize the SYSCFG peripheral.
 */
void syscfg_init(void);

/**
 * @brief System memory mode
 * These values specify what memory to map to address 0x00000000.
 * @see syscfg_set_mem_mode
 */
typedef enum syscfg_mem_mode {
    /** Main flash memory is mapped at 0x0. */
    SYSCFG_MEM_MODE_FLASH        = 0x0,
    /** System flash (i.e. ST's baked-in bootloader) is mapped at 0x0. */
    SYSCFG_MEM_MODE_SYSTEM_FLASH = 0x1,
    /** FSMC bank 1 (NOR/PSRAM 1 and 2) is mapped at 0x0. */
    SYSCFG_MEM_MODE_FSMC_BANK_1  = 0x2,
    /** Embedded SRAM (i.e., not backup SRAM) is mapped at 0x0. */
    SYSCFG_MEM_MODE_SRAM         = 0x3,
} syscfg_mem_mode;


/**
 * @brief Set the memory to be mapped at address 0x00000000.
 *
 * This function can be used to override the BOOT pin
 * configuration. Some restrictions apply; see your chip's reference
 * manual for the details.
 *
 * @param mode Mode to set
 * @see syscfg_mem_mode
 */
void syscfg_set_mem_mode(syscfg_mem_mode);



#ifdef __cplusplus
}
#endif

#endif
