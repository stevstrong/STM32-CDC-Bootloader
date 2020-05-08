/*
 * system.c
 *
 *  Created on: Apr 25, 2020
 *      Author: stevestrong
 */


#include "flash.h"
#include "rcc.h"
#include "gpio.h"
#include "board.h"


// Allow boards to provide a PLL multiplier. This is useful for
// e.g. STM32F100 value line MCUs, which use slower multipliers.
// (We're leaving the default to RCC_PLLMUL_9 for now, since that
// works for F103 performance line MCUs, which is all that LeafLabs
// currently officially supports).
#ifndef BOARD_RCC_PLLMUL
  #if !USE_HSI_CLOCK
	#if F_CPU==128000000
		#define BOARD_RCC_PLLMUL RCC_PLLMUL_16
	#elif F_CPU==72000000
		#define BOARD_RCC_PLLMUL RCC_PLLMUL_9
	#elif F_CPU==48000000
		#define BOARD_RCC_PLLMUL RCC_PLLMUL_6
	#elif F_CPU==16000000
		#define BOARD_RCC_PLLMUL RCC_PLLMUL_2
	#endif
  #else
	#define BOARD_RCC_PLLMUL RCC_PLLMUL_16
  #endif
#endif


	static stm32f1_rcc_pll_data pll_data = {BOARD_RCC_PLLMUL};
#if !USE_HSI_CLOCK
	__weak rcc_pll_cfg w_board_pll_cfg = {RCC_PLLSRC_HSE, &pll_data};
#else
	__weak rcc_pll_cfg w_board_pll_cfg = {RCC_PLLSRC_HSI_DIV_2, &pll_data};
#endif

void board_setup_clock_prescalers(void) {
	rcc_set_prescaler(RCC_PRESCALER_AHB, RCC_AHB_SYSCLK_DIV_1);
	rcc_set_prescaler(RCC_PRESCALER_APB1, RCC_APB1_HCLK_DIV_2);
	rcc_set_prescaler(RCC_PRESCALER_APB2, RCC_APB2_HCLK_DIV_1);
	rcc_clk_disable(RCC_USB);
	#if F_CPU == 72000000
	rcc_set_prescaler(RCC_PRESCALER_USB, RCC_USB_SYSCLK_DIV_1_5);
	#elif F_CPU == 48000000
	rcc_set_prescaler(RCC_PRESCALER_USB, RCC_USB_SYSCLK_DIV_1);
	#endif
}

//-----------------------------------------------------------------------------
void Setup_flash(void)
{
	// Turn on as many Flash "go faster" features as
	// possible. flash_enable_features() just ignores any flags it
	// can't support.
	flash_enable_features(FLASH_PREFETCH | FLASH_ICACHE | FLASH_DCACHE);
	// Configure the wait states, assuming we're operating at "close
	// enough" to 3.3V.
	flash_set_latency(FLASH_SAFE_WAIT_STATES);
}
//-----------------------------------------------------------------------------
void Setup_clocks(void)
{
#if 1
	// Turn on HSI. We'll switch to and run off of this while we're
	// setting up the main PLL.
	rcc_turn_on_clk(RCC_CLK_HSI);

	// Turn off and reset the clock subsystems we'll be using, as well
	// as the clock security subsystem (CSS). Note that resetting CFGR
	// to its default value of 0 implies a switch to HSI for SYSCLK.
	RCC->CFGR = 0x00000000;
	rcc_disable_css();
	rcc_turn_off_clk(RCC_CLK_PLL);
	rcc_turn_off_clk(RCC_CLK_HSE);
	// Clear clock readiness interrupt flags and turn off clock
	// readiness interrupts.
	RCC->CIR = 0x00000000;
#if !USE_HSI_CLOCK
	// Enable HSE, and wait until it's ready.
	rcc_turn_on_clk(RCC_CLK_HSE);
	while (!rcc_is_clk_ready(RCC_CLK_HSE))
		;
#endif
	// Configure AHBx, APBx, etc. prescalers and the main PLL.
	board_setup_clock_prescalers();
	rcc_configure_pll(&w_board_pll_cfg);

	// Enable the PLL, and wait until it's ready.
	rcc_turn_on_clk(RCC_CLK_PLL);
	while(!rcc_is_clk_ready(RCC_CLK_PLL))
		;

	// Finally, switch to the now-ready PLL as the main clock source.
	rcc_switch_sysclk(RCC_CLKSRC_PLL);
#else
	/* Enable HSE */
	SET_BIT(RCC->CR, RCC_CR_HSEON);

	/* Wait until HSE is ready */
	while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0) {
		;
	}

	/* Enable Prefetch Buffer & set Flash access to 2 wait states */
//	SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2);

	/* SYSCLK = PCLK2 = HCLK */
	/* PCLK1 = HCLK / 2 */
	/* PLLCLK = HSE * 9 = 72 MHz */
	SET_BIT(RCC->CFGR,
		RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV2 |
		RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

	/* Enable PLL */
	SET_BIT(RCC->CR, RCC_CR_PLLON);

	/* Wait until PLL is ready */
	while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0) {
		;
	}

	/* Select PLL as system clock source */
	SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

	/* Wait until PLL is used as system clock source */
	while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS_1) == 0) {
		;
	}
#endif
}


