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
 * @file libmaple/stm32f3/include/series/usart.h
 * @author F3-port by Hanspeter Portner <dev@open-music-kontrollers.ch>
 * @brief STM32F3 USART support.
 */

#ifndef _LIBMAPLE_STM32F3_USART_H_
#define _LIBMAPLE_STM32F3_USART_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "libmaple_types.h"
#include "stm32f3xx.h"
#include "gpio.h"
#include "util.h"
#include "rcc.h"
#include "nvic.h"
#include "ring_buffer.h"
#include "..\cdc.h"


/*
 * Devices
 */

#ifndef USART_RX_BUF_SIZE
#define USART_RX_BUF_SIZE               64
#endif

#ifndef USART_TX_BUF_SIZE
#define USART_TX_BUF_SIZE               64
#endif


/*
 * Devices
 */
/** USART device type */
typedef struct usart_dev {
    usart_reg_def *regs;             /**< Register map */
    ring_buffer_t *rb;                 /**< RX ring buffer */
    ring_buffer_t *wb;                 /**< TX ring buffer */
    rcc_clk_id clk_id;               /**< RCC clock information */
    nvic_irq_num irq_num;            /**< USART NVIC interrupt */
} usart_dev;


extern const usart_dev usart1;
extern const usart_dev usart2;
extern const usart_dev usart3;
#define USART1 (&usart1)
#define USART2 (&usart2)
#define USART3 (&usart3)

#ifdef STM32_HIGH_DENSITY
extern const usart_dev uart4;
extern const usart_dev uart5;
#define UART4 (&uart4)
#define UART5 (&uart5)
#endif




/*
 * F3-only register bit definitions.
 */


/* Interrupt and status register */
// common register bits with other STM32 series are defined as USART_SR_* for compatibility 
#define USART_SR_REACK_BIT					22
#define USART_SR_TEACK_BIT					21
#define USART_SR_WUF_BIT						20
#define USART_SR_RWU_BIT						19
#define USART_SR_SBKF_BIT						18
#define USART_SR_CMF_BIT						17
#define USART_SR_BUSY_BIT						16
#define USART_SR_ABRF_BIT						15
#define USART_SR_ABRE_BIT						14
#define USART_SR_EOBF_BIT						12
#define USART_SR_RTOF_BIT						11
#define USART_SR_CTS_BIT						10
#define USART_SR_CTSIF_BIT					9

#define USART_SR_REACK				(1UL << USART_ISR_REACK_BIT)
#define USART_SR_TEACK        	    (1UL << USART_ISR_TEACK_BIT)
#define USART_SR_WUF          	    (1UL << USART_ISR_WUF_BIT)
#define USART_SR_RWU          	    (1UL << USART_ISR_RWU_BIT)
#define USART_SR_SBKF         	    (1UL << USART_ISR_SBKF_BIT)
#define USART_SR_CMF          	    (1UL << USART_ISR_CMF_BIT)
#define USART_SR_BUSY         	    (1UL << USART_ISR_BUSY_BIT)
#define USART_SR_ABRF         	    (1UL << USART_ISR_ABRF_BIT)
#define USART_SR_ABRE         	    (1UL << USART_ISR_ABRE_BIT)
#define USART_SR_EOBF         	    (1UL << USART_ISR_EOBF_BIT)
#define USART_SR_RTOF         	    (1UL << USART_ISR_RTOF_BIT)
#define USART_SR_CTS	        	   (1UL << USART_ISR_CTS_BIT)
#define USART_SR_CTSIF        	    (1UL << USART_ISR_CTSIF_BIT)

/* Interrupt clear register */
#define USART_ICR_WUFCF_BIT					20
#define USART_ICR_CMCF_BIT					17
#define USART_ICR_EOBCF_BIT					12
#define USART_ICR_RTOCF_BIT					11
#define USART_ICR_CTSCF_BIT					9
#define USART_ICR_LBDCF_BIT					8
#define USART_ICR_TCCF_BIT					6
#define USART_ICR_IDLECF_BIT				4
#define USART_ICR_ORECF_BIT					3
#define USART_ICR_NCF_BIT					2
#define USART_ICR_FECF_BIT					1
#define USART_ICR_PECF_BIT					0

