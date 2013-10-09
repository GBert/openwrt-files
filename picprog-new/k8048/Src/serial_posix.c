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

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "k8048.h"

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int
open_port(const char *serial_device)
{
	int fd; /* File descriptor for the port */
	struct termios options;

	fd = open(serial_device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
		return -1;
	else
		fcntl(fd, F_SETFL, O_NONBLOCK);

	/*
	 * Get the current options for the port...
	 */
	tcgetattr(fd, &options);

	/*
	 * Enable the receiver 
	 */
	options.c_cflag = IGNBRK | IGNPAR | CS8 | CREAD | CLOCAL;

	/*
	 * Reset other options
	 */
	options.c_oflag = 0;
	options.c_lflag = 0;
	
	/*
	 * Set the new options for the port...
	 */
	tcsetattr(fd, TCSANOW, &options);

	return (fd);
}

/*
 * Get CTS (input set when +ve)
 */
int
get_cts(int fd)
{
	int status;

	ioctl(fd, TIOCMGET, &status);

	return status & TIOCM_CTS;
}

/*
 * Set DTR (output +ve on set)
 */
void
set_dtr(int fd, int dtr)
{
	int status;

	ioctl(fd, TIOCMGET, &status);

	if (dtr)
		status |= TIOCM_DTR;
	else
		status &= ~TIOCM_DTR;

	ioctl(fd, TIOCMSET, &status);
}

/*
 * Set RTS (output +ve on set)
 */
void
set_rts(int fd, int rts)
{
	int status;

	ioctl(fd, TIOCMGET, &status);

	if (rts)
		status |= TIOCM_RTS;
	else
		status &= ~TIOCM_RTS;

	ioctl(fd, TIOCMSET, &status);
}

/*
 * Set Tx 
 */
void
set_tx(int fd, int tx)
{
	if (tx)
		ioctl(fd, TIOCSBRK, 0); /* +ve */
	else
		ioctl(fd, TIOCCBRK, 0); /* -ve */
}

/*
 * Set DTR/RTS (output +ve on set)
 */
void
set_dtr_rts(int fd, int dtr, int rts)
{
	int status;

	ioctl(fd, TIOCMGET, &status);

	if (dtr)
		status |= TIOCM_DTR;
	else
		status &= ~TIOCM_DTR;
	if (rts)
		status |= TIOCM_RTS;
	else
		status &= ~TIOCM_RTS;
	
	ioctl(fd, TIOCMSET, &status);
}

#if 0
/*
 * Print status as human readable
 */
void
print_status(int status)
{
	if (status & TIOCM_LE)	printf("DSR (data set ready/line enable)\n");
	if (status & TIOCM_DTR)	printf("DTR (data terminal ready)\n");
	if (status & TIOCM_RTS)	printf("RTS (request to send)\n");
	if (status & TIOCM_ST)	printf("Secondary TXD (transmit)\n");
	if (status & TIOCM_SR)	printf("Secondary RXD (receive)\n");
	if (status & TIOCM_CTS)	printf("CTS (clear to send)\n");
	if (status & TIOCM_CAR)	printf("DCD (data carrier detect)\n");
	if (status & TIOCM_CD)	printf("Synonym for TIOCM_CAR\n");
	if (status & TIOCM_RNG)	printf("RNG (ring)\n");
	if (status & TIOCM_RI)	printf("Synonym for TIOCM_RNG\n");
	if (status & TIOCM_DSR)	printf("DSR (data set ready)\n");
}
#endif
