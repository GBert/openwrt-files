/*
 * Copyright (C) 2005-2014 Darron Broad
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name `Darron Broad' nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "k8048.h"

/*
 * Open serial port
 *
 * For bit-bang I/O use speed B0.
 *
 *  return file descriptor on success or -1 on error
 */
int
serial_open(const char *device, speed_t speed)
{
	int serial_port;
	struct termios options;

	serial_port = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (serial_port < 0)
		return -1;

	fcntl(serial_port, F_SETFL, O_NONBLOCK);

	/* Get options */
	tcgetattr(serial_port, &options);

	/*
	 * Raw mode
	 *
	 *  Linux TERMIOS(3)
	 */

	/* Input options */
	options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP |
		INLCR | IGNCR | ICRNL | IXON);

	/* Output options */
	options.c_oflag &= ~(OPOST);

	/* Control options */
	options.c_cflag &= ~(CSIZE | PARENB);
	options.c_cflag |= (CS8);

	/* Local options */
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

	/* Control characters */
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;

	/* Baud rate */
	cfsetispeed(&options, speed);
	cfsetospeed(&options, speed);

	/* Set options */
	tcsetattr(serial_port, TCSANOW, &options);

	/* Discard */
	tcflush(serial_port, TCIOFLUSH);

	return serial_port;
}

/*
 * Close serial port
 */
void
serial_close(int serial_port)
{
	tcdrain(serial_port);
	close(serial_port);
}

/*
 * Get CTS (input set when +ve) (DATA_IN)
 */
int
get_cts(int serial_port)
{
	int status;

	ioctl(serial_port, TIOCMGET, &status);

	return (status & TIOCM_CTS) ? (HIGH) : (LOW);
}

/*
 * Set DTR (output +ve on set) (DATA_OUT)
 */
void
set_dtr(int serial_port, int dtr)
{
	int status;

	ioctl(serial_port, TIOCMGET, &status);

	if (dtr)
		status |= TIOCM_DTR;
	else
		status &= ~TIOCM_DTR;

	ioctl(serial_port, TIOCMSET, &status);
}

/*
 * Set RTS (output +ve on set) (CLOCK)
 */
void
set_rts(int serial_port, int rts)
{
	int status;

	ioctl(serial_port, TIOCMGET, &status);

	if (rts)
		status |= TIOCM_RTS;
	else
		status &= ~TIOCM_RTS;

	ioctl(serial_port, TIOCMSET, &status);
}

/*
 * Set Tx (output +ve on set) (VPP)
 */
void
set_tx(int serial_port, int tx)
{
	if (tx)
		ioctl(serial_port, TIOCSBRK, 0); /* +ve */
	else
		ioctl(serial_port, TIOCCBRK, 0); /* -ve */
}

/*
 * return speed_t for given baud rate
 */
speed_t
serial_speed(uint32_t baudrate)
{
	static uint32_t rates[] = {
		0, 75, 110, 300, 1200, 2400, 4800, 9600,
		19200, 38400, 57600, 115200, UINT32_MAX
	};
	static speed_t speeds[] = {
		B0, B75, B110, B300, B1200, B2400, B4800, B9600,
		B19200, B38400, B57600, B115200, B115200
	};
	int i = 0;
	while (baudrate > rates[i++])
		;
	return speeds[--i];
}
