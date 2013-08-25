/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _IO_H
#define _IO_H

/*
 * busy cursor
 */
#define BUSY (8)

/*
 * command i/o
 */
#define RESYNCTIME (1000000) /* 1 second */

#define EOT (0x04)
#define ACK (0x06)
#define NAK (0x15)

/*
 * CONSTANT CMD_LED        = 0x01  ; 0x00..0x3F        SET K8048 LEDS
 * CONSTANT CMD_SWITCH     = 0x02  ;                   GET K8048 SWITCHES
 * CONSTANT CMD_SLEEP      = 0x10  ;                   SLEEP UNTIL WATCHDOG TIME-OUT
 * CONSTANT CMD_WATCHDOG   = 0x11  ; 1|0               SET WATCHDOG ENABLE/DISABLE
 * CONSTANT CMD_CLOCK      = 0x12  ; 0..7              SET INTERNAL RC CLOCK DIVIDER
 * CONSTANT CMD_DIRECTION  = 0x20  ; 0..5 0x00..0xFF   SET PORT DATA DIRECTION
 * CONSTANT CMD_OUTPUT     = 0x21  ; 0..5 0x00..0xFF   SET PORT DATA OUTPUT
 * CONSTANT CMD_INPUT      = 0x22  ; 0..5              GET PORT DATA INPUT
 * CONSTANT CMD_ANALOG     = 0x30  ; 0..N|0xFF         SET ANALOG CHANNEL ENABLE/DISABLE
 * CONSTANT CMD_SAMPLE     = 0x31  ;                   GET ANALOG CHANNEL INPUT
 * CONSTANT CMD_VREF       = 0x38  ; 0..15|0xFF        SET VREF VOLTAGE LEVEL OR DISABLE
 * CONSTANT CMD_EEREAD     = 0x40  ; ADDRESS           READ DATA EEPROM
 * CONSTANT CMD_EEWRITE    = 0x41  ; ADDRESS DATA      WRITE DATA EEPROM
 * CONSTANT CMD_READ       = 0x50  ; ADDRESH ADDRESL   READ PROGRAM FLASH
 * CONSTANT CMD_WRITE      = 0x51  ; 		       UNIMPLIMENTED
 * CONSTANT CMD_TEST       = 0xFE  ; 0x00..0xFF        TEST COMMAND
 * CONSTANT CMD_ERROR      = 0xFF  ;                   GET LAST FIRMWARE ERROR
 */
#define CMD_LED       (0x01)
#define CMD_SWITCH    (0x02)
#define CMD_SLEEP     (0x10)
#define CMD_WATCHDOG  (0x11)
#define CMD_CLOCK     (0x12)
#define CMD_DIRECTION (0x20)
#define CMD_OUTPUT    (0x21)
#define CMD_INPUT     (0x22)
#define CMD_ANALOG    (0x30)
#define CMD_SAMPLE    (0x31)
#define CMD_VREF      (0x38)
#define CMD_EEREAD    (0x40)
#define CMD_EEWRITE   (0x41)
#define CMD_READ      (0x50)
#define CMD_WRITE     (0x51)
#define CMD_TEST      (0xFE)
#define CMD_ERROR     (0xFF)

/* Analog sample delay */
#define FWSAMPLE  (200)
/* EEPROM write delay */
#define FWEEPROM  (200)

/*
 * CONSTANT ERRNONE     = 0    ; OK
 * CONSTANT ERRTIMEOUT  = 1    ; CLOCK TIMED OUT
 * CONSTANT ERRPROTOCOL = 2    ; INVALID STOP OR START BIT
 * CONSTANT ERRPARITY   = 3    ; INVALID PARITY BIT
 * CONSTANT ERRNOTSUP   = 4    ; COMMAND NOT SUPPORTED
 * CONSTANT ERRINVALID  = 5    ; INVALID RESPONSE
 */
#define ERRNONE     (0)
#define ERRTIMEOUT  (1)
#define ERRPROTOCOL (2)
#define ERRPARITY   (3)
#define ERRNOTSUP   (4)
#define ERRINVALID  (5)

/*
 * prototypes
 */
void io_signal();
void io_signal_on();
void io_signal_off();

void io_cursor(struct k8048 *, char);

#endif /* !_IO_H */
