/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Bryan Newbold.
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
 * @file wirish/include/wirish/boards.h
 * @author Bryan Newbold <bnewbold@leaflabs.com>,
 *         Marti Bolivar <mbolivar@leaflabs.com>
 * @brief init() and board-specific pin information.
 */

#ifndef _WIRISH_BOARDS_H_
#define _WIRISH_BOARDS_H_

#include <libmaple_types.h>
#include <wirish_types.h>
#include <board.h>



/**
 * @brief Pins capable of PWM output.
 *
 * Its length is BOARD_NR_PWM_PINS.
 */
extern const uint8 boardPWMPins[];

/**
 * @brief Array of pins capable of analog input.
 *
 * Its length is BOARD_NR_ADC_PINS.
 */
extern const uint8 boardADCPins[];

/**
 * @brief Pins which are connected to external hardware.
 *
 * For example, on Maple boards, it always at least includes
 * PB1 for the LED.  Its length is BOARD_NR_USED_PINS.
 */
extern const uint8 boardUsedPins[];

/**
 * @brief Generic board initialization function.
 *
 * This function is called before main().  It ensures that the clocks
 * and peripherals are configured properly for use with wirish, then
 * calls boardInit().
 *
 * @see boardInit()
 */
void init(void);

/**
 * @brief Board-specific initialization function.
 *
 * This function is called from init() after all generic board
 * initialization has been performed.  Each board is required to
 * define its own.
 *
 * @see init()
 */
extern void boardInit(void);

/**
 * @brief Test if a pin is used for a special purpose on your board.
 * @param pin Pin to test
 * @return true if the given pin is in boardUsedPins, and false otherwise.
 * @see boardUsedPins
 */
uint8 boardUsesPin(uint8 pin);

/*
 * Derived and default board definitions
 */

#define CLOCK_SPEED_MHZ                 CYCLES_PER_MICROSECOND
#define CLOCK_SPEED_HZ                  (CLOCK_SPEED_MHZ * 1000000UL)

#ifndef SYSTICK_RELOAD_VAL
#define SYSTICK_RELOAD_VAL              (1000 * CYCLES_PER_MICROSECOND - 1)
#endif

#ifndef BOARD_BUTTON_PRESSED_LEVEL
#define BOARD_BUTTON_PRESSED_LEVEL      HIGH
#endif

/**
 * @brief Does the board break out a USART/UART's RX and TX pins?
 *
 * BOARD_HAVE_USART(n) is nonzero iff USARTn is available (n must be
 * an integer literal, 1 through 6). Also see BOARD_HAVE_USART1, ...,
 * BOARD_HAVE_UART4 (sic), etc.
 */
#define BOARD_HAVE_USART(n) ((BOARD_USART##n##_TX_PIN) + (BOARD_USART##n##_RX_PIN))
/** Feature test: nonzero iff the board has USART1. */
#define BOARD_HAVE_USART1               BOARD_HAVE_USART(1)
/** Feature test: nonzero iff the board has USART2, 0 otherwise. */
#define BOARD_HAVE_USART2               BOARD_HAVE_USART(2)
/** Feature test: nonzero iff the board has USART3, 0 otherwise. */
#define BOARD_HAVE_USART3               BOARD_HAVE_USART(3)
/** Feature test: nonzero iff the board has UART4, 0 otherwise. */
#define BOARD_HAVE_UART4                BOARD_HAVE_USART(4)
/** Feature test: nonzero iff the board has UART5, 0 otherwise. */
#define BOARD_HAVE_UART5                BOARD_HAVE_USART(5)
/** Feature test: nonzero iff the board has USART6, 0 otherwise. */
#define BOARD_HAVE_USART6               BOARD_HAVE_USART(6)

/**
 * @brief Does the board break out a SPI peripheral's pins?
 *
 * BOARD_HAVE_SPI(n) is nonzero iff SPIn is available (n must be an
 * integer literal: 1, 2, or 3). Also see BOARD_HAVE_SPI1,
 * BOARD_HAVE_SPI2, BOARD_HAVE_SPI3. */
#define BOARD_HAVE_SPI(n) (defined(BOARD_SPI##n##_NSS_PIN)  &&          \
                           defined(BOARD_SPI##n##_SCK_PIN)  &&          \
                           defined(BOARD_SPI##n##_MISO_PIN) &&          \
                           defined(BOARD_SPI##n##_MOSI_PIN))
/** Feature test: nonzero iff the board has SPI1. */
#define BOARD_HAVE_SPI1 BOARD_HAVE_SPI(1)
/** Feature test: nonzero iff the board has SPI2. */
#define BOARD_HAVE_SPI2 BOARD_HAVE_SPI(2)
/** Feature test: nonzero iff the board has SPI3. */
#define BOARD_HAVE_SPI3 BOARD_HAVE_SPI(3)

/**
 * @brief Feature test: nonzero iff the board has SerialUSB.
 */
 //Roger Clark. Change so that BOARD_HAVE_SERIALUSB is always true, so that it can be controller by -DSERIAL_USB
#define BOARD_HAVE_SERIALUSB 1

/*(defined(BOARD_USB_DISC_DEV) && defined(BOARD_USB_DISC_BIT))*/

#endif