#define USART_ICR_WUFCF        			(1UL << USART_ICR_WUFCF_BIT)




#define SERIAL_8N1	0B00000000
#define SERIAL_8N2	0B00100000
#define SERIAL_9N1	0B00001000
#define SERIAL_9N2	0B00101000

#define SERIAL_8E1	0B00001010
#define SERIAL_8E2	0B00101010
/* not supported:
#define SERIAL_9E1	0B00001010
#define SERIAL_9E2	0B00101010
*/
#define SERIAL_8O1	0B00001011
#define SERIAL_8O2	0B00101011
/* not supported:
#define SERIAL_9O1	0B00001011
#define SERIAL_9O2	0B00101011
*/

// Functions

void usart_init(const usart_dev *dev);


/**
 * @brief Configure GPIOs for use as USART TX/RX.
 * @param udev USART device to use
 * @param rx_dev RX pin gpio_dev
 * @param rx     RX pin bit on rx_dev
 * @param tx_dev TX pin gpio_dev
 * @param tx     TX pin bit on tx_dev
 * @param flags  Currently ignored
 */

#define USART_USE_PCLK 0
void usart_set_baud_rate(const usart_dev *dev, uint32 baud);
void usart_config(const usart_dev *udev, uint8 rx_pin, uint8 tx_pin, uint8_t flags);

void usart_enable(const usart_dev *dev);
void usart_disable(const usart_dev *dev);
void usart_foreach(void (*fn)(const usart_dev *dev));
/**
 * @brief Nonblocking USART transmit
 * @param dev Serial port to transmit over
 * @param buf Buffer to transmit
 * @param len Maximum number of bytes to transmit
 * @return Number of bytes transmitted
 */
uint16 usart_tx(const usart_dev *dev, const uint8 *buf, uint16 len);
uint16 usart_rx(const usart_dev *dev, uint8 *buf, uint16 len);
void usart_putudec(const usart_dev *dev, uint32 val);


/**
 * @brief Disable all serial ports.
 */
static inline void usart_disable_all(void) {
    usart_foreach(usart_disable);
}

/**
 * @brief Transmit one character on a serial port.
 *
 * This function blocks until the character has been successfully
 * transmitted.
 *
 * @param dev Serial port to send on.
 * @param byte Byte to transmit.
 */
static inline void usart_putc(const usart_dev* dev, uint8 byte) {
    while (!usart_tx(dev, &byte, 1))
        ;
}

/**
 * @brief Transmit a character string on a serial port.
 *
 * This function blocks until str is completely transmitted.
 *
 * @param dev Serial port to send on
 * @param str String to send
 */
static inline void usart_putstr(const usart_dev *dev, const char* str) {
    uint32 i = 0;
    while (str[i] != '\0') {
        usart_putc(dev, str[i++]);
    }
}

/**
 * @brief Read one character from a serial port.
 *
 * It's not safe to call this function if the serial port has no data
 * available.
 *
 * @param dev Serial port to read from
 * @return byte read
 * @see usart_data_available()
 */
static inline uint8 usart_getc(const usart_dev *dev) {
    return rb_read(dev->rb);
}

/*
 * Roger Clark. 20141125, 
 * added peek function.
 * @param dev Serial port to read from
 * @return byte read
 */
static inline int usart_peek(const usart_dev *dev)
{
	return rb_peek(dev->rb);
}

/**
 * @brief Return the amount of data available in a serial port's RX buffer.
 * @param dev Serial port to check
 * @return Number of bytes in dev's RX buffer.
 */
static inline uint32 usart_rx_available(const usart_dev *dev) {
    return rb_read_available(dev->rb);
}

/**
 * @brief Discard the contents of a serial port's RX buffer.
 * @param dev Serial port whose buffer to empty.
 */
static inline void usart_reset_rx(const usart_dev *dev) {
    rb_reset(dev->rb);
}
/*
 * Routines
 */

gpio_af usart_get_af(const usart_dev *dev);

#ifdef __cplusplus
}
#endif

#endif
