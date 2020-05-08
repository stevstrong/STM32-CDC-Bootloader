/* USB als virtuellen COM Port betreiben */

#ifndef __USB_H__
#define __USB_H__

#include <stdint.h>
#include <stdbool.h>

#include "ring_buffer.h"
#include "usb_func.h"

// storage buffers
extern ring_buffer_t rxBuf;
extern ring_buffer_t txBuf;

extern status_t usb_state;

/* Liste der von aussen zu benutzenden Funktionen */
int UsbRxAvail(void);    /* nr. of caharacters received */
int UsbGetChar(void);   /* liest ein Char vom Host, returns -1 if not available */
bool UsbActive (void);   /* true, wenn der Host per SET_CONFIGURATION eine Konfiguration aktiviert hat und das Ger‰t nicht im Suspend ist. Ansonsten ist keine VCP-Kommunikation m√∂glich. */
bool UsbTxReady(void);   /* true, wenn mindestens 1 Char gesendet werden kann */
bool UsbTxEmpty(void);   /* true, wenn der Sendepuffer total leer ist */
uint16 UsbTxFree(void);     /* Anzahl freier Pl‰tze im Sendepuffer */
bool UsbPutChar(char c); /* sendet ein Char zum Host */
void UsbPutStr(char* S); /* sendet einen String zum Host */
void UsbTxFlush (void);  /* Sende alles im Puffer befindliche ab (asynchron, nicht-blockierend) */
void UsbSetup(void); /* Starten des USB-Cores */

#endif
