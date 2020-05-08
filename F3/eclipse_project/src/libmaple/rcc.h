/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
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
 * @file libmaple/include/libmaple/rcc.h
 * @brief Reset and Clock Control (RCC) interface.
 */

#ifndef _LIBMAPLE_RCC_H_
#define _LIBMAPLE_RCC_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "libmaple_types.h"
#include "stm32f3xx.h"


/* Put the SYSCLK sources before the series header is included, as it
 * might need them. */
/**
 * @brief SYSCLK sources
 * @see rcc_switch_sysclk()
 */
typedef enum rcc_sysclk_src {
    RCC_CLKSRC_HSI = 0x0,
    RCC_CLKSRC_HSE = 0x1,
    RCC_CLKSRC_PLL = 0x2,
} rcc_sysclk_src;


/*
 * Register bit definitions
 */



/*
 * libmaple-mandated enumeration types.
 */

/**
 * @brief STM32F3 rcc_clk_id.
 */
typedef enum rcc_clk_id {
	RCC_GPIOA,
	RCC_GPIOB,
	RCC_GPIOC,
	RCC_USART1,
#if 0
	RCC_GPIOD,
	RCC_GPIOE,
	RCC_GPIOF,

	RCC_ADC12,
	RCC_ADC34,

	RCC_DAC,

	RCC_DMA1,
	RCC_DMA2,

	RCC_I2C1,
	RCC_I2C2,

	RCC_SPI1,
	RCC_SPI2,
	RCC_SPI3,

	RCC_USART2,
	RCC_USART3,
#if defined(STM32_HIGH_DENSITY) || defined(STM32_XL_DENSITY)
	RCC_UART4,
	RCC_UART5,
#endif
	RCC_TIMER1,
	RCC_TIMER2,
	RCC_TIMER3,
	RCC_TIMER4,
	RCC_TIMER5,
	RCC_TIMER6,
	RCC_TIMER7,
	RCC_TIMER8,
#if 0
	RCC_TIMER9,
	RCC_TIMER10,
	RCC_TIMER11,
	RCC_TIMER12,
	RCC_TIMER13,
	RCC_TIMER14,
#endif
	RCC_TIMER15,
	RCC_TIMER16,
	RCC_TIMER17,
	RCC_CRC,
	RCC_FLITF,
	RCC_SRAM,
#endif
	RCC_SYSCFG,
	RCC_PWR,
	RCC_USB,
} rcc_clk_id;

/**
 * @brief STM32F3 PLL clock sources.
 * @see rcc_configure_pll()
 */
typedef enum rcc_pllsrc {
    RCC_PLLSRC_HSE = (0x1 << RCC_CFGR_PLLSRC_Pos),
    RCC_PLLSRC_HSI_DIV_2 = (0x0 << RCC_CFGR_PLLSRC_Pos)
} rcc_pllsrc;

/**
 * @brief STM32F3 clock domains.
 * @see rcc_dev_clk()
 */
typedef enum rcc_clk_domain {
    RCC_APB1,
    RCC_APB2,
    RCC_AHB
} rcc_clk_domain;

/**
 * @brief STM32F3 Prescaler identifiers
 * @see rcc_set_prescaler()
 */
typedef enum rcc_prescaler {
    RCC_PRESCALER_AHB,
    RCC_PRESCALER_APB1,
    RCC_PRESCALER_APB2,
    RCC_PRESCALER_USB,
} rcc_prescaler;

/**
 * @brief STM32F3 ADC prescaler dividers
 * @see rcc_set_prescaler()
 */
typedef enum rcc_adc_divider {
    RCC_ADCPRE_PCLK_DISABLED = 0x00,
    RCC_ADCPRE_PCLK_DIV_1    = 0x10,
    RCC_ADCPRE_PCLK_DIV_2    = 0x11,
    RCC_ADCPRE_PCLK_DIV_4    = 0x12,
    RCC_ADCPRE_PCLK_DIV_6    = 0x13,
    RCC_ADCPRE_PCLK_DIV_8    = 0x14,
    RCC_ADCPRE_PCLK_DIV_10   = 0x15,
    RCC_ADCPRE_PCLK_DIV_12   = 0x16,
    RCC_ADCPRE_PCLK_DIV_16   = 0x17,
    RCC_ADCPRE_PCLK_DIV_32   = 0x18,
    RCC_ADCPRE_PCLK_DIV_64   = 0x19,
    RCC_ADCPRE_PCLK_DIV_128  = 0x1A,
    RCC_ADCPRE_PCLK_DIV_256  = 0x1B,
} rcc_adc_divider;

/**
 * @brief STM32F3 PREDIV prescaler dividers
 * @see rcc_set_prescaler()
 */
