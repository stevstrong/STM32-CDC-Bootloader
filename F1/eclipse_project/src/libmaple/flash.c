/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
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
 * @file libmaple/flash.c
 * @brief Flash management functions
 */

#include "libmaple_types.h"
#include "flash.h"
#include "nvic.h"

/**
 * @brief Set flash wait states
 *
 * Note that not all wait states are available on every MCU. See the
 * Flash programming manual for your MCU for restrictions on the
 * allowed value of wait_states for a given system clock (SYSCLK)
 * frequency.
 *
 * @param wait_states number of wait states (one of
 *                    FLASH_WAIT_STATE_0, FLASH_WAIT_STATE_1,
 *                    ..., FLASH_WAIT_STATE_7).
 */
void flash_set_latency(uint32 wait_states)
{
    uint32 val = FLASH->ACR;

    val &= ~FLASH_ACR_LATENCY;
    val |= wait_states;

    FLASH->ACR = val;
}

//-----------------------------------------------------------------------------
// Reference: RM0008 chap. 3.3.3.
//-----------------------------------------------------------------------------
/*
 * The right sequence is :

ERASE:
 - check busy flag (wait till it is reset)
 - Set PER bit to enable the erase
 - write the address of the page to be erased into FLASH_AR register (the entire page containing the address will be erased).
 - Set STRT bit to trigger the erase
 - Wait till Busy flag or STRT flag is cleared by hardware (it takes a time)
 - Read and verify that the location is erased (should have 0xFFFF)

WRITE:
 - Unlock the flash
 - Wait on BUSY
 - Set the PG bit
 - Write to destination (16-bit in a half-word aligned address)
 - Wait on BUSY
*/
//-----------------------------------------------------------------------------
void flash_erase_page(uint16_t *page)
{
	// Unlock Flash with magic keys
	flash_unlock();
	flash_wait_for_ready();

	// Erase page
	flash_set_cr(FLASH_CR_PER); // erase page flag
	flash_set_page((uint32_t) page);
	flash_start();

	flash_wait_for_ready();
}

//-----------------------------------------------------------------------------
void flash_write_data(uint16_t *page, uint16_t *data, uint16_t size)
{
	flash_set_cr(FLASH_CR_PG); // erase program flag

	while (size--)
	{
		*page++ = *data++;

		flash_wait_for_ready();
	}
}

