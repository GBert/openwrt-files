/*
 * Copyright (C) 2005-2020 Darron Broad
 * All rights reserved.
 *
 * This file is part of Pickle Microchip PIC ICSP.
 *
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation.
 *
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "pickle.h"

/*
 * Read/Write buffered I/O
 */

/*
 * Read at most bufmax characters into buffer
 *
 *  The number of characters read is not guaranteed to be greater than one
 *  regardless of the number requested.
 *
 *  return number of bytes or error
 */
int
rw_get(int fsock, char *buffer, int bufmax, int timeout)
{
	struct timeval timeval;
	fd_set fdset;
	int rc = 0;

	while (1) {
		timeval.tv_sec = timeout;
		timeval.tv_usec = 0;

		FD_ZERO(&fdset);
		FD_SET(fsock, &fdset);

		rc = select(fsock + 1, &fdset, NULL, NULL, &timeval);
		if (rc < 0) {
			if (errno == EINTR) {
				continue;
			}
			return -EIO;
		}
		if (rc != 1) {
			return -ETIMEDOUT;
		}
		if (!FD_ISSET(fsock, &fdset)) {
			return -ETIMEDOUT;
		}
		rc = read(fsock, buffer, bufmax);
		if (rc < 0) {
			if (errno == EINTR || errno == EAGAIN) {
				continue;
			}
			return -EIO;
		}
		if (rc == 0)
			return -EIO; /* Connection reset */

		return rc;
	}
	/* Not reached */
}

/*
 * Read bufmax characters into buffer
 *
 *  return number of bytes or error
 */
int
rw_read(int fsock, char *buffer, int bufmax, int timeout)
{
	int rc, nb = 0;

	do {
		rc = rw_get(fsock, &buffer[nb], bufmax - nb, timeout);
		if (rc < 0)
			return rc;
		nb += rc;
	}
	while (nb < bufmax);
#if 0
	if (nb > bufmax) {
		return -EIO;
	}
#endif
	return nb;
}

/*
 * Write bufmax characters from buffer
 *
 *  return number of bytes or error
 */
int
rw_write(int fsock, char *buffer, int bufmax, int timeout)
{
	fd_set fdset;
	struct timeval timeval;
	int rc = 0, nb = 0;

	while (nb < bufmax) {
		timeval.tv_sec = timeout;
		timeval.tv_usec = 0;

		FD_ZERO(&fdset);
		FD_SET(fsock, &fdset);

		rc = select(fsock + 1, NULL, &fdset, NULL, &timeval);
		if (rc < 0) {
			if (errno == EINTR) {
				continue;
			}
			return -EIO;
		}
		if (rc != 1) {
			return -ETIMEDOUT;
		}
		if (!FD_ISSET(fsock, &fdset)) {
			return -ETIMEDOUT;
		}
		rc = write(fsock, &buffer[nb], bufmax - nb);
		if (rc < 0) {
			if (errno == EINTR || errno == EAGAIN) {
				continue;
			}
			return -EIO;
		}
		if (rc == 0)
			return -EIO; /* Connection reset */

		nb += rc;
	}
#if 0
	if (nb > bufmax) {
		return -EIO;
	}
#endif
	return nb;
}
