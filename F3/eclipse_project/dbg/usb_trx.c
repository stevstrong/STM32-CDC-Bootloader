
#include "usb_def.h"
#include "usb_trx.h"
#include "usb_func.h"

// ring buffers for USB In und Out data
RING_BUFFER(rxBuf, 256);
RING_BUFFER(txBuf, 256);


#ifdef USB_DEBUG
#define DBG_SIZE 10000
	char dBuf[DBG_SIZE];
	int dLen = 0;
#define DIGITS_SIZE 10
	char digits[DIGITS_SIZE];

void DoTrace(char * s, int len)
{
	if ( (dLen+len)>DBG_SIZE )
		return;

	char* dest = &dBuf[dLen];
	dLen += len;
	const char* src = s;
	while (len--)
		*dest++ = *src++;
}
void NrTrace(int val, int nl)
{
	int i = DIGITS_SIZE-1;
	if (nl)
		digits[i--] = '\n';
	do {
		digits[i--] = val % 10 + '0';
		val /= 10;
	} while (val > 0 && i>=0);
	i++;
	DoTrace(&digits[i], DIGITS_SIZE-i);
}
#endif

// USB serial related routines
// These functions are used on main level

// Returns the number of available bytes received by USB
int UsbRxAvail(void)
{
    return rb_read_available(&rxBuf);
}

// Reads a character from the USB receive buffer.
// Returns -1 if none received
int UsbGetChar(void)
{
	return rb_read_safe(&rxBuf);
}

// Returns true if the USB has been started and configured.
// Data transmission is only possible this returns true.
// The main level application should wait for this condition before performing any communication.
bool UsbActive (void)
{
	status_t state = { usb_state.both }; // read status with one instruction
    return ( state.configured && !state.suspended );
}

// Returns the number of available free slots in the transmit buffer
uint16 UsbTxFree(void)
{
    return rb_write_available(&txBuf);
}

// Returns true if there is not data to send
bool UsbTxEmpty(void)
{
    return rb_is_empty(&txBuf);
}
/*
// send everything from the Tx buffer
void UsbTxFlush (void)
{
	DataBeginTransmit ();
}

// Transmits a character over the USB serial interface
bool UsbPutChar(char c)
{
	if ( !rb_write_safe(&txBuf, c) )
		return false;

	DataBeginTransmit ();

    return true;
}
*/
// Transmits a string over the USB serial interface
void UsbPutStr(char* S)
{
    while (*S && UsbPutChar(*S++));
}

