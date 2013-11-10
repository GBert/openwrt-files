/*
 * Velleman K8048 Programmer for FreeBSD and others.
 *
 * Copyright (c) 2005-2013 Darron Broad
 * All rights reserved.
 *
 * Licensed under the terms of the BSD license, see file LICENSE for details.
 */

#ifndef _KIO_H
#define _KIO_H

#include "k8048.h"

/* prototypes */
void usage(struct k8048 *, char *, char *);
uint8_t getbytearg(struct k8048 *, char *, char *);
uint16_t getshortarg(struct k8048 *, char *, char *);
uint32_t get24arg(struct k8048 *, char *, char *);
uint32_t getintarg(struct k8048 *, char *, char *);
uint8_t getportarg(struct k8048 *, char *, char *);
int main(int, char **);

#endif /* !_KIO_H */