typedef enum rcc_prediv_divider {
		RCC_PREDIV_PCLK_DIV_1  = 0x0,
		RCC_PREDIV_PCLK_DIV_2  = 0x1,
		RCC_PREDIV_PCLK_DIV_3  = 0x2,
		RCC_PREDIV_PCLK_DIV_4  = 0x3,
		RCC_PREDIV_PCLK_DIV_5  = 0x4,
		RCC_PREDIV_PCLK_DIV_6  = 0x5,
		RCC_PREDIV_PCLK_DIV_7  = 0x6,
		RCC_PREDIV_PCLK_DIV_8  = 0x7,
		RCC_PREDIV_PCLK_DIV_9  = 0x8,
		RCC_PREDIV_PCLK_DIV_10 = 0x9,
		RCC_PREDIV_PCLK_DIV_11 = 0xA,
		RCC_PREDIV_PCLK_DIV_12 = 0xB,
		RCC_PREDIV_PCLK_DIV_13 = 0xC,
		RCC_PREDIV_PCLK_DIV_14 = 0xD,
		RCC_PREDIV_PCLK_DIV_15 = 0xE,
		RCC_PREDIV_PCLK_DIV_16 = 0xF,
} rcc_prediv_divider;

/**
 * @brief STM32F3 APB1 prescaler dividers
 * @see rcc_set_prescaler()
 */
typedef enum rcc_apb1_divider {
    RCC_APB1_HCLK_DIV_1  = RCC_CFGR_PPRE1_DIV1,
    RCC_APB1_HCLK_DIV_2  = RCC_CFGR_PPRE1_DIV2,
    RCC_APB1_HCLK_DIV_4  = RCC_CFGR_PPRE1_DIV4,
    RCC_APB1_HCLK_DIV_8  = RCC_CFGR_PPRE1_DIV8,
    RCC_APB1_HCLK_DIV_16 = RCC_CFGR_PPRE1_DIV16,
} rcc_apb1_divider;

/**
 * @brief STM32F3 APB2 prescaler dividers
 * @see rcc_set_prescaler()
 */
typedef enum rcc_apb2_divider {
    RCC_APB2_HCLK_DIV_1  = RCC_CFGR_PPRE2_DIV1,
    RCC_APB2_HCLK_DIV_2  = RCC_CFGR_PPRE2_DIV2,
    RCC_APB2_HCLK_DIV_4  = RCC_CFGR_PPRE2_DIV4,
    RCC_APB2_HCLK_DIV_8  = RCC_CFGR_PPRE2_DIV8,
    RCC_APB2_HCLK_DIV_16 = RCC_CFGR_PPRE2_DIV16,
} rcc_apb2_divider;

/**
 * @brief STM32F3 AHB prescaler dividers
 * @see rcc_set_prescaler()
 */
typedef enum rcc_ahb_divider {
    RCC_AHB_SYSCLK_DIV_1   = RCC_CFGR_HPRE_DIV1,
    RCC_AHB_SYSCLK_DIV_2   = RCC_CFGR_HPRE_DIV2,
    RCC_AHB_SYSCLK_DIV_4   = RCC_CFGR_HPRE_DIV4,
    RCC_AHB_SYSCLK_DIV_8   = RCC_CFGR_HPRE_DIV8,
    RCC_AHB_SYSCLK_DIV_16  = RCC_CFGR_HPRE_DIV16,
    RCC_AHB_SYSCLK_DIV_64  = RCC_CFGR_HPRE_DIV64,
    RCC_AHB_SYSCLK_DIV_128 = RCC_CFGR_HPRE_DIV128,
    RCC_AHB_SYSCLK_DIV_256 = RCC_CFGR_HPRE_DIV256,
    RCC_AHB_SYSCLK_DIV_512 = RCC_CFGR_HPRE_DIV512,
} rcc_ahb_divider;

typedef enum rcc_usb_divider {
    RCC_USB_SYSCLK_DIV_1 = 1 << RCC_CFGR_USBPRE_Pos,
    RCC_USB_SYSCLK_DIV_1_5 = 0 << RCC_CFGR_USBPRE_Pos,
} rcc_usb_divider;


/**
 * @brief STM32F3 clock sources.
 */
typedef enum rcc_clk {
    RCC_CLK_PLL    = (uint16)((offsetof(rcc_reg_def, CR) << 8) |
                              RCC_CR_PLLON_Pos), /**< Main PLL, clocked by
                                                    HSI or HSE. */
    RCC_CLK_HSE    = (uint16)((offsetof(rcc_reg_def, CR) << 8) |
                              RCC_CR_HSEON_Pos), /**< High speed external. */
    RCC_CLK_HSI    = (uint16)((offsetof(rcc_reg_def, CR) << 8) |
                              RCC_CR_HSION_Pos), /**< High speed internal. */
    RCC_CLK_LSE    = (uint16)((offsetof(rcc_reg_def, BDCR) << 8) |
                              RCC_BDCR_LSEON_Pos), /**< Low-speed external
                                                    * (32.768 KHz). */
    RCC_CLK_LSI    = (uint16)((offsetof(rcc_reg_def, CSR) << 8) |
                              RCC_CSR_LSION_Pos), /**< Low-speed internal
                                                   * (approximately 32 KHz). */
} rcc_clk;

/**
 * @brief STM32F3 PLL multipliers.
 */
