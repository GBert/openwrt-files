/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 *
 * See README.serial
 */

#ifndef _SERIAL_POSIX_H
#define _SERIAL_POSIX_H

/* linux/cygwin */
#define SERIAL_DEVICE "/dev/ttyS0"

int open_port(const char *);
int get_cts(int);
void set_dtr(int, int);
void set_rts(int, int);
void set_tx(int, int);
void set_dtr_rts(int, int, int);
void print_status(int);

#endif
