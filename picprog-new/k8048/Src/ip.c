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
 * Connect to port on host
 */
int
ip_connect(const char *hostname, int port)
{
	struct sockaddr_in addr;
	struct hostent *host;
	int sock;

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if ((addr.sin_addr.s_addr = inet_addr(hostname)) == htonl(INADDR_NONE)) {
		if ((host = gethostbyname(hostname)) == NULL) {
			printf("%s: error: gethostbyname failed\n", __func__);
			return -1;
		}
	        addr.sin_addr.s_addr = ((struct in_addr *)host->h_addr_list[0])->s_addr;
	}
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("%s: error: socket failed\n", __func__);
		return -1;
	}
	if ((connect(sock, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
		close(sock);
		printf("%s: error: connect failed\n", __func__);
		return -1;
	}
#if 0
	int optval = 1;
	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&optval, sizeof(optval)) < 0) {
		close(sock);
		printf("setsockopt(): ERROR!!!\n");
		return -1;
	}
	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void *)&optval, sizeof(optval)) < 0) {
		close(sock);
		printf("setsockopt(): ERROR!!!\n");
		return -1;
	}
#endif
	fcntl(sock, F_SETFL, O_NONBLOCK | fcntl(sock, F_GETFL));
	return sock;
}       

/*
 * Listen on port on host
 */
int
ip_listen(__attribute__((unused)) const char *hostname, int port)
{
	struct sockaddr_in addr;
	int listen_sock, sock;

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((listen_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("%s: error: socket failed\n", __func__);
		return -1;
	}
	int optval = 1;
	if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval)) < 0) {
		close(listen_sock);
		printf("%s: error: setsockopt failed\n", __func__);
		return -1;
	}
	if (bind(listen_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(listen_sock);
		printf("%s: error: bind failed\n", __func__);
		return -1;
	}
	if (listen(listen_sock, 1) < 0) {
		close(listen_sock);
		printf("%s: error: listen failed\n", __func__);
		return -1;
	}
	while (1) {
		sock = accept(listen_sock, NULL, NULL);
		if (sock < 0) {
			if (errno == EINTR || errno == EAGAIN) {
				continue;
			}
			close(listen_sock);
			printf("%s: error: accept failed\n", __func__);
			return -1;
		}
		close(listen_sock);
		fcntl(sock, F_SETFL, O_NONBLOCK | fcntl(sock, F_GETFL));
		return sock;
	}
	/* Not reached */
}