typedef enum rcc_pll_multiplier {
    RCC_PLLMUL_2 = (0x0 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_3 = (0x1 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_4 = (0x2 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_5 = (0x3 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_6 = (0x4 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_7 = (0x5 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_8 = (0x6 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_9 = (0x7 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_10 = (0x8 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_11 = (0x9 << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_12 = (0xA << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_13 = (0xB << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_14 = (0xC << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_15 = (0xD << RCC_CFGR_PLLMUL_Pos),
    RCC_PLLMUL_16 = (0xE << RCC_CFGR_PLLMUL_Pos),
} rcc_pll_multiplier;

/* FIXME [0.0.13] Just have data point to an rcc_pll_multiplier! */

/**
 * @brief STM32F3 PLL configuration values.
 * Point to one of these with the "data" field in a struct rcc_pll_cfg.
 * @see struct rcc_pll_cfg.
 */
typedef struct stm32f3_rcc_pll_data {
     rcc_pll_multiplier pll_mul; /**< PLL multiplication factor. */
		 rcc_prediv_divider pclk_prediv; /**< PCLK predivider. */
} stm32f3_rcc_pll_data;


/* Note: Beyond the usual (registers, etc.), it's up to the series
 * header to define the following types:
 *
 * - enum rcc_clk: Available system and secondary clock sources,
 *   e.g. RCC_CLK_HSE, RCC_CLK_PLL, RCC_CLK_LSE.
 *
 *   Note that the inclusion of secondary clock sources (like LSI and
 *   LSE) makes enum rcc_clk different from the SYSCLK sources, which
 *   are defined in this header as enum rcc_sysclk_src.
 *
 *   IMPORTANT NOTE TO IMPLEMENTORS: If you are adding support for a
 *   new STM32 series, see the comment near rcc_clk_reg() in
 *   libmaple/rcc.c for information on how to choose these values so
 *   that rcc_turn_on_clk() etc. will work on your series.
 *
 * - enum rcc_clk_id: For each available peripheral. These are widely used
 *   as unique IDs (TODO extricate from RCC?). Peripherals which are
 *   common across STM32 series should use the same token for their
 *   rcc_clk_id in each series header.
 *
 * - enum rcc_clk_domain: For each clock domain. This is returned by
 *   rcc_dev_clk(). For instance, each AHB and APB is a clock domain.
 *
 * - enum rcc_prescaler: And a suitable set of dividers for
 *   rcc_set_prescaler().
 *
 * - enum rcc_pllsrc: For each PLL source. Same source, same token.
 *
 * - A target-dependent type to be pointed to by the data field in a
 *   struct rcc_pll_cfg.
 */

#ifdef __DOXYGEN__
/** RCC register map base pointer */
#define RCC_REGS
#endif


#include "rcc_private.h"


/* Clock prescaler management. */

/**
 * @brief Set the divider on a peripheral prescaler
 * @param prescaler prescaler to set
 * @param divider prescaler divider
 */
static inline void rcc_set_prescaler(rcc_prescaler prescaler, uint32 divider) {
    rcc_do_set_prescaler(prescaler, divider);
}

/* SYSCLK. */

void rcc_switch_sysclk(rcc_sysclk_src sysclk_src);

/* PLL configuration */

/**
 * @brief Specifies a configuration for the main PLL.
 */
typedef struct rcc_pll_cfg {
    rcc_pllsrc  pllsrc;     /**< PLL source */

    /** Series-specific configuration data. */
    void       *data;
} rcc_pll_cfg;

/**
 * @brief Configure the main PLL.
 *
 * You may only call this function while the PLL is disabled.
 *
 * @param pll_cfg Desired PLL configuration. The contents of this
 *                struct depend entirely on the target.
 */
extern void rcc_configure_pll(rcc_pll_cfg *pll_cfg);

/* System and secondary clock sources. */

void rcc_turn_on_clk(rcc_clk clock);
void rcc_turn_off_clk(rcc_clk clock);
int rcc_is_clk_on(rcc_clk clock);
int rcc_is_clk_ready(rcc_clk clock);

/* Peripheral clock lines and clock domains. */

/**
 * @brief Turn on the clock line on a peripheral
 * @param id Clock ID of the peripheral to turn on.
 */
extern void rcc_clk_enable(rcc_clk_id id);

/**
 * @brief Reset a peripheral.
 *
 * Caution: not all rcc_clk_id values refer to a peripheral which can
 * be reset. (Only rcc_clk_ids for peripherals with bits in an RCC
 * reset register can be used here.)
 *
 * @param id Clock ID of the peripheral to reset.
 */
static inline void rcc_reset_dev(rcc_clk_id id) {
    rcc_do_reset_dev(id);
}


/* Clock security system */

/**
 * @brief Enable the clock security system (CSS).
 */
static inline void rcc_enable_css() {
    RCC_REGS->CR |= RCC_CR_CSSON;
}

/**
 * @brief Disable the clock security system (CSS).
 */
static inline void rcc_disable_css() {
    RCC_REGS->CR &= ~RCC_CR_CSSON;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif
