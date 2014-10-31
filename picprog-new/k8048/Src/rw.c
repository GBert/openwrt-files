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
 * Read/Write buffered I/O
 */

/*
 * Read at most buflen characters into buffer
 *
 *  The number of characters read is not guaranteed to be greater than one
 *  regardless of the number requested.
 *
 *  return number of bytes or error
 */
int
rw_get(int fsock, char *buffer, int buflen, int timeout)
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
		rc = read(fsock, buffer, buflen);
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
 * Read buflen characters into buffer
 *
 *  return number of bytes or error
 */
int
rw_read(int fsock, char *buffer, int buflen, int timeout)
{
	int rc, nb = 0;

	do {
		rc = rw_get(fsock, &buffer[nb], buflen - nb, timeout);
		if (rc < 0)
			return rc;
		nb += rc;
	}
	while (nb < buflen);
#if 0
	if (nb > buflen) {
		return -EIO;
	}
#endif
	return nb;
}

/*
 * Write buflen characters from buffer
 *
 *  return number of bytes or error
 */
int
rw_write(int fsock, char *buffer, int buflen, int timeout)
{
	fd_set fdset;
	struct timeval timeval;
	int rc = 0, nb = 0;

	while (nb < buflen) {
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
		rc = write(fsock, &buffer[nb], buflen - nb);
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
	if (nb > buflen) {
		return -EIO;
	}
#endif
	return nb;
}
