/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
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

/*
 * RCC private header.
 */

#ifndef _LIBMAPLE_PRIVATE_RCC_H_
#define _LIBMAPLE_PRIVATE_RCC_H_

#include "bitband.h"

struct rcc_dev_info {
    const rcc_clk_domain clk_domain;
    const uint8 line_num;
};

extern void rcc_do_clk_enable(__IO uint32** enable_regs, rcc_clk_id id);
extern void rcc_do_reset_dev(__IO uint32** reset_regs, rcc_clk_id id);
extern void rcc_do_set_prescaler(const uint32 *masks, rcc_prescaler prescaler, uint32 divider);
extern void rcc_do_clk_disable(__IO uint32** enable_regs, rcc_clk_id id);

extern const struct rcc_dev_info rcc_dev_table[];


#endif
